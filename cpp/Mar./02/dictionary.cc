 ///
 /// @file    dictionary.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-28 19:01:03
 ///
 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct Element{
	string word;
	int frequency;
};

class Dictionary
{
public:
	void readFile(const char *filename);
	void writeDic(const char *filename);
private:
	vector<Element> vecElem;
};

void Dictionary::readFile(const char *filename)
{
	ifstream ifs(filename);
	if(!ifs.good()){
		cout << "ifstream open error" << endl;
		return;
	}
//	cout << "open success" << endl;
	Element elem;
	vector<Element>::iterator iter;
	vecElem.reserve(100);

//	cout << "vector.empty" << vecElem.empty() << endl;
	
	while(ifs >> elem.word){
		if(atoi(elem.word.c_str())){
			continue;
		}

		if(vecElem.empty()){
			elem.frequency = 1;
			vecElem.push_back(elem);
			cout << "push success" << endl;
			continue;
		}

		for(iter = vecElem.begin(); iter != vecElem.end(); ++iter){
			if(iter->word == elem.word){
				++iter->frequency;
				break;
			}
		}
		if(iter == vecElem.end()){
			elem.frequency = 1;
			vecElem.push_back(elem);
		}
	}
	cout << "readFile success" << endl;
	ifs.close();
}

void Dictionary::writeDic(const char *filename)
{
	ofstream ofs(filename);
	if(!ofs.good()){
		cout << "ofstream open error" << endl;
		return ;
	}
	vector<Element>::iterator iter;
	for(iter = vecElem.begin(); iter != vecElem.end(); ++iter){
		ofs << iter->word << ": " << iter->frequency << endl;
	}
	cout << "write success." << endl;
}

			
int main(void)
{
	Dictionary *dic = new Dictionary();
	dic->readFile("The_Holy_Bible.txt");
	dic->writeDic("dictionary.txt");

	return 0;
}
