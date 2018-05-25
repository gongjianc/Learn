#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void printNumber(std::vector<int> &number, std::function<bool (int)> filter)
{
    for (const int &i : number){
        if(filter(i))
            std::cout << i << ", ";
    }
    std::cout << std::endl;
}

bool isBetween(int i, int min, int max)
{
    return i > min && i < max;
}

/*
 * 获取vector中的最值
 * */
void getMinMax(std::vector<int> &number, std::function<void ()> &printer)
{
    int min = number.front();
    int max = number.front();
    for (int i : number){
        if (i < min)
            min = i;
        if (i > max)
            max = i;
    }
    printer = [=] (){
        std::cout << "min:" << min << std::endl;
        std::cout << "max:" << max << std::endl;
    };
}

int main()
{
    auto l = [] (const std::string& s) {
        std::cout << s << std::endl;
    };
    l("hello, world");

    std::vector<int> numbers {1, 2, 3, 4, 5, 10, 15, 20, 25, 35, 45, 50};
    int sum = 0;
    //以引用得形式捕获sum变量
    std::for_each(numbers.begin(), numbers.end(), [&sum] (const int &i) { sum += i;});
    std::cout << "sum is " << sum << std::endl;

    printNumber(numbers, [] (int i){return i % 5 ==0;});
    printNumber(numbers, [] (int i){return i > 20 ;});

    std::cout << "---------------" << std::endl;
    std::cout << "std::function var" << std::endl;
    //测试function
    std::function<bool (int)> func = [] (int i){ return i % 2 == 0;};
    printNumber(numbers, func);
    std::cout << "---------------" << std::endl;

    std::function<void()> printer;
    getMinMax(numbers, printer);
    printer();

    std::function<bool(int)> filter = std::bind(isBetween, std::placeholders::_1, 20, 40);
    printNumber(numbers, filter);
}


