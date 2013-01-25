
#include <stdio.h>

struct tester
{
    unsigned int foo;
    char bar;
};

void f( struct tester* t )
{
    t->bar = 'c';
}

int main()
{
    struct tester t;
    t.bar = 'd';

    f(&t);

    printf("%c\n", t.bar);

    return 0;
}
