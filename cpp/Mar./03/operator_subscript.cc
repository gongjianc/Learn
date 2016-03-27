/*
** @file    operator_subscript.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-02 19:45:23
*/
 
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

class CharArray
{
public:
	CharArray(int sz)
	: _iLen(sz)
	, _pBuf(new char[_iLen])
	{
		cout << "CharArray(int)" << endl;
	}
	~CharArray()
	{
		cout << "~CharArray()" << endl;
		delete []_pBuf;
	}

	int capacity()
	{
		return _iLen;
	}

	char &operator[](int idx)
	{
		static char nullChar = '\0';
		if(idx < _iLen && idx >= 0){
			return _pBuf[idx];
		}else{
			cout << "cross" <<endl;
			return nullChar;
		}
	}
private:
	int _iLen;
	char *_pBuf;
};

int main(void)
{
	char pstr[] = "helloworld";
	CharArray ca(strlen(pstr) + 1);

	for(int idx = 0; idx != ca.capacity(); ++idx){
		ca[idx] = pstr[idx];
	}

	for(int idx = 0; idx < ca.capacity(); ++idx){
		cout << ca[idx];
	}

	cout << endl;

	return 0;
}
