/*======================================================================
Copyright (c) 2010,ｚｙｑ５９４５@126.com
All rights reserved.

文件名称：MyThreadPool.h
摘    要：任务类和模板实例化线程池类的定义

当前版本：1.00
作    者：ｚｙｑ５９４５@126.com（注意把中文字符换成英文字符）
完成日期：2010年6月26日
发布Blog：http://blog.csdn.net/zyq5945/
======================================================================*/

#pragma once
#include "msg_thread_pool.h"

/*======================================================================
类名称：    CTask
类说明：    自定义的测试任务类
            如果类中有指针或者其他的类的话(深拷贝和浅拷贝),
            没有把握最好是不要用编译器提供的默认拷贝构造函数和等号操作符
======================================================================*/
class CTask
{
public:
    CTask();
    CTask(const CTask& rTask);
    CTask& operator =(const CTask& rTask);

public:
    int m_nIndex;
    SYSTEMTIME m_cTime;

};

/*======================================================================
类名称：    class CMyThreadPool : public CMsgThreadPool<CTask>
类说明：    具体是线程池类
            需要在这个类的虚函数DoWork中实现要做的具体工作
            也可以如此继承
            typedef CMsgThreadPool<CTask> CCTaskThreadPool;
            class CMyThreadPool : public CCTaskThreadPool
======================================================================*/
class CMyThreadPool : public CMsgThreadPool<CTask>
{
public:
    CMyThreadPool(void);
    ~CMyThreadPool(void);

protected:
    virtual void DoWork(CTask& rTask);
};
