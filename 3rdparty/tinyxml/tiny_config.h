#ifndef __TINY_CONFIG_H__
#define __TINY_CONFIG_H__


#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(TINYXML_DLL)
    #if defined(TINY_EXPORTS)
        #define TINY_API __declspec(dllexport)
    #else
        #define TINY_API __declspec(dllimport)
    #endif
#endif


#if !defined(TINY_API)
    #define TINY_API
#endif






#endif // __TINY_CONFIG_H__