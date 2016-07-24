//
// AFtoSTR.CPP Returns a (const) string for a given
//                         address family ID
//

#include <winsock2.h>

////////////////////////////////////////////////////////////

typedef struct tagADDRESSFAMILIES
{
	int nFamily;
	char *szDescription;
}ADDRESSFAMILIES;

////////////////////////////////////////////////////////////

ADDRESSFAMILIES AddrFamilies[] = 
{
	AF_UNSPEC,	"AF_UNSPEC",
	AF_UNIX,	"AF_UNIX",
	AF_INET,	"AF_INET",
	AF_IMPLINK,	"AF_IMPLINK",
	AF_PUP,		"AF_PUP",
	AF_CHAOS,	"AF_CHAOS",
//	AF_NS,		"AF_NS",	// Same as AF_IPX
	AF_IPX,		"AF_IPX",
	AF_ISO,		"AF_ISO",
	AF_OSI,		"AF_OSI",
	AF_ECMA,	"AF_ECMA",
	AF_DATAKIT,	"AF_DATAKIT",
	AF_CCITT,	"AF_CCITT",
	AF_SNA,		"AF_SNA",
	AF_DECnet,	"AF_DECnet",
	AF_DLI,		"AF_DLI",
	AF_LAT,		"AF_LAT",
	AF_HYLINK,	"AF_HYLINK",
	AF_APPLETALK,"AF_APPLETALK",
	AF_NETBIOS,	"AF_NETBIOS",
	AF_VOICEVIEW,"AF_VOICEVIEW",
	AF_FIREFOX,	"AF_FIREFOX",
	AF_UNKNOWN1,"AF_UNKNOWN1 Somebody is using this!",
	AF_BAN,		"AF_BAN",
	AF_ATM,		"AF_ATM",
	AF_INET6,	"AF_INET6"
   };

////////////////////////////////////////////////////////////

//
// AFtoSTR()
//
LPCTSTR AFtoSTR(int nAddressFamily)
{
	#define NUMELEMENTS sizeof(AddrFamilies) /	\
					    sizeof(AddrFamilies[0])
	int nNdx;

	for (nNdx = 0; nNdx < NUMELEMENTS; nNdx++)
	{
		if (AddrFamilies[nNdx].nFamily == nAddressFamily)
			return(AddrFamilies[nNdx].szDescription);
	}
	return("Unknown Address Family");
}

