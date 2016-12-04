#ifndef XXX_ThreadBinder_H
#define XXX_ThreadBinder_H

#ifdef WIN32

#elif defined(LINUX)

#else
#error	"Please fix os ver"
#endif

typedef void * HANDLE;

struct ThreadWrapperArg {
	void * (* StartAddress)(void *);
	void * arg;
};

class TThreadAPIBinder
{
public:
	static HANDLE MakeThread(unsigned long * ThreadID, void * (* StartAddress)(void *) , void * arg, int *error);
	static long WaitFor(unsigned long Handle);
	static void ExitFromThread(unsigned long ExitCode);
	static void Cleanup(unsigned long Handle);

};

#endif