#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *s = "HI!";
    printf("%p\n", s);
    //printf("%c", *(s + 1));
    //printf("%c", *(s + 2));

    int *n = malloc(sizeof(int));
    *n = 50;
    printf("%i\n", *n);
    free(n);
}
