import sys
import socket
import ssl
import re
import logging
from urllib import parse
from contextlib import contextmanager
from dataclasses import dataclass
from typing import Any, Dict, Iterator, List, Tuple

logging.basicConfig(
    level=logging.NOTSET,
    format='%(asctime)s [%(levelname)s] %(name)s: %(message)s',
    datefmt='%H:%M:%S'
)

@dataclass(frozen=True)
class Request:
    line: str
    hostname: str
    path: str

@dataclass(frozen=True)
class Response:
    headers: Dict[str, str]
    cookies: Dict[str, str]
    body: str

class WebTester:
    def __init__(self, uri: str):
        self.uri = uri
        self.port = 443 
        self.logger = logging.getLogger(f'WebClient({uri})') 
    
    @contextmanager
    def _ssl_connection(self, protocols: List[str], hostname: str) -> Iterator[ssl.SSLSocket]:

        self.logger.debug(f'Connecting to {hostname} for protocols {protocols}')
        ctx = ssl.create_default_context()
        ctx.set_alpn_protocols(protocols)
        sock = socket.create_connection((hostname, self.port))
        ssock = ctx.wrap_socket(sock, server_hostname=hostname)
        self.logger.info(f'Connected to {hostname}')
        try:
            yield ssock

        except Exception as e:
            self.logger.error(f'Connection failed for {hostname}: {e}') 
            raise e
        finally:
            ssock.close()
            sock.close()

    def _get_http_headers(self, ssock: ssl.SSLSocket, req: str) -> Tuple[str, str]:
        http_headers = ''
        body = ''
        ssock.send(req.encode())
        while True:
            data = ssock.recv(4096)
            if not data: break
            if '\r\n\r\n' in http_headers:
                body += data.decode('utf-8')
            else:
                #todo: implement streaming protocol
                http_headers += data.decode('utf-8')
        #self.logger.debug(f'Raw headers for {self.uri}:\n{http_headers}')
        return (http_headers, body)


    
    def _parse_cookies(self, http_headers: str) -> Dict[str, str]:
        cookies = {}
        expr = re.findall(r'Set-Cookie:([^=]+)=([^;]+);', http_headers,
                          re.IGNORECASE)
        for cookie in expr:
            k, v = cookie
            cookies[k] = v
        self.logger.debug(f'Cookies for {self.uri}: {cookies}')
        return cookies

    def _parse_headers(self, http_headers: str) -> Dict[str, str]:
        headers = {}
        lines = http_headers.splitlines()
        if not lines: return headers
        status_line = lines[0].split(' ', 2)
        if len(status_line) >= 2:
            headers['protocol'] = status_line[0]
            headers['response'] = status_line[1]
        for line in lines[1:]:
            if not line or line == '\r': continue
            if ':' in line:
                k, v = line.split(':', 1)
                headers[k.lower().strip()] = v.strip()

        self.logger.info(f'parsed headers\n{headers}')
        return headers

    def _request(self, method: str, uri: str) -> Request:
        if not uri.startswith(('http://', 'https://')):
            uri = 'https://' + uri
        parsed = parse.urlparse(uri)
        hostname = parsed.hostname or ''
        path = parsed.path or '/'
        line = ''
        if method == 'POST':
            line = f'{method} {path} HTTP/1.1\r\nHost: {hostname}\r\nContent-Length: 0\r\nConnection: close\r\n\r\n'
        else:
            line = f'{method} {path} HTTP/1.1\r\nHost: {hostname}\r\nConnection: close\r\n\r\n'
        self.logger.info(f'request:\n{line}')
        return Request(line, hostname, path)
        
    def check_supports(self):
        results = { 'http/1.1': False, 'h2': False }
        req = self._request('GET', self.uri)
        for protocol in results.keys():
            with self._ssl_connection([protocol], req.hostname) as ssock:
                # force handshake to complete
                ssock.do_handshake()
                selected = ssock.selected_alpn_protocol()
                if selected == protocol:
                    results[protocol] = True
        self.logger.info(f'{self.uri} supports: {results}')
        return results

    def fetch(self, method: str, uri: str) -> Response:

        req = self._request(method, uri)
        with self._ssl_connection(['http/1.1'], req.hostname) as ssock:
            (http_headers, body) = self._get_http_headers(ssock, req.line)
            res = Response(
                        headers = self._parse_headers(http_headers),
                        cookies = self._parse_cookies(http_headers),
                        body = body
                    )
            self.pprint(req, res)
            if (res.headers['response'] == '302' 
            or res.headers['response'] == '301'):
                redirect = res.headers['location']
                # handle absolute redirect
                if parse.urlparse(redirect).hostname:
                    return self.fetch(method, redirect)
                # handle relative redirect
                return self.fetch(method, req.hostname + res.headers['location'])

            if res.headers['response'] == '401':
                print('password protected')
            return res
    def get(self):
        return self.fetch('GET', self.uri)

    def pprint(self, req: Request, res: Response):
        print(
            '\n---Request begin---\n',
            req.line,
            '\n---Request end---\n',
            'HTTP request sent, awaiting response...',
            '\n---Response header---\n',
            '\n'.join(f'{k}: {v}' for k, v in res.headers.items()),
            res.cookies,
            '\n---Response body---\n',
            '<insert body>'
            )


def main():
    if len(sys.argv) != 2:
        raise Exception('Usage: ./WebTester.py <URI>')

    wt = WebTester(sys.argv[1])
    wt.check_supports()
    wt.get().headers

if __name__ == '__main__':
    main()
