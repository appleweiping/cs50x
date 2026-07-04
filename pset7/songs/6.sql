-- 6.sql — the names of all songs by Post Malone (without hardcoding his artist id).
SELECT "name"
  FROM "songs"
 WHERE "artist_id" = (
    SELECT "id" FROM "artists" WHERE "name" = 'Post Malone'
 );
