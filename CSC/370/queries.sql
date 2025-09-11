-- explore performance among:
explain select * from (select * from productions natural join ratings) as T where title = 'Mission Impossible';
/*
                                         QUERY PLAN
-------------------------------------------------------------------------------------------
 Nested Loop  (cost=0.99..1633.02 rows=18 width=83)
   ->  Index Scan using idxprodtitle on productions  (cost=0.56..530.73 rows=131 width=71)
         Index Cond: (title = 'Mission Impossible'::text)
   ->  Index Scan using ratings_pkey1 on ratings  (cost=0.43..8.41 rows=1 width=22)
         Index Cond: (id = productions.id)
 */
SET enable_hashjoin = off
explain with t as (select * from productions natural join ratings) select * from t where title = 'Mission Impossible';
/*
                                                   QUERY PLAN
-----------------------------------------------------------------------------------------------------------------
 CTE Scan on t  (cost=587008.21..621512.59 rows=7668 width=152)
   Filter: (title = 'Mission Impossible'::text)
   CTE t
     ->  Merge Join  (cost=1.03..587008.21 rows=1533528 width=83)
           Merge Cond: (productions.id = ratings.id)
           ->  Index Scan using n_productions_pkey on productions  (cost=0.56..481539.99 rows=11266375 width=71)
           ->  Index Scan using ratings_pkey1 on ratings  (cost=0.43..58133.35 rows=1533528 width=22)
(7 rows)
 */
explain with t as (select * from productions natural join ratings) select * from t where title = 'Mission Impossible';
/*
                                             QUERY PLAN
----------------------------------------------------------------------------------------------------
 CTE Scan on t  (cost=522796.01..557300.39 rows=7668 width=152)
   Filter: (title = 'Mission Impossible'::text)
   CTE t
     ->  Gather  (cost=55837.38..522796.01 rows=1533528 width=83)
           Workers Planned: 2
           ->  Hash Join  (cost=54837.38..368443.21 rows=638970 width=83)
                 Hash Cond: (productions.id = ratings.id)
                 ->  Parallel Seq Scan on productions  (cost=0.00..182273.23 rows=4694323 width=71)
                 ->  Hash  (cost=26682.28..26682.28 rows=1533528 width=22)
                       ->  Seq Scan on ratings  (cost=0.00..26682.28 rows=1533528 width=22)
(10 rows)

 */
