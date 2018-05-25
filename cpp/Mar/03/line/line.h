/*
** @file    nested.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-03 00:37:49
*/
#ifndef __LINE_H__
#define __LINE_H__
#include <iostream>

class Line
{
//PIMPL惯用法
private:
	class Impl;//头文件的循环依赖 前向声明
public:
	Line(int x1, int y1, int x2, int y2);
	void printLine();

	~Line();
private:
	Impl *_pImpl;
};
#endif
