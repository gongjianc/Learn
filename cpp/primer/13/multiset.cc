/*
** @file    multiset.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 00:16:48
*/
 
#include <iostream>
#include <set>
#include <vector>
 
using std::cout;
using std::endl;
using std::set;
using std::multiset;
using std::vector;

int main(void)
{
    vector<int> ivec;
    for(vector<int>::size_type i = 0; i != 10; ++i){
        ivec.push_back(i);
        ivec.push_back(i);
    }
    set<int> iset(ivec.cbegin(), ivec.cend());
    multiset<int> miset(ivec.cbegin(), ivec.cend());

    cout << ivec.size() << endl;
    cout << iset.size() << endl;
    cout << miset.size() << endl;

    return 0;
}
