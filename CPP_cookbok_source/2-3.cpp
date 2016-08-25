// global.h
#ifndef GLOBAL_H__ // See Recipe 2.0
    #define GLOBAL_H__
    #include <string>

    extern int x;
    extern std::string s;

#endif

// global.cpp
#include <string>
int x = 7;
std::string s = "Kangaroo";

// main.cpp
#include <iostream>
#include "global.h"

using namespace std;

int main()
{
    cout << "x = " << x << endl;
    cout << "s = " << s << endl;
}
