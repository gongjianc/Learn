/*
** @file    sequential_container3.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 04:12:04
*/
 
#include <iostream>
#include <vector>
 
using std::cout;
using std::endl;
using std::vector;

void display(vector<int> &vec)
{
    vector<int>::const_iterator cit;
    for(cit = vec.cbegin(); cit != vec.cend(); ++cit){
        cout << *cit << " ";
    }
    cout << endl;
}

int main(void)
{
    vector<int> vecInt(3, 0);
    display(vecInt);

    vector<int>::iterator it;
    it = vecInt.end();
    vecInt.insert(it, 1);
    display(vecInt);

    it = vecInt.end();
    vecInt.insert(it, 3, 2);
    display(vecInt);

    return 0;
}


