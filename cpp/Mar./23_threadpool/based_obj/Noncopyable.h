#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

//接口继承
//实现继承
namespace wd{

class Noncopyable{ //没有必要生成对象
protected:
    Noncopyable(){}
private:
    Noncopyable(const Noncopyable &rhs);
    Noncopyable &operator=(const Noncopyable &rhs);
};

}//end of namespace 

#endif
