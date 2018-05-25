#include <iostream>
#include <algorithm>

using namespace std;

int cmp(int a, int b)
{
    return a > b;
}

int main()
{
    int num[5] = {1, 4, 3, 5, 2};
    cout << "Before:" << endl;
    for(int i = 0; i < 5; i++){
        cout << "num[" << i << "]is " << num[i] << endl;
    }
    cout << "After:" << endl;
    sort(num, num + 5, cmp); //这里的sort的排序会修改原来的内容
    for(int i = 0; i < 5; i++){
        cout << "num[" << i << "]is " << num[i] << endl;
    }
}
