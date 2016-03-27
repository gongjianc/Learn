/*
** @file    point3d.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 18:23:26
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class Point
{
public:
    Point()
    : _ix(0)
    , _iy(0)
    {}
    Point(int x, int y)
    : _ix(x)
    , _iy(y)
    {}
    int getX() const
    { return _ix; }
    int getY() const
    { return _iy; }
    void disp() const
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" << endl;
    }
private:
    int _ix;
    int _iy;
};

class Point3D : public Point
{
public:
    Point3D(int x, int y, int z)
    : Point(x, y)
    , _iz(z)
    {}
    void disp() const
    {
        cout << "(" << getX()
             << "," << getY()
             << "," << _iz
             << ")" << endl;
    }
    int calcSum()
    { return getX() + getY() + _iz; }
private:
    int _iz;
};

int main(void)
{
    Point3D pt1(3, 4, 5);
    cout << "pt1.getX() = " << pt1.getX() << endl;
    cout << "pt1.calcSum() = " << pt1.calcSum() << endl;
    pt1.disp();
    return 0;
}
