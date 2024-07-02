import csv

from collections import Counter

with open("favorites.csv", "r") as file:
    reader = csv.DictReader(file)
    counts = Counter()

    for row in reader:
        favorite = row["language"]
        counts[favorite] += 1

# print all content
for favorite, count in counts.most_common():
    print(f"{favorite}: {count}")

# print specific input
favorite = input("Favorite: ")
print(f"{favorite}: {counts[favorite]}")
