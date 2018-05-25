#include <iostream>

using namespace std;

class String{
public:
    String (const char *p){
        cout << "string is " << p << endl;
    }
    String (int n){
        cout << "num is " << n << endl;
    }

};

int main()
{
    String s1 = "hello";
    String s2 = 2;
    String s3 = 'a';
}
