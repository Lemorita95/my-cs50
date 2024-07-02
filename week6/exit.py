import sys

if len(sys.argv) != 2:
    print("Missing command-line argument")
    sys.exit()

print(f"Hello, {sys.argv[1]}")
