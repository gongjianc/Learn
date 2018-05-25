#include <iostream>
#include <vector>

int main()
{
    std::vector<double> vec = {1, 2, 3, 4, 5, 6};
    for (auto elem : vec){
        elem *= 3;
        std::cout << "elem is :" << elem << std::endl;
    }

    for (auto& elem2 : vec){
        elem2 *= 3;
        std::cout << "elem2 is :" << elem2 << std::endl;
    }
    return 0;
}
