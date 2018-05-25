#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

class Printer {
private:
    int min, max;
public:
    Printer(int x, int y)
    :min(x)
    ,max(y)
    {}

    void print()
    {
        std::cout << "min:" << min << std::endl;
        std::cout << "max:" << max << std::endl;
    }
};

void useringCallback(std::function<void ()>print)
{
    print();
}

int main()
{
    std::vector<int> numbers {1, 2, 3, 4, 5, 10, 15, 20, 25, 35, 45, 50};
    Printer printer = Printer(10, 50);
    std::function<void ()> print = std::bind(&Printer::print, printer);
    useringCallback(print);
}
