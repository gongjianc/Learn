#include <array>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    array<string, 5> coll = {"hello", "wo"};

    //why use ++i ??
    for (int i = 0; i < coll.size(); ++i){
        cout << i << ":" << coll[i] << "-" << endl;
    }

    for (int i = 0; i < coll.size(); ++i){
        cout << coll[i] << "," ;
    }
    cout << "Finsh!";
    cout << endl;
}
