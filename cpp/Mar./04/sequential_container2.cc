/*
** @file    sequential_container2.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 03:39:56
*/
 
#include <iostream>
#include <vector>
#include <list>
#include <deque>
 
using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::deque;

int main(void)
{
	vector<int> vecInt(3, 1);
	vecInt.push_back(2);

	vector<int>::const_iterator cit;
	for(cit = vecInt.cbegin(); cit != vecInt.end(); ++cit){
		cout << *cit << " ";
	}
    cout << endl;

	vecInt.pop_back();
	for(cit = vecInt.cbegin(); cit != vecInt.end(); ++cit){
		cout << *cit << " ";
	}
	cout << endl;

	return 0;
}
