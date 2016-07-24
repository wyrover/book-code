//
// PROTSTR.CPP - PROTOtoSTR()
//				 Returns a (const) string for a given
//               protocol type
//

#include <winsock2.h>

// Protocol specific annexes
#include <wsipx.h>
#include <ws2dnet.h>
#include <ws2atm.h>

// #include <ws2rsvp.h> -- Not provided
// RSVP protocols according to WSANX203.DOC
#define IPPROTO_RSVP 		0x2e 

// #include <ws2osi.h>	-- Not provided
// ISO protocols according to WSANX203.DOC
#define ISOPROTO_TP_CONS   25
#define ISOPROTO_TP4_CLNS  29
#define ISOPROTO_CLTP_CLNS 30
#define ISOPROTO_X25       32

////////////////////////////////////////////////////////////

typedef struct tagPROTOCOLTYPES
{
        int iProtocolType;
        char *szDescription;
}PROTOCOLTYPES;

////////////////////////////////////////////////////////////

PROTOCOLTYPES ProtocolTypes[] = 
{
	// WINSOCK2.H
	IPPROTO_IP,		"IPPROTO_IP",
	IPPROTO_ICMP,	"IPPROTO_ICMP",
	IPPROTO_IGMP,	"IPPROTO_IGMP",
	IPPROTO_GGP,	"IPPROTO_GGP",
	IPPROTO_TCP,	"IPPROTO_TCP",
	IPPROTO_PUP,	"IPPROTO_PUP",
	IPPROTO_UDP,	"IPPROTO_UDP",
	IPPROTO_IDP,	"IPPROTO_IDP",
	IPPROTO_ND,		"IPPROTO_ND",
	IPPROTO_RAW,	"IPPROTO_RAW",
	// Novell - WSIPX.H
	NSPROTO_IPX,	"NSPROTO_IPX",
	NSPROTO_SPX,	"NSPROTO_SPX",
	NSPROTO_SPXII,	"NSPROTO_SPXII",
	//DecNet - WS2DNET.H
	DNPROTO_NSP,	"DNPROTO_NSP",		// = IPPROTO_ICMP
	DNPROTO_RAW,	"DNPROTO_RAW",		// = IPPROTO_RAW 
	// OSI - WS2OSI.H
	ISOPROTO_TP_CONS,	"ISOPROTO_TP_CONS",
	ISOPROTO_TP4_CLNS,	"ISOPROTO_TP4_CLNS",
	ISOPROTO_CLTP_CLNS,	"ISOPROTO_CLTP_CLNS",
	ISOPROTO_X25,		"ISOPROTO_X25",
	// ATM - WS2ATM.H
	ATMPROTO_AAL1,	"ATMPROTO_AAL1",	// = IPPROTO_ICMP
	ATMPROTO_AAL2,	"ATMPROTO_AAL2",	// = IPPROTO_IGMP
	ATMPROTO_AAL34,	"ATMPROTO_AAL34",	// = IPPROTO_GGP
	ATMPROTO_AAL5,	"ATMPROTO_AAL5",
	// RSVP - WS2RSVP.H
	IPPROTO_RSVP,	"IPPROTO_RSVP"
};

////////////////////////////////////////////////////////////

//
// PROTOtoSTR()
//
LPCTSTR PROTOtoSTR(int iProtocol)
{
	#define NUMELEMENTS sizeof(ProtocolTypes) /  \
					    sizeof(ProtocolTypes[0])
	int nNdx;

	for (nNdx = 0; nNdx < NUMELEMENTS; nNdx++)
	{
		if (ProtocolTypes[nNdx].iProtocolType == iProtocol)
			return(ProtocolTypes[nNdx].szDescription);
	}
	return("Unknown Protocol Type");
}

