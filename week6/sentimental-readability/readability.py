from cs50 import get_string

text = get_string("Text: ")


def text_info(txt):

    l = 0
    w = 1
    s = 0

    for t in txt:
        if t.isalpha():
            l += 1
        else:
            pass

        if t == ' ':
            w += 1
        else:
            pass

        if t in ['.', '!', '?']:
            s += 1
        else:
            pass

    return l, w, s


num_letters, num_words, num_sentences = text_info(text)


L = num_letters * 100.0 / num_words
S = num_sentences * 100.0 / num_words
index = round(0.0588 * L - 0.296 * S - 15.8)


if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
