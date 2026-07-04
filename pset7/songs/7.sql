-- 7.sql — the average energy of songs by Drake (without hardcoding his artist id).
SELECT AVG("energy")
  FROM "songs"
 WHERE "artist_id" = (
    SELECT "id" FROM "artists" WHERE "name" = 'Drake'
 );
