-- 3.sql — the names of the top 5 longest songs, longest first.
SELECT "name" FROM "songs" ORDER BY "duration_ms" DESC LIMIT 5;
