from cs50 import SQL

db = SQL("sqlite:///favorites.db")

favorite = input("Favorite: ")

rows = db.execute("SELECT COUNT(*) as n from favorites WHERE language = ?", favorite)
row = rows[0]

print(row["n"])
