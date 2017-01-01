#ifndef types_h__
#define types_h__

#if defined(__GNUC__) && defined(__GNUC_MINOR__) 
#	ifndef GCC_VERSION_AT_LEAST
#		define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION>= (major * 10000 + minor * 100 + patch))
#	endif
#else
#	define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif

#if (GCC_VERSION_AT_LEAST(4,6,0) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || (defined(_MSC_VER) && _MSC_VER >= 1600)
#	define HAVE_NULLPTR 1
#else
#	define nullptr NULL
#endif


#endif // types_h__