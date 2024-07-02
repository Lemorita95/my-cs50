SELECT DISTINCT people.name FROM movies JOIN stars on movies.id = stars.movie_id JOIN people ON people.id = stars.person_id WHERE movies.year = 2004 ORDER BY people.birth ASC;
