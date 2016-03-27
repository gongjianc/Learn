/*
** @file    Figure.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 20:11:16
*/
 
#include <iostream>
#include <cmath>
 
using std::cout;
using std::endl;

const float PI = 3.1415926;

class Figure{
public:
    virtual float area() = 0;
    virtual void display() = 0;
};

class Circle : public Figure{
public:
    Circle(float r = 0)
    : _radius(r)
    {
        cout << "Circle::Circle(float)" << endl;
    }
    float area()
    {
        return PI * _radius * _radius;
    }
    void display()
    {
        cout << "Circle: " ;
    }
private:
    float _radius;
};

class Rectangle : public Figure{
public:
    Rectangle(float fx = 0, float fy = 0)
    : _fx(fx)
    , _fy(fy)
    {
        cout << "Rectangle::Rectangle(float, float)" << endl;
    }
    float area()
    {
        return _fx * _fy;
    }

    void display()
    {
        cout << "Rectangle :" ;
    }
    
private:
    float _fx;
    float _fy;
};

class Triangle : public Figure{
public:
    Triangle(float fx, float fy, float fz)
    : _fx(fx)
    , _fy(fy)
    , _fz(fz)
    {
        cout << "Triangle(float, float)" << endl;
    }
    float area()
    {
        float p = (_fx + _fy + _fz) / 2;
        return sqrt(p * (p - _fx) * (p - _fy) * (p - _fz));
    }
    void display()
    {
        cout << "Triangle :" ;
    }
private:
    float _fx;
    float _fy;
    float _fz;
};

int main(void)
{
    //抽象类不能实例化
    //但是可以使用指针
    Figure *pFig = NULL;
    Circle c(3);
    Rectangle r(1.2f, 3.6f);
    Triangle t(6, 7, 8);

    pFig = &c;
    pFig->display();
    cout << pFig->area() << endl << endl;
    pFig = &r;
    pFig->display();
    cout << pFig->area() << endl << endl;
    pFig = &t;
    pFig->display();
    cout << pFig->area() << endl << endl;

    return 0;

}
