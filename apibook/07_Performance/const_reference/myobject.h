/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   myobject.h
/// \author Martin Reddy
/// \brief  Show differences between pass by value/reference
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef MY_OBJECT_H
#define MY_OBJECT_H

#include <string>

namespace apibook
{

///
/// An object that is a data member of MyObject
///
class MySubObject
{
public:
    /// constructor
    MySubObject();
    /// destructor
    ~MySubObject();
    /// copy constructor
    MySubObject(const MySubObject &obj);
    /// assignment operator
    MySubObject &operator = (const MySubObject &obj);
};

///
/// The class that MySubObject is a data member of
///
class MyObject
{
public:
    /// constructor
    MyObject();
    /// destructor
    ~MyObject();
    /// copy constructor
    MyObject(const MyObject &obj);
    /// assignment operator
    MyObject &operator = (const MyObject &obj);

private:
    MySubObject mSubObject;
};

///
/// A class that supports pass by value and reference
///
class MyObjectHolder
{
public:
    MyObjectHolder();
    ~MyObjectHolder();

    MyObject GetObject() const;
    /// Set the object using pass-by-value
    void SetObjectByValue(MyObject obj);
    /// Set the object using pass-by-const-reference
    void SetObjectByConstReference(const MyObject &obj);

private:
    MyObject mObject;
};

}

#endif
