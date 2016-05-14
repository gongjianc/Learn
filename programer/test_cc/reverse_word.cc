/*
** @file    reverse_word.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-09 14:15:27
*/
 
#include <iostream>
#include <string.h>
 
using std::cout;
using std::endl;

void reverseWord(char *str, int len)
{
    int st = 0, ed = len - 1;
    char temp;
    while(ed > st){
        temp = str[st];
        str[st++] = str[ed];
        str[ed--] = temp;
    }
}

char *reverse(char *str)
{
    char *p = str;
    char *q = p;
    int count = 0;
    int strLen = strlen(str);
    reverseWord(str, strLen);
    while(*p){
        if(*p == ' '){
            reverseWord(q, count);
            count = 0;
            p++;
            q = p;
        }
        count++;
        p++;
    }
    reverseWord(q, count);
    return str;
}

int main(void)
{
    char str[] = "Today is Friday";
    cout << "reverseWord : " << reverse(str) << endl;
    return 0;
}
