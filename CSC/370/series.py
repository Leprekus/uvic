import psycopg2
import sys
import getpass

def udf(title):
    return f'''

	with eps as (
		select season, e.id, e.episodeof from productions p
		inner join episodes e on p.id = e.episodeof
		--and season is not null
		and ptype = 'tvSeries'
		and title = '{title}'
	),
	epstats as (
		select  min(year) minyear,
			count(episodeof) nepisodes,
			avg(votes) avgvotes,
			avg(rating) avgrating,
			episodeof, season
		from ratings r
		inner join eps on r.id = eps.id
		inner join productions p on p.id = eps.id
		group by episodeof, season
	),
	ret as (
		select  cast(season as text) season, minyear, cast(nepisodes as integer) nepisodes, 
			floor((cast(avgvotes as double precision) * power(10, 1)) + 0.5) / power(10, 1) avgvotes, 
			floor((cast(avgrating as double precision) * power(10, 1)) + 0.5) / power(10, 1) avgrating, 
			floor((cast((avgrating - rating) as double precision) * power(10, 1)) + 0.5) / power(10, 1) diff
		from epstats
		inner join (
			select rating, id episodeof from ratings
			where id = (select episodeof from epstats limit 1)

		) A on epstats.episodeof = A.episodeof
	)
	select * from ret
	order by season asc nulls last;
'''
def runUDF(title):  return f"select * from user0169_episodes('{title}');"
def main():
    if len(sys.argv) < 2:
        raise Exception('usage: python3 series.py <series-name>')

    title = sys.argv[1]
    sys.stderr.write("Username: ")
    sys.stderr.flush()
    user: str = input()

    password = getpass.getpass("Password: ")

    conn = psycopg2.connect(
        host="localhost",
        port=4040,
        dbname="imdb",
        user = user,
        password = password

    )

    query = f"select * from productions where title = '{title}' and ptype ='tvSeries';"
    cur = conn.cursor()
    cur.execute(query)
    rows = cur.fetchall()
    if len(rows) > 1:
        sys.stderr.write("Error: Multiple matching records found")
        sys.exit(1)

    
    cur.execute(udf(title))
    rows = cur.fetchall()

    output = f'''<!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>Series Stats</title>
    </head>
    <body>
        <table border="1">
            <tr>
                <th>Season</th>
                <th>Year</th>
                <th>Episodes</th>
                <th>Avg. Votes</th>
                <th>Avg. Rating</th>
                <th>Difference</th>
            </tr>
            {''.join(f"<tr>{''.join(f'<td>{data}</td>' for data in row)}</tr>" for row in rows)} 
            </tr>
        </table>
    </body>
    </html>'''

    print(output)
    cur.close()
    conn.close()

if __name__ == "__main__":
    main()

