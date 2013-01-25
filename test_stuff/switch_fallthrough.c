#include <stdio.h>

int main()
{
    int foo = 0;

    switch (foo)
    {
        case 0:
            if (1)
                foo = 1;
            else
                break;
        case 1:
            printf("hit");
            break;
        default:
            printf("default");
    }

    return 0;
}
