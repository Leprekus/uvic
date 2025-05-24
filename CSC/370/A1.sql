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
and year >= 2000;

/*
2) Which are the TV series (not episodes) with an average rating above 9.0 and at least 100,000 votes that
were made since 2010 (year 2010 including).
 */

with tvseries as (
	select * from productions
	where ptype = 'tvSeries'
	and year >= 2010
),
r as (
	select id from ratings
	where rating > 9.0
	and votes >= 100000
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
	select A.director, A.id from directors as A
	inner join directors as B
		on A.id <> B.id
		and A.director = B.director
),
dirsTwo as (
	select director, id from dirs 
	where id in (select id from movies)
)
select * from dirsTwo;
	

-- PART III
-- What episodes of the tvMiniSeries with title Ahsoka do not have an average rating?
with ahsoka as (
	select id from productions
	where title = 'Ahsoka'
),
eps as (
	select id from episodes
	where episodeof in (select id from ahsoka)
),
avgrating as (
	select rating from ratings
	where id in (select id from ahsoka)
)
select id from ratings
where id in (select id from eps)
and rating <> (select rating from avgrating);
