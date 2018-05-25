#include <list>
#include <iostream>

using namespace std;

int main()
{
    list<char> coll;

    for (char a = 'a'; a < 'z'; ++a){
        coll.push_back(a);
    }

    for(auto elem : coll){
        cout << elem << " ";
    }
    cout << endl;
}

