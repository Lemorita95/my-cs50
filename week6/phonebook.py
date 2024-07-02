people = {
    "Carter": "+1...",
    "David": "+2...",
    "John": "+3...",
 }


name = input("Name: ")


if name in people:
   number = people[name]
   print(f"Found {number}")
else:
    print("Not found")
