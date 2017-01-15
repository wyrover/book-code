#ifndef MSVISIO_H
#define MSVISIO_H


static const char VISIO[] = "Visio (TM) Drawing\x0d\x0a"; //24
struct VsdHeader
{
	char key[24];
	unsigned short minor;
	unsigned short major;

};

#endif