/*
** @file    HelloLog4Cpp.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-29 01:22:06
*/
 
#include <iostream>
#include <fstream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>

using std::cout;
using std::fstream;

int main(void)
{
	
	fstream fs("log.txt", std::ios::in | std::ios::out | std::ios::ate);
	log4cpp::OstreamAppender *osAppender = new log4cpp::OstreamAppender("osAppender", &fs);
	osAppender->setLayout(new log4cpp::BasicLayout());
	
	log4cpp::Category &root = log4cpp::Category::getRoot();
	root.addAppender(osAppender);
	root.setPriority(log4cpp::Priority::DEBUG);

	root.error("Hello log4cpp in a Error Message!");
	root.warn("Hello log4cpp in a Warning Message!");

	log4cpp::Category::shutdown();
	return 0;

}
