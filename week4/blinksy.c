#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *x;
    int *y;

    x = malloc(sizeof(int));

    *x = 42;
    //*y = 13;

    y = x;

    printf("x: %i, y: %i\n", *x, *y);

    *y = 13;

    printf("x: %i, y: %i\n", *x, *y);

    free(x);
}
