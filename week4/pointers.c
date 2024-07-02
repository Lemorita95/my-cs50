#include <stdio.h>

int main(void)
{
    int x = 50;
    int *p = &x;
    printf("endereço de x: %p\n", &x); //endereço de x
    printf("endereço de p: %p\n", &p); // endereço de p
    printf("conteudo de p: %p\n", p); // conteudo de p (endereço de x)
    printf("conteudo do endereço apontado por p: %i\n", *p); // o que tem no endereço apontado por p
}
