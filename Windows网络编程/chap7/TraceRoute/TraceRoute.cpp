// TraceRoute.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TraceRoute1.h"



int main(int argc, char* argv[])
{   
	CTraceRoute route;
	if (argc < 2) 
        printf("input:%s host-name [max-hops]\n", argv[0]);
			
    if (argc == 3)
        route.m_nMaxhops = atoi(argv[2]);
    else
        route.m_nMaxhops = MAX_HOPS;
	route.ConnectToHost(argv[1]);
	printf("Trace route end!\n");
	route.Cleanup();
	getchar();
	return 0;
}
