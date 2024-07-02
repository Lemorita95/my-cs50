from cs50 import get_int


while True:

    h = get_int("height: ")

    if h > 0 and h <= 8:
        break
    else:
        pass

maxh = h


def ppyramid(height):

    if (height <= 0):
        return

    ppyramid(height-1)

    print(' ' * (maxh - height) + '#' * height + '  ' + '#' * height)


ppyramid(h)
