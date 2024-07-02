from cs50 import get_string
import re

numero = get_string("cc number: ")


def label(n):

    if re.search("^(34|37)[0-9]{13}$", numero):
        return "AMEX"
    elif re.search("^(51|52|53|54|55)[0-9]{14}$", numero):
        return "MASTERCARD"
    elif re.search("^(4)[0-9]{12,15}$", numero):
        return "VISA"
    else:
        return "INVALID"


def verificador(n):

    l = len(n)

    odd = 0
    even = 0

    for idx, val in enumerate(numero):

        posicao = l - idx

        x = int(val)

        if posicao % 2 == 0:
            even += int((x * 2) / 10) + ((x * 2) % 10)
        else:
            odd += x

    if (odd + even) % 10 == 0:
        return True
    else:
        return False


if verificador(numero):
    print(label(numero))
else:
    print("INVALID")
