-- CS50x pset7 — Fiftyville: solving the theft of the CS50 duck.
-- The duck was stolen on July 28, 2023 at 10:15am on Humphrey Street.
-- This file is the sequence of queries used to identify the thief, the city they escaped
-- to, and their accomplice.

-- (1) The crime scene report for the day/place of the theft.
SELECT description
  FROM crime_scene_reports
 WHERE year = 2023 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- => "Theft ... 10:15am at the Humphrey Street bakery. Three witnesses ... mention the bakery."

-- (2) The three witness transcripts that mention the bakery.
SELECT name, transcript
  FROM interviews
 WHERE year = 2023 AND month = 7 AND day = 28
   AND transcript LIKE '%bakery%';
-- Ruth   : thief drove out of the bakery parking lot within 10 min of the theft.
-- Eugene : thief withdrew money at the Leggett Street ATM earlier that morning.
-- Raymond: thief made a call < 1 min, planning the earliest flight out tomorrow, and
--          asked the other person to buy the ticket.

-- (3) License plates leaving the bakery parking lot 10:15-10:25.
SELECT license_plate
  FROM bakery_security_logs
 WHERE year = 2023 AND month = 7 AND day = 28
   AND hour = 10 AND minute BETWEEN 15 AND 25
   AND activity = 'exit';

-- (4) People who withdrew from the Leggett Street ATM that morning.
SELECT people.name
  FROM people
  JOIN bank_accounts ON bank_accounts.person_id = people.id
  JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
 WHERE atm_transactions.year = 2023 AND atm_transactions.month = 7 AND atm_transactions.day = 28
   AND atm_transactions.atm_location = 'Leggett Street'
   AND atm_transactions.transaction_type = 'withdraw';

-- (5) The thief: a single person who (a) drove out of the bakery lot in that window,
--     (b) withdrew at the Leggett Street ATM, and (c) made a short (< 1 min) phone call
--     that day. Two candidates remain (Diana, Bruce).
SELECT people.name, people.passport_number, people.phone_number
  FROM people
 WHERE people.license_plate IN (
        SELECT license_plate FROM bakery_security_logs
         WHERE year = 2023 AND month = 7 AND day = 28
           AND hour = 10 AND minute BETWEEN 15 AND 25 AND activity = 'exit')
   AND people.id IN (
        SELECT bank_accounts.person_id FROM bank_accounts
          JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
         WHERE atm_transactions.year = 2023 AND atm_transactions.month = 7
           AND atm_transactions.day = 28 AND atm_transactions.atm_location = 'Leggett Street'
           AND atm_transactions.transaction_type = 'withdraw')
   AND people.phone_number IN (
        SELECT caller FROM phone_calls
         WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60);

-- (6) The earliest flight out of Fiftyville the next day (July 29) and its destination city.
SELECT flights.id, arrival.city AS destination_city, flights.hour, flights.minute
  FROM flights
  JOIN airports AS departure ON departure.id = flights.origin_airport_id
  JOIN airports AS arrival ON arrival.id = flights.destination_airport_id
 WHERE departure.city = 'Fiftyville'
   AND flights.year = 2023 AND flights.month = 7 AND flights.day = 29
 ORDER BY flights.hour, flights.minute
 LIMIT 1;
-- => flight 36, destination New York City, 8:20.

-- (7) The tie-breaker: which candidate was actually a passenger on that earliest flight.
SELECT people.name
  FROM people
  JOIN passengers ON passengers.passport_number = people.passport_number
 WHERE passengers.flight_id = 36
   AND people.name IN ('Diana', 'Bruce');
-- => Bruce is the thief.

-- (8) The accomplice: the receiver of the thief's short phone call.
SELECT receiver_person.name
  FROM phone_calls
  JOIN people AS caller_person ON caller_person.phone_number = phone_calls.caller
  JOIN people AS receiver_person ON receiver_person.phone_number = phone_calls.receiver
 WHERE phone_calls.year = 2023 AND phone_calls.month = 7 AND phone_calls.day = 28
   AND phone_calls.duration < 60
   AND caller_person.name = 'Bruce';
-- => Robin is the accomplice.
