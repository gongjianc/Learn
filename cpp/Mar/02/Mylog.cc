/*
** @file    MyLog.cc
** @author  jaygong(gongjian0309@gmail.com)
*/
 
#include "Mylog.h"

using std::cout;
using std::endl;
using std::fstream;
using std::ostringstream;
using std::string;

namespace logsystem
{
MyLog *MyLog::_pInstance = NULL;

MyLog &MyLog::getInstance()
{
	if(_pInstance == NULL)
		_pInstance = new MyLog();
	return *_pInstance;
}

MyLog::MyLog()
: _catRef(log4cpp::Category::getRoot())
{
	cout << "MyLog::MyLog()" << endl;
	log4cpp::PatternLayout *pLayout1 = new log4cpp::PatternLayout();
	pLayout1->setConversionPattern("%d: %p %c %x: %m%n");
	log4cpp::PatternLayout *pLayout2 = new log4cpp::PatternLayout();
	pLayout2->setConversionPattern("%d: %p %c %x: %m%n");

	log4cpp::Appender *fileAppender = new log4cpp::FileAppender("fileAppender", "wxb.log");
	fileAppender->setLayout(pLayout1);

	log4cpp::OstreamAppender *oStreamAppender = new log4cpp::OstreamAppender("oStreamAppender", &cout);
	oStreamAppender->setLayout(pLayout2);

	_catRef.addAppender(fileAppender);
	_catRef.addAppender(oStreamAppender);

	_catRef.setPriority(log4cpp::Priority::DEBUG);

}

void MyLog::destory()
{
	delete _pInstance;
	log4cpp::Category::shutdown();
}

void MyLog::fatal(const char *msg)
{
	_catRef.fatal(msg);
}

void MyLog::error(const char *msg)
{
	_catRef.error(msg);
}

void MyLog::info(const char *msg)
{
	_catRef.info(msg);
}

void MyLog::debug(const char *msg)
{
	_catRef.debug(msg);
}

void MyLog::warn(const char *msg)
{
	_catRef.warn(msg);
}

}// end of namespace logsystem

int main(void)
{
	logsystem::MyLog &log = logsystem::MyLog::getInstance();

	log.info("this is system info");
	log.error("something error happened");
	
	log.destory();

	return 0;
	
}


	
	
