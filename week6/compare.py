from cs50 import get_int

x = get_int("What's x? ")
y = get_int("What's y? ")

if x < y:
    print("x is less then y")
elif x > y:
    print("x is greater then y")
else:
    print("x equals y")

#######

s = input("s: ")
t = input("t: ")

if s == t:
    print("same")
else:
    print("different")
