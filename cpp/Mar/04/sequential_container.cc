/*
** @file    sequential_container.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 03:00:08
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
	vector<int> vecInt;
	cout << "vecInt's size: " << vecInt.size() << endl;

	double darr[5] = {1, 2, 3, 4, 5};
	deque<double> dequeDb(darr, darr + 5);//为什么+5 左闭右开的区间 迭代器
	
	for(size_t idx = 0; idx != 5; ++idx){
		cout << dequeDb[idx] << endl;
	}

	deque<double>::iterator dit= dequeDb.end();
	while(dit != dequeDb.begin()){
		--dit;
		cout << *dit << endl;
	}

	list<float> listFl(3, 5);
	//cout << listFl[0] << endl;//list不支持随机迭代器的访问 下标运算符
	
	list<float>::iterator lit = listFl.begin();
	for(; lit != listFl.end(); ++lit){
		(*lit) += 2;
		cout << *lit << endl;
	}

	list<float> listFl2(4, 9);
	listFl.swap(listFl2);

	list<float>::iterator iter;
	for(iter = listFl.begin(); iter != listFl.end(); ++iter){
		cout << *iter << endl;
	}
	return 0;
}
