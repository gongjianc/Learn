#include <iostream>
#include <string>
#include <sstream>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>


namespace logsystem
{
class MyLog
{
public:
	static MyLog &getInstance();
	static void destory();

	void fatal(const char *msg);
	void error(const char *msg);
	void warn(const char *msg);
	void info(const char* msg);
	void debug(const char* mst);
private:
	MyLog();
	static MyLog *_pInstance;
	log4cpp::Category &_catRef;
};

} //end of namespace logsystem
