#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;

    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);

    for (int i = 0; i < h; i++)
    {
        printf("%.*s", h - i - 1, "        ");
        printf("%.*s", i + 1, "########");
        printf("%.*s", 2, "  ");
        printf("%.*s\n", i + 1, "########");
    }
}
