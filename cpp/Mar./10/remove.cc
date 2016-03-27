/*
** @file    remove.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 19:16:06
*/
 
#include <iostream>
#include <vector>
#include <algorithm>
 
using std::cout;
using std::endl;
using std::vector;

//value_type 不用使用迭代器
void print(vector<int>::value_type value)
{
    cout << value << " ";
}

int main(void)
{
    vector<int> vecInt;
    vecInt.reserve(10);
    cout << vecInt.capacity() << endl;
    for(size_t idx = 1; idx <= 10; ++idx)
        vecInt.push_back(idx);

    vecInt[3] = vecInt[5] = vecInt[9] = 99;
    for_each(vecInt.begin(), vecInt.end(), print);
    cout << endl;
    //vector<int>::iterator iter;
    //remove并不删除元素，只进行覆盖
    //返回值是要删除的第一个元素的迭代器
    //remove-erase惯用法
    //迭代器失效 
    vecInt.erase(remove(vecInt.begin(), vecInt.end(), 99), vecInt.end()); 
    //iter = remove(vecInt.begin(), vecInt.end(), 99);
    //vecInt.erase(iter, vecInt.end());
    for_each(vecInt.begin(), vecInt.end(), print);
    cout << endl;
    //cout << *iter << endl;

    return 0;
}
