#include <iostream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

struct{
    bool operator()(pair<string, int> a, pair<string, int>b) const {
        return a.second > b.second;
    }
}customGreater;


int main()
{

    map<string, int> coll;

    coll = { {"zhangsan", 0},
             {"lisi", 0}};

    //set vs. multiset  
    /* set<string> coll_ticket = {"zhangsan", "zhangsan", "lisi", "lisi", "zhangsan", "zhangsan"}; */
    multiset<string> coll_ticket = {"zhangsan", "zhangsan", "lisi", "lisi", "zhangsan", "zhangsan"};

    for (auto name : coll_ticket){
        for(auto elem : coll){
            if(name == elem.first){
                cout << name << " get one ticket" << endl;
                ++coll[elem.first];
            }
        }
    }


    /* sort(coll.begin(), coll.end(), greater<int>()); */
    /* sort(coll.begin(), coll.end(), customLess); */
    sort(coll.begin(), coll.end(), customGreater);

    /* for (auto elem : coll){
        if(elem.first == "zhangsan"){
            [>cout << "zhangsan get one ticket" << endl;<]
            ++elem.second; 
            cout << "elem.second is " << elem.second << endl;
            ++coll["zhangsan"];
        }else if(elem.first == "lisi"){
            [>cout << "lisi get one ticket" << endl;<]
            ++elem.second;
        }else
            continue;
    } */

    for(auto elem : coll){
        cout << elem.first << " get tickets:" << elem.second << endl;
    }

}
