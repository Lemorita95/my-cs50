WITH m as (
    SELECT stars.movie_id FROM stars
    JOIN people on stars.person_id = people.id
    WHERE (people.name = 'Kevin Bacon' AND people.birth = 1958)
)
SELECT DISTINCT people.name from stars
JOIN people on stars.person_id = people.id
WHERE stars.movie_id IN m
AND NOT (people.name = 'Kevin Bacon' AND people.birth = 1958);
