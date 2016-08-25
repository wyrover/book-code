// myheader.h
#ifndef MYHEADER_H__
#define MYHEADER_H__

class A; // No need to include A's header

class B
{
public:
    void f(const A& a);
    // ...
private:
    A* a_;
};
#endif
