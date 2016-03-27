#ifndef __STRING_HH__
#define __STRING_HH__

#include <iostream>

class String
{
public:
	String();
	String(const char *);
	String(const String&);
	~String();
	String &operator=(const String &);
	String &operator=(const char *);

	String &operator+=(const String &);
	String &operator+=(const char *);

	char &operator[](std::size_t index);
    const char &operator[](std::size_t index) const;

    std::size_t size() const;
    const char* c_str() const;

    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);
    friend bool operator<=(const String &, const String &);
    friend bool operator>=(const String &, const String &);
    friend bool operator>(const String &, const String &);
    friend bool operator<(const String &, const String &);
    
    friend std::ostream &operator<<(std::ostream &os, const String &s);
    friend std::istream &operator>>(std::istream &is, String &s);
private:
    char *_pStr;
};

String operator+(const String &, const String &);
String operator+(const char *, const String &);
String operator+(const String & ,const char *);
#endif
