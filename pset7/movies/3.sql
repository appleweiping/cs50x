-- 3.sql — the titles of all movies released in 2018 or later, alphabetically.
SELECT "title" FROM "movies" WHERE "year" >= 2018 ORDER BY "title";
