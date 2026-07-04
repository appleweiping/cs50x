-- 4.sql — the names of songs that are "danceable": danceability, energy, and valence
-- all strictly greater than 0.75.
SELECT "name"
  FROM "songs"
 WHERE "danceability" > 0.75
   AND "energy" > 0.75
   AND "valence" > 0.75;
