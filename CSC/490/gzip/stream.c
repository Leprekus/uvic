#include "stream.h"
#include "io_buf.h"
#include <stdlib.h>
#include <unistd.h>

struct Stream {
    size_t size;
    size_t count;
    u8 *data;
};

Stream *stream_init(size_t nbytes) {
    Stream *s = malloc(sizeof(Stream));
    if(!s) return NULL;
    u8 *d = malloc(sizeof(u8) * nbytes);
    if(!d) {
        free(s);
        return NULL;
    }
    s->size = nbytes;
    s->count = 0;
    s->data = d;
    return s;
}

void stream_reset(Stream *s) {
    s->count = 0;
}
int stream_read_from(Stream *s, int FD) {
    if(s->count == s->size) stream_reset(s);
    int n = read(FD, s->data + s->count , s->size - s->count);
    if(n < 0) return n;
    s->count += n;
    return n;
}
u8 *stream_data(Stream *s) {
    return s->data;
}

size_t stream_length(Stream *s) {
    return s->count;
}

void stream_push(Stream *s, const u8 data[], size_t len) {
    size_t end = s->count + len;
    if(end < s->size) {
        for(int i = s->count; i < end; i++)
            s->data[i] = data[i - s->count];
        s->count = end;
    }
}
