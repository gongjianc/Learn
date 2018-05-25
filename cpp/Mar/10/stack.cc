/*
** @file    stack.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 22:47:07
*/
 
#include <iostream>
#include <stack>
#include <string>
#include <vector>
 
using std::cout;
using std::endl;
using std::stack;
using std::string;
using std::vector;

int main(void)
{
   stack<int, vector<int> > stackInt;
   for(int idx = 1; idx != 21; ++idx)
       stackInt.push(idx);
   while(stackInt.size() > 0){
       cout << stackInt.top() << " ";
       stackInt.pop();
   }
   cout << endl;

   stack<string, vector<string> > stackStr;
   for(char ch = 'A'; ch <= 'Z'; ++ch){
       string s(2, ch);
       stackStr.push(s);
   }
   while(stackStr.size() > 0){
       cout << stackStr.top() << " ";
       stackStr.pop();
   }
   cout << endl;
   
   return 0;
}
