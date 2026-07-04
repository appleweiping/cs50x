-- 13.sql — the names of all people who starred in a movie with Kevin Bacon (born 1958),
-- excluding Kevin Bacon himself.
SELECT DISTINCT "people"."name"
  FROM "people"
  JOIN "stars" ON "stars"."person_id" = "people"."id"
 WHERE "stars"."movie_id" IN (
    SELECT "stars"."movie_id"
      FROM "stars"
      JOIN "people" ON "people"."id" = "stars"."person_id"
     WHERE "people"."name" = 'Kevin Bacon'
       AND "people"."birth" = 1958
 )
   AND "people"."name" != 'Kevin Bacon';
