#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int numletters(string s);
int numwords(string s);
int numsentences(string s);

int main(void)
{
    string text = get_string("Text: ");

    float L = (numletters(text) * 100.0 / numwords(text));
    float S = (numsentences(text) * 100.0 / numwords(text));
    float index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) index);
    }
}

int numletters(string s)
{
    int l = 0;

    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if ((s[i] >= 'A') && (s[i] <= 'z'))
        {
            l += 1;
        }
        else
        {
            l += 0;
        }
    }
    return l;
}

int numwords(string s)
{
    int w = 0;

    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if (s[i] == ' ')
        {
            w += 1;
        }
        else
        {
            w += 0;
        }
    }
    return w + 1;
}

int numsentences(string s)
{
    int sent = 0;

    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if ((s[i] == '.') || (s[i] == '!') || (s[i] == '?'))
        {
            sent += 1;
        }
        else
        {
            sent += 0;
        }
    }
    return sent;
}
