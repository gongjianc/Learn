#ifndef __MY_NONCOPYABLE_H__
#define __MY_NONCOPYABLE_H__

namespace wd{

class Noncopyable{
public:
    Noncopyable() {}
protected:
    Noncopyable(const Noncopyable &rhs);
    Noncopyable &operator=(const Noncopyable &rhs);
};

}//end of namespace


#endif
