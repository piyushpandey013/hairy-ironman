#include <stdio.h>

int main()
{
    enum {
        DOWN = -1,
        UP = 1
    } direction;

    direction = DOWN;

    if (direction)
        printf( "UP\n" );
    else
        printf( "DOWN\n" );
    printf("%d\n", direction);

    if (-1)
        printf("negative\n");
    return 0;
}
