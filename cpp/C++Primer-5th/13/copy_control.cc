/*
** @file    copy_control.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-14 06:25:23
*/
 
#include <iostream>
#include <string>
#include <set>
 
using std::cout;
using std::endl;

class Folder;
class Message;

class Folder{
    friend class Message;
public:

    void addMsg(const Message *);
    void remMsg(const Message *);
private:
    std::set<Message *> _pMessages;
};

class Message{
    friend class Folder;
public:
    explicit Message(const std::string &str = " ")
    : _contents(str)
    {}
    Message(const Message &);
    Message &operator=(const Message &);
    ~Message();
    void save(Folder &);
    void remove(Folder &);
private:
    std::string _contents;
    std::set<Folder *> _pFolders;
    //
    void add_to_Folders(const Message &);
    void remove_from_Folders();
};

void Message::save(Folder &f)
{
    _pFolders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f)
{
    _pFolders.erase(&f);
    f.remMsg(this);
}

void Message::add_to_Folders(const Message &m)
{
    for(auto f : m._pFolders)
        f->addMsg(this);
}

Message::Message(const Message &m)
: _contents(m._contents)
, _pFolders(m._pFolders)
{
    add_to_Folders(m);
}

void Message::remove_from_Folders()
{
    for(auto f : _pFolders)
        f->remMsg(this)
    _pFolders.clear();
}

Message::~Message()
{
    remove_from_Folders();
}

Message &Message::operator=(const Message &rhs)
{
    remove_from_Folders();
    _contents = rhs._contents;
    _pFolders = rhs._pFolders;
    add_to_Folders(rhs);
    return *this;
}

void Folder::addMsg(const Message *pm)
{
    _pMessages.insert(*pm);
}

