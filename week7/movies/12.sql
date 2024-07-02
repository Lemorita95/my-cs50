SELECT foo.title FROM (
SELECT movies.title, COUNT(*) AS ct FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name IN ('Bradley Cooper', 'Jennifer Lawrence')
GROUP BY movies.title)foo
WHERE foo.ct > 1;
