-- 5.sql — titles and release years of all Harry Potter movies, oldest first.
SELECT "title", "year"
  FROM "movies"
 WHERE "title" LIKE 'Harry Potter%'
 ORDER BY "year";
