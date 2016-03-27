 ///
 /// @file    computer.cc
 /// @author  jaygong(gongjian0309@gmail.com)
 /// @date    2016-02-26 07:17:58
 ///
 
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

class Computer
{
private:
	//和struct类比 struct的访问修饰符是public 
	//class在默认情况下的访问修饰符是private
	//封装的好处是 不能被修改 隐藏实现 
	char brand_[20];
	float price_; //加下划线是为了说明是数据成员
public:
	//类生成的对象不包含成员函数所占据的空间
	void print();
	void setBrand(char *brand);
	void setPrice(float price);
};

void Computer::setBrand(char *brand)
{
	strcpy(brand_,brand);
}
void Computer::setPrice(float price)
{
	price_ = price;
}

void Computer::print()
{
	std::cout << "Brand:" << brand_ << std::endl;
	std::cout << "Price:" << price_ << std::endl;
}

class Y
{};//sizeof(Y) = 1 ? 是为了区分尤其产生的不同对象 

int main(void)
{
	Computer pc;//类的实例化
	pc.setPrice(5000);
	pc.setBrand("Dell");
	pc.print();	
	cout << endl
		<< "sizeof(Computer) " << sizeof(Computer) << endl;
	//对象里边只包含数据成员 
}
	
