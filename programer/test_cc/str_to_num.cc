#include <iostream>

using std::cout;
using std::endl;
using std::cin;

int strToNum(const char *str)
{
    int ret = 0;
    bool minus = false;
    const char *p = str;
    if(str != NULL){
        if(*p == '+')
            p++;
        else if(*p == '-'){
            minus = true;
            p++;
        }
        while(*p){
            if(*p >= '0' && *p <= '9'){
                ret = ret * 10 + (*p - '0');
                ++p;
            }else{
                printf("illegle\n");
                return -1;
            }
        }
    }
    if(minus)
        return 0 - ret;//负数
    return ret;
}

int main(void)
{
    char src[128];
    cout << "enter the string : ";
    cin >> src;
    cout << "the result is : " << strToNum(src) << endl;
    return 0;
}
    
