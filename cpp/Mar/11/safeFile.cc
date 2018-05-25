/*
** @file    safeFile.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 23:01:13
*/
 
#include <iostream>
#include <stdio.h>
#include <stdexcept>
 
using std::cout;
using std::endl;
using std::runtime_error;

class SafeFile
{
public:
    SafeFile(const char *filename)
    : _fileHandler(fopen(filename, "w+"))
    {
        if(NULL == _fileHandler)
            throw runtime_error("open error!");
    }

    ~SafeFile()
    {
        cout << "fclose(_fileHandler)" << endl;
        fclose(_fileHandler);
    }

    void write(const char *str)
    {
        if(fputs(str, _fileHandler) == EOF)
            throw runtime_error("write error!");
    }

    void write(const char *buffer, size_t num)
    {
        if(num != 0 && fwrite(buffer, num, 1, _fileHandler) == 0)
            throw runtime_error("write1 error!");
    }
private:
    SafeFile(const SafeFile &);
    SafeFile &operator=(const SafeFile &);
private:
    FILE *_fileHandler;
};

int main(void)
{
    SafeFile filehandler("footest.dat");
    filehandler.write("Hello, RAII");
    return 0;
}
