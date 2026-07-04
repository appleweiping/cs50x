-- 7.sql — titles and ratings of all movies released in 2010, by rating (desc) then title.
SELECT "movies"."title", "ratings"."rating"
  FROM "movies"
  JOIN "ratings" ON "ratings"."movie_id" = "movies"."id"
 WHERE "movies"."year" = 2010
 ORDER BY "ratings"."rating" DESC, "movies"."title" ASC;
