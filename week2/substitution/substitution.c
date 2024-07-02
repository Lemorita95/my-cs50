#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool chkkey(string s);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (chkkey(argv[1]))
        {
            string pt = get_string("plaintext:  ");
            printf("ciphertext: ");
            for (int i = 0, j = strlen(pt); i < j; i++)
            {
                if (isupper(pt[i]))
                {
                    printf("%c", toupper(argv[1][pt[i] - 'A']));
                }
                else if (islower(pt[i]))
                {
                    printf("%c", tolower(argv[1][pt[i] - 'a']));
                }
                else
                {
                    printf("%c", pt[i]);
                }
            }
            printf("\n");
        }
        else
        {
            printf("invalid key.\n");
            return 1;
        }
    }
    else if (argc != 2)
    {
        printf("invalid amount of in line arguments.\n");
        return 1;
    }
}

bool chkkey(string s)
{
    int len = strlen(s);

    if (len != 26)
    {
        return false; // chave com tamanho incorreto
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z'))
            {
                for (int j = i + 1; j < len; j++)
                {
                    if (toupper(s[i]) == toupper(s[j]))
                    {
                        return false; // valor duplicado encontrado
                    }
                }
            }
            else
            {
                return false; // possui caractere não alfabetico
            }
        }
        return true;
    }
}
