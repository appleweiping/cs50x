-- 10.sql — the names of all people who directed a movie rated at least 9.0.
SELECT DISTINCT "people"."name"
  FROM "people"
  JOIN "directors" ON "directors"."person_id" = "people"."id"
  JOIN "ratings" ON "ratings"."movie_id" = "directors"."movie_id"
 WHERE "ratings"."rating" >= 9.0;
