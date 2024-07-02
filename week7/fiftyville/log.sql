-- Keep a log of any SQL queries you execute as you solve the mystery.

-- the theft took place on July 28, 2023 and that it took place on Humphrey Street.

-- STEP 1
-- explore into crime_scene_reports at theft date and place and get description
-- 10:15 am @ bakery / 3 witness
-- Emma is bakery owner
SELECT id, description
FROM crime_scene_reports
WHERE
    (year = 2023)
    AND (month = 7)
    AND (day = 28)
    AND (street = 'Humphrey Street');

-- STEP 2
-- explore through interviews
-- Ruth: look into bakery security footage parking lot for cars LEFT @ ~ 10:15 --> EXPLORE BAKERY_SECURITY_LOGS
-- Eugene: thief withdraw money @ Leggett Street ATM before 10:15 --> EXPLORE ATM_TRANSACTIONS
-- Raymond: Phonecall @ ~ 10:15 (< 1 min long) thief planned to take the first flight out of fiftyville @ 29/07/2023 --> EXPLORE PHONE_CALLS, FLIGHTS
SELECT *
FROM interviews
WHERE
   (year = 2023)
   AND (month = 7)
   AND (day = 28)
   AND LOWER(transcript) like '%bakery%';

-- STEP 3
-- RUTH's TIP, explore bakery security footage log
-- OUTPUT: Thief possible license plates
SELECT license_plate
FROM bakery_security_logs
WHERE
    (year = 2023)
    AND (month = 7)
    AND (day = 28)
    AND (hour = 10)
    AND (minute BETWEEN 15 AND 25)
    AND (activity = 'exit')
ORDER BY
    minute ASC;

-- STEP 4
-- EUGENE's TIP, explore ATM TRANSACTIONS
-- intersect with license places exiting bakery parking lot
-- OUTPUT: (PEOPLE WHO WITHDRAW MONEY THAT DAY and EXIT THE BAKERY WITHIN TIME FRAME OF THEFT) = SUSPECTS
WITH lp AS (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE
        (year = 2023)
        AND (month = 7)
        AND (day = 28)
        AND (hour = 10)
        AND (minute BETWEEN 15 AND 25)
        AND (activity = 'exit')
)
SELECT b.person_id, p.name, p.phone_number, p.passport_number
FROM atm_transactions AS a
JOIN bank_accounts AS b ON a.account_number = b.account_number
JOIN people AS p on b.person_id = p.id
WHERE
    (a.year = 2023)
    AND (a.month = 7)
    AND (a.day = 28)
    AND (a.atm_location = 'Leggett Street')
    AND (a.transaction_type = 'withdraw')
    AND (p.license_plate IN lp);

-- STEP 5
-- RAYMOND's TIP, explore PHONE CALLS
-- intersect suspects phone numbers with phone_calls
-- as raymonds interview, the thief are the caller and the accomplice are the ticket buyer
-- OUTPUT: POSSIBLE SUSPECTS AND ACCOMPLICES
WITH suspects AS (
    WITH lp AS (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE
            (year = 2023)
            AND (month = 7)
            AND (day = 28)
            AND (hour = 10)
            AND (minute BETWEEN 15 AND 25)
            AND (activity = 'exit')
    )
    SELECT p.phone_number
    FROM atm_transactions AS a
    JOIN bank_accounts AS b ON a.account_number = b.account_number
    JOIN people AS p on b.person_id = p.id
    WHERE
        (a.year = 2023)
        AND (a.month = 7)
        AND (a.day = 28)
        AND (a.atm_location = 'Leggett Street')
        AND (a.transaction_type = 'withdraw')
        AND (p.license_plate IN lp)
)
SELECT *
FROM phone_calls p
WHERE
    (p.year = 2023)
    AND (p.month = 7)
    AND (p.day = 28)
    AND (p.duration < 60)
    AND (p.caller IN suspects);

