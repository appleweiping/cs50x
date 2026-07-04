-- 12.sql — the titles of all movies starring both Bradley Cooper and Jennifer Lawrence.
SELECT "movies"."title"
  FROM "movies"
  JOIN "stars" ON "stars"."movie_id" = "movies"."id"
  JOIN "people" ON "people"."id" = "stars"."person_id"
 WHERE "people"."name" = 'Bradley Cooper'
 INTERSECT
SELECT "movies"."title"
  FROM "movies"
  JOIN "stars" ON "stars"."movie_id" = "movies"."id"
  JOIN "people" ON "people"."id" = "stars"."person_id"
 WHERE "people"."name" = 'Jennifer Lawrence';
