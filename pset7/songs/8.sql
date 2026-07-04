-- 8.sql — the names of all songs that feature another artist (title contains "feat.").
SELECT "name" FROM "songs" WHERE "name" LIKE '%feat.%';
