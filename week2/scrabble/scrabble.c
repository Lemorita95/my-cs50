#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int points(string s);

int main(void)
{
    string s1 = get_string("Player 1: ");
    string s2 = get_string("Player 2: ");

    int score1 = points(s1);
    int score2 = points(s2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int points(string s)
{
    int points = 0;

    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if (isupper(s[i]))
        {
            points += POINTS[s[i] - 'A'];
        }
        else if (islower(s[i]))
        {
            points += POINTS[s[i] - 'a'];
        }
        else
        {
            points += 0;
        }
    }

    return points;
}
