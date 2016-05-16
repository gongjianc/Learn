#include <stdio.h>
#include <stdlib.h>

char *test_malloc()
{
    char *p = malloc(16);
    p = "hello,world";
    return p;
}
int main(void)
{
    printf("test_malloc: %s\n", test_malloc());

}
