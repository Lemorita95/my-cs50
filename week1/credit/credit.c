#include <cs50.h>
#include <stdio.h>
#include <string.h>

string label(long numero);
bool verificador(long numero);

int main(void)
{

    long ccnumber = get_long("Numero: ");

    if (verificador(ccnumber))
    {
        printf("%s", label(ccnumber));
    }
    else
    {
        printf("INVALID\n");
    }
}

string label(long numero)
{
    char str[20];
    sprintf(str, "%ld", numero);

    int tamanho = strlen(str);

    char fdigit = str[0];
    char sdigit = str[1];

    if (fdigit == '4' && (tamanho == 13 || tamanho == 16))
    {
        return "VISA\n";
    }
    else if (((fdigit == '3' && sdigit == '4') || (fdigit == '3' && sdigit == '7')) && (tamanho == 15))
    {
        return "AMEX\n";
    }
    else if (((fdigit == '5' && sdigit == '1') || (fdigit == '5' && sdigit == '2') || (fdigit == '5' && sdigit == '3') ||
              (fdigit == '5' && sdigit == '4') || (fdigit == '5' && sdigit == '5')) &&
             (tamanho == 16))
    {
        return "MASTERCARD\n";
    }
    else
    {
        return "INVALID\n";
    }
}

bool verificador(long numero)
{
    char str[20];
    sprintf(str, "%ld", numero);
    int tamanho = strlen(str);

    int odd = 0;
    int even = 0;

    for (int i = 0; i < tamanho; i++)
    {
        int posicao = tamanho - i;

        int x = str[i] - '0';

        if ((posicao % 2) == 0)
        {
            int additional = x * 2;
            even += (additional / 10) + (additional % 10);
        }
        else
        {
            odd += x;
        }
    }

    if ((even + odd) % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
