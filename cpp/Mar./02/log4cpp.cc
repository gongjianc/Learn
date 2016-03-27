/*
** @file    HelloLog4Cpp.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-29 01:22:06
*/
 
#include <iostream>
#include <string>
#include <sstream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

using std::cout;
using std::endl;
using std::fstream;
using std::ostringstream;
using std::string;

int main(void)
{
	log4cpp::PatternLayout *pLayout1 = new log4cpp::PatternLayout();
	pLayout1->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::PatternLayout *pLayout2 = new log4cpp::PatternLayout();
	pLayout2->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::Appender *fileAppender = new log4cpp::FileAppender("fileAppender", "wxb.log");
	fileAppender->setLayout(pLayout1);
	log4cpp::RollingFileAppender *rollFileAppender = 
		new log4cpp::RollingFileAppender("rollFileAppender", 
										 "rollwxb.log", 
										 5*1024,
										 1);
	rollFileAppender->setLayout(pLayout2);

	log4cpp::Category &root = log4cpp::Category::getRoot().getInstance("RootName");
	root.addAppender(fileAppender);
	root.addAppender(rollFileAppender);
	root.setPriority(log4cpp::Priority::DEBUG);

	for(int idx = 0; idx< 100; idx++){
		string strError;
		ostringstream oss;
		oss << idx << ": Root Error Message!";
		strError = oss.str();
		root.error(strError);
	}
	log4cpp::Category::shutdown();
	return 0;
}
