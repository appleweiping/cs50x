-- 9.sql — the names of all people who starred in a 2004 movie, by birth year.
SELECT DISTINCT "people"."name"
  FROM "people"
  JOIN "stars" ON "stars"."person_id" = "people"."id"
  JOIN "movies" ON "movies"."id" = "stars"."movie_id"
 WHERE "movies"."year" = 2004
 ORDER BY "people"."birth";
