-- PART I
/*
 1) From 2000 (inclusive), in which movies has Clint Eastwood been both a director and had a major role?
 */
with p as (
    select pid from persons
    where personname = 'Clint Eastwood'
),
r as (
    select id from roles
    where pid = (select pid from p)
    and roletype = 'actor'
),
d as (
    select id from directors
    where director = (select pid from p)
)
select * from productions
where id in (
    select id from r
    intersect
    select id from d
)
and year >= 2000
order by year asc;

/*
2) Which are the TV series (not episodes) with an average rating above 9.0 and at least 50,000 votes that
were made since 2015 (year 2015 including).
 */

with tvseries as (
	select * from productions
	where ptype = 'tvSeries'
	and year >= 2015
),
r as (
	select id from ratings
	where rating > 9.0
	and votes >= 50000
	order by votes asc
)
select * from productions
where id in (
	select id from tvseries
	intersect
	select id from r 
);

-- PART II
/*
1) What is the production (or productions, as they might be more than one) 
directed by ’Christopher Nolan’with the highest rating.
*/

with nolan as (
	select pid from persons
	where personname = 'Christopher Nolan'
),
movies as (
	select id from directors
	where director in (select pid from nolan)
),
highest as (
	select max(rating) from ratings
	where id in (select id from movies)
),
movies_highest as (
	select id from ratings
	where id in (select id from movies)
	and rating = (select max from highest)

)
select * from productions
where id in (select id from movies_highest);

/*
2) Which directors have directed two or more movies with a rating of at least 8.5 and at least 500,000 votes?
-- director | title | year
*/


-- filter by votes and ratings
with movies as (
	select id from ratings
	where rating >= 8.5
	and votes >= 500000
),
dirs as (
	select director, id from directors
	where id in (select id from movies)
),
filtered as (
	select distinct A.director, A.id from dirs A
	inner join dirs B
		on A.director = B.director
		and A.id <> B.id
),
names as (
	select personname, pid, id from persons
	right join filtered
		on pid = director
),
titles as (
	select title, year, id from productions
	where id in (select id from filtered)
)

select personname as director, title, year from names
	right join titles
		on titles.id = names.id
	order by director, title asc;

-- PART III
-- What episodes of the tvMiniSeries with title Andor do not have an average rating?
with A as ( 
	select id from productions where title = 'Andor' 
	and ptype = 'tvSeries'
),
epsids as (
	select id from episodes
	where episodeof in (select id from A)
),
nullids as (
	select id from  epsids
	where id not in (select id from ratings)
)
select id, episodeof, season, episode from episodes
where id in (select id from nullids);

