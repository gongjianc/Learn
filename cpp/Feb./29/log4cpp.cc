/*
** @file    HelloLog4Cpp.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-02-29 01:22:06
*/
 
#include <iostream>
#include <fstream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>

using std::cout;
using std::endl;
using std::fstream;

int main(void)
{
	
//	fstream fs("log.txt", std::ios::in | std::ios::out | std::ios::ate);
//	if(!fs.good()){
//		cout << "open file error " << endl;
//		return -1;
//	}


	log4cpp::OstreamAppender *osAppender = new log4cpp::OstreamAppender("osAppender", &cout);

	log4cpp::PatternLayout *pLayout = new log4cpp::PatternLayout();

	pLayout->setConversionPattern("%d: %p %c %x: %m %n");
	osAppender->setLayout(pLayout);
	
	log4cpp::Category &root = log4cpp::Category::getRoot();

	log4cpp::Category &infoCategory = root.getInstance("infoCategory");
	infoCategory.addAppender(osAppender);
	infoCategory.setPriority(log4cpp::Priority::INFO);

	infoCategory.info("system is running");
	infoCategory.error("system has a error, can`t find a file");
	infoCategory.fatal("system has a fatal error, must bu shutdown");
	infoCategory.warn("system has a warning");
	infoCategory.info("system shutdown, you can find some information in system log");

	log4cpp::Category::shutdown();

	return 0;

}
