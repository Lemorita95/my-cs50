SELECT DISTINCT people.name FROM ratings JOIN directors ON directors.movie_id = ratings.movie_id JOIN people ON people.id = directors.person_id where ratings.rating >= 9;
