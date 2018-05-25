 ///
 /// @file    vector.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 17:52:33
 ///
 
#include <iostream>
#include <vector>

//vector 向量 动态的调整大小 容器 

using std::vector;
using std::cout;
using std::endl;

void print(vector<int> &vec)
{
	cout << "vecInt`s size: " << vec.size() << endl;
	//capacity 
	cout << "vecInt`s capacity(): " << vec.capacity() << endl;

	//how to use vector
	vector<int>::iterator iter;
	for(iter = vec.begin(); iter != vec.end(); ++iter){
		cout << *iter << endl;
	}

}
int main(void)
{
	vector<int> vecInt;
	print(vecInt);

	vecInt.push_back(1);
	print(vecInt);

	vecInt.push_back(2);
	print(vecInt);
	return 0;
}

