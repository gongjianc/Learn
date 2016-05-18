/*
** @file    test_server.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 18:52:33
*/

#include "SpellcorrectServer.h"
#include <iostream>

int main(void)
{
    wd::SpellcorrectServer server("../conf/my.conf");
    server.start();
    return 0;
}
