#include <assert.h>
#include <stdio.h>

int main(void)
{
    printf("assert test : ");
    assert(2 > 1);
    assert(1 >= 2);
    return 0;
}
