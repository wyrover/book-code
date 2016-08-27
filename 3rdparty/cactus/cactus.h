#ifndef __CACTUS_H__
#define __CACTUS_H__

#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(CACTUS_DLL)
    #if defined(CACTUS_EXPORTS)
        #define CACTUS_API __declspec(dllexport)
    #else
        #define CACTUS_API __declspec(dllimport)
    #endif
#endif


#if !defined(CACTUS_API)
    #define CACTUS_API
#endif

#endif // __CACTUS_H__