-- STEP 6
-- looking throught the flights and passengers
-- airport: Fiftyville
-- date: 29/07/2023 first flight
-- OUTPUT: persons in suspect flight
WITH f AS ( --flight
    SELECT f.id
    FROM flights f
    JOIN airports a ON f.origin_airport_id = a.id
    WHERE
        (f.year = 2023)
        AND (f.month = 7)
        AND (f.day = 29)
        AND (a.abbreviation = 'CSF')
    ORDER BY f.hour ASC, f.minute ASC
    LIMIT 1
)
SELECT *
FROM passengers pa
JOIN people pe ON pa.passport_number = pe.passport_number
WHERE pa.flight_id IN f;

-- STEP 7
-- intersect SUSPECTS from STEP 5 with PASSENGERS at STEP 6 by Phone Number
-- OUTPUT: THIEF (BRUCE / ID 686048)
WITH s as ( -- suspects
    WITH suspects AS (
        WITH lp AS (
            SELECT license_plate
            FROM bakery_security_logs
            WHERE
                (year = 2023)
                AND (month = 7)
                AND (day = 28)
                AND (hour = 10)
                AND (minute BETWEEN 15 AND 25)
                AND (activity = 'exit')
        )
        SELECT p.phone_number
        FROM atm_transactions AS a
        JOIN bank_accounts AS b ON a.account_number = b.account_number
        JOIN people AS p on b.person_id = p.id
        WHERE
            (a.year = 2023)
            AND (a.month = 7)
            AND (a.day = 28)
            AND (a.atm_location = 'Leggett Street')
            AND (a.transaction_type = 'withdraw')
            AND (p.license_plate IN lp)
    )
    SELECT caller
    FROM phone_calls p
    WHERE
        (p.year = 2023)
        AND (p.month = 7)
        AND (p.day = 28)
        AND (p.duration < 60)
        AND (p.caller IN suspects)
)
, p AS ( -- passengers
    WITH f AS (
        SELECT f.id
        FROM flights f
        JOIN airports a ON f.origin_airport_id = a.id
        WHERE
            (f.year = 2023)
            AND (f.month = 7)
            AND (f.day = 29)
            AND (a.abbreviation = 'CSF')
        ORDER BY f.hour ASC, f.minute ASC
        LIMIT 1
    )
    SELECT pe.*
    FROM passengers pa
    JOIN people pe ON pa.passport_number = pe.passport_number
    WHERE pa.flight_id IN f
) SELECT p.*
FROM s
    INNER JOIN p
    ON s.caller = p.phone_number;

-- FIND ACCOMPLICE
-- THROUGHT STEP 5 (PHONECALLS)
-- ACCOMPLICE: ROBIN
WITH calls AS (
    WITH suspects AS (
        WITH lp AS (
            SELECT license_plate
            FROM bakery_security_logs
            WHERE
                (year = 2023)
                AND (month = 7)
                AND (day = 28)
                AND (hour = 10)
                AND (minute BETWEEN 15 AND 25)
                AND (activity = 'exit')
        )
        SELECT p.phone_number
        FROM atm_transactions AS a
        JOIN bank_accounts AS b ON a.account_number = b.account_number
        JOIN people AS p on b.person_id = p.id
        WHERE
            (a.year = 2023)
            AND (a.month = 7)
            AND (a.day = 28)
            AND (a.atm_location = 'Leggett Street')
            AND (a.transaction_type = 'withdraw')
            AND (p.license_plate IN lp)
    )
    SELECT *
    FROM phone_calls p
    WHERE
        (p.year = 2023)
        AND (p.month = 7)
        AND (p.day = 28)
        AND (p.duration < 60)
        AND (p.caller IN suspects)
) SELECT p1.name AS thief, p2.name AS accomplice
FROM calls c
JOIN people p1 ON c.caller = p1.phone_number
JOIN people p2 on c.receiver = p2.phone_number
WHERE p1.id = 686048;

-- FIND DESTINATION
-- THTOUGHT FLIGHTS (STEP 6)
-- FLIGHT_ID = 36
-- DESTINATION: New York City
SELECT a.*
FROM flights f
JOIN airports a on f.destination_airport_id = a.id
WHERE f.id = 36;

