
-- Part I
-- 1)
with p1921 as (
	select D.director, D.id from directors D
		inner join (select id from productions where ptype = 'movie' and year = 1921) A
			on A.id = D.id
),
dcnt as (
	select distinct director,
		count(director) over(partition by director) n
		from p1921
),
mostdirected as (
	select A.director, A.n from dcnt A
		inner join dcnt B
			on A.n = (select distinct max(n) over() from dcnt)
),
person as (
	select pid, personname from persons
	where pid in (select director from mostdirected)
),
alltimecnt as (
			select id,
			count(id) over() n
			from (
				select id from directors
				where director in (select director from mostdirected)
				intersect
				select id from productions
				where ptype = 'movie'
			) A
		),
alltimedirected as (
	select alltimecnt.id, alltimecnt.n, director,
		'movie' as productiontype
		from alltimecnt
		inner join directors
			on directors.id = alltimecnt.id
)

select distinct person.personname, person.pid,
	atd.productiontype, md.n as n1921, atd.n
	from mostdirected md
	inner join person on md.director = person.pid
	inner join alltimedirected atd on atd.director = md.director;


-- 2)


with shows4seasons as (
	select episodeof, A.id, season from episodes A
	inner join productions
		on A.id = productions.id
	where exists (select id from episodes B where A.episodeof = B.episodeof and season >= 4)

),
epscnt as (
	select episodeof, count(episodeof) n from shows4seasons
	group by episodeof
),
maxeps as (
	select max(n) from epscnt
),
shows as (
	select episodeof, shows4seasons.id, coalesce(season, -1) as season, year from shows4seasons
	inner join productions
		on shows4seasons.id = productions.id
	where episodeof = (select episodeof from epscnt -- select shows that have the max episode count
				where n = (select max from maxeps)
		)
),
minyear as (
	select episodeof, season, min(year) as year from shows
	group by episodeof, season
),
epcnt as (
	select episodeof, season, count(episodeof) as nepisodes
	from shows
	group by episodeof, season
),
ratingcnt as (

	select episodeof, season, count(id) as withratings, avg(rating) as avgrating, sum(votes) as sumvotes from (
		select episodeof, season, ratings.id, rating, votes from shows
		left join ratings
			on ratings.id = shows.id
	)A 
	group by episodeof, season
),
ret as (
	select  title, my.season, my.year, nepisodes, 
		coalesce(withratings, 0) as withratings, coalesce(avgrating, 0) as avgrating, coalesce(sumvotes, 0) as sumvotes 
		from minyear my

		inner join productions on productions.id = my.episodeof
		inner join epcnt on epcnt.episodeof = my.episodeof and epcnt.season = my.season
		inner join ratingcnt on ratingcnt.episodeof = my.episodeof and ratingcnt.season = my.season
		order by year
)
select * from ret;


-- 3)

-- qeuestions for office hours:
-- Tony Kaye does not appear in the result, but he meets the criteria
-- Why is there such a big discrepancy between my nother eand expected nother?
-- i am counting avgratings (which are correct) shouldn't nother be at least as much as count(ratings)?
with great as (
	select id, rating from ratings r
		where r.rating >= 8.5
		and r.votes >= 200000
		and id in (select id from productions where ptype = 'movie')
),
dir as (
	select director from directors
	where id in (select id from productions where ptype = 'movie')
	group by director
	having count(director) >= 7
),
prod as (
	select d.id, d.director, rating from directors d
		inner join great on d.id = great.id
		inner join dir on d.director = dir.director
	where d.id = great.id
	and d.director = dir.director
),
other as (
	select directors.director, ratings.rating from directors 
	inner join prod on directors.director = prod.director
	inner join ratings on directors.id = ratings.id
	where directors.id not in (select id from prod)
	and directors.id in (select id from productions where ptype = 'movie')
),
df as (
	select pid, personname, p.director, ngreat, avggreat, nother, avgother from prod p
		inner join persons on p.director = pid
		inner join great g on g.id = p.id
		inner join (
			select director, count(director) ngreat from prod
			group by director
		) dircnt on p.director = dircnt.director
		inner join (
			select director, avg(rating) avggreat from prod
			group by director
		) ratingcnt on p.director = ratingcnt.director
		inner join (
			select director, count(director) nother, avg(rating) avgother from other
			group by director
		) othercnt on p.director = othercnt.director

	
)
select distinct pid, personname as director, prop, diff, ngreat, avggreat, nother, avgother from df
	inner join (
		select director, cast(ngreat as decimal)/ cast(nother as decimal) prop from df
	) proportion on proportion.director = df.director
	inner join (
		select director, cast(avggreat as decimal) - cast(avgother as decimal) diff from df
	) difference on difference.director = df.director
	order by avgother desc, prop desc, personname asc;


-- part II

create or replace function user0169_series (
                str text
        )
	returns setof series
        language plpgsql as
        $$
        begin
		return query
		with pstats as (
			select title, year, runtime, id from productions
			where ptype = 'tvSeries' and title = str
			limit 1
		),
		estats as (
			select count(episodeof) nepisodes,
			       max(season) nseasons,
			       episodeof
			from (
				select episodeof, season from episodes e
				inner join pstats on e.episodeof = pstats.id
			) A
			group by episodeof
		),
		rstats as (
			select  episodeof,
				avg(rating) avgrating,
				sum(votes) numvotes
			from (
				select episodeof, rating, votes from ratings r
				inner join (
					select id, episodeof from episodes
					where episodeof = (select id from pstats)
				) A on r.id = a.id
			) B
			group by episodeof
			

		),
		ret as (
			select title, year, nseasons, cast(nepisodes as integer) nepisodes, runtime, avgrating, cast(numvotes as integer)
			from pstats
			inner join estats on pstats.id = estats.episodeof
			inner join rstats on pstats.id = rstats.episodeof
		)
                select * from ret limit 1;
        end;
        $$;

select user0169_series('Borgen');


create or replace function user0169_episodes (
                str text
        )

        returns setof seasons
        language plpgsql as
        $$
        begin
		
	return query
	with eps as (
		select season, e.id, e.episodeof from productions p
		inner join episodes e on p.id = e.episodeof
		--and season is not null
		and ptype = 'tvSeries'
		and title = str
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
        end;
        $$;

select user0169_episodes('The Lone Ranger');
	-- Serial Psych
