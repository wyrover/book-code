#include "olessoffice.h"

namespace OleStructuredStorage {



	unsigned short olessoffice::GetnFib(POLE::Stream* stream) {
		unsigned short nFib = 0;
		

		Fib* fib = helper::GetStructFromStream<Fib>(stream);

		nFib = fib->base.nFib;
		//if (fib->cbRgFcLcb > 0x00FF) {		//Corrupted file	}	

		stream->seek(sizeof(Fib) + (fib->cbRgFcLcb * 8));

		Fib2* fib2 = helper::GetStructFromStream<Fib2>(stream);
		if (fib2->cswNew != 0) {
			nFib = fib2->nFibNew;
		}

		delete fib;
		delete fib2;
		return nFib;
	}
	std::string olessoffice::GetVersion_Word(unsigned short nFib) {
		std::string ans;

		switch (nFib) {
		case 314: ans = "Word2005"; break;
		case 274: ans = "Word2005_Compat2003"; break;
		case 268: ans = "Word2003"; break;
		case 257: ans = "Word2002"; break;
		case 217: ans = "Word2000"; break;
		case 193: ans = "Word97"; break;
		case 104: ans = "Word95_Or_Word6"; break;
		case 105: ans = "Word95Beta"; break;
		case 101: ans = "WinWord6.0"; break;
		case 45:  ans = "WinWord2.0"; break;
		case 33:  ans = "WinWord1.0"; break;
		default:  ans = "Unknown"; break;
		}
		return ans;
	}

	unsigned short olessoffice::GetXlsVersion(POLE::Stream* stream) {
		unsigned short version = 0;

		BOF* bof = helper::GetStructFromStream<BOF>(stream);
		version = bof->rubBuild;

		delete bof;
		return version;
	}
	std::string olessoffice::GetVersion_Excel(unsigned short version) {
		std::string ans;

		switch (version) {
		case 1280: ans = "BIFF5"; break;
		case 1536: ans = "BIFF8"; break;
		default:  ans = "Unknown"; break;
		}
		return ans;
	}


	unsigned short olessoffice::GetPptVersion(POLE::Storage* storage, POLE::Stream* stream) {
		unsigned short version = 0;
		POLE::Stream *strmCurrentUser = new POLE::Stream(storage, "/Current User");

		CurrentUserAtom* cua = helper::GetStructFromStream<CurrentUserAtom>(strmCurrentUser);

		stream->seek(cua->offsetToCurrentEdit);

		UserEditAtom* uea = helper::GetStructFromStream<UserEditAtom>(stream);
		version = uea->version;

		delete uea;
		delete cua;
		delete strmCurrentUser;
		return version;
	}

	bool withinCheck(unsigned long num, unsigned long target, unsigned long range) {
		return (num >= (target - range) && num <= (target + range));
	}

	std::string olessoffice::GetVersion_Powerpoint(unsigned short version) {
		std::string ans = "Unknown";

		if (version == 0) {
			ans = "Non-Office";
		}
		else if (withinCheck(version, 4569, 10)) {
			ans = "Office 2013 SP1";
		}
		else if (withinCheck(version, 4569, 10)) {
			ans = "Office 2013 SP1";
		}
		else if (withinCheck(version, 4420, 10)) {
			ans = "Office 2013 RTM";
		}
		else if (withinCheck(version, 4128, 10)) {
			ans = "Office 2013 Preview";
		}
		else if (withinCheck(version, 7015, 10)) {
			ans = "Office 2010 SP2"; 	//14.0.7015.1000
		}
		else if (withinCheck(version, 6029, 10)) {
			ans = "Office 2010 SP1"; 	//14.0.6029.1000
		}
		else if (withinCheck(version, 4763, 10)) {
			ans = "Office 2010 RTM"; 	//14.0.4763.1000
		}
		else if (withinCheck(version, 6425, 10)) {
			ans = "Office 2007 SP2";    //12.0.6425.1000
		}
		else if (withinCheck(version, 6211, 10)) {
			ans = "Office 2007 SP1";    //12.0.6211.1000
		}
		else if (withinCheck(version, 4518, 10)) {
			ans = "Office 2007 RTM";    //12.0.6425.1014

		}
		else if (withinCheck(version, 8173, 10)) {
			ans = "Office 2003 SP3"; 	// 11.0.8169.?
		}
		else if (withinCheck(version, 7969, 10)) {
			ans = "Office 2003 SP2"; 	// 11.0.6564.6568
		}
		else if (withinCheck(version, 6361, 10)) {
			ans = "Office 2003 SP1"; 	// 11.0.6361.0
		}
		else if (withinCheck(version, 5529, 10)) {
			ans = "Office 2003 RTM"; 	// 11.0.5529.0
		}
		else if (withinCheck(version, 6501, 10)) {
			ans = "Office XP SP3"; 		// 10.0.6501.6735
		}
		else if (withinCheck(version, 4205, 10)) {
			ans = "Office XP SP2"; 		// 10.0.4205.0
		}
		else if (withinCheck(version, 3506, 10)) {
			ans = "Office XP SP1"; 		// 10.0.3506.0
		}
		else if (withinCheck(version, 2623, 10)) {
			ans = "Office XP RTM"; 		// 10.0.2623.0
		}
		else if (withinCheck(version, 6620, 10)) {
			ans = "Office 2000 SP3"; 	// 9.0.6620
		}
		else if (withinCheck(version, 4527, 10)) {
			ans = "Office 2000 SP2"; 	// 9.0.4527
		}
		else if (withinCheck(version, 3821, 10)) {
			ans = "Office 2000 SP1"; 	// 9.0.3821
		}
		else if (withinCheck(version, 2716, 10)) {
			ans = "Office 2000 RTM"; 	// 9.0.2716
		}
		else if (withinCheck(version, 3516, 10)) {
			ans = "Office 97 RTM"; 		// 8.0.3516
		}
		else if (withinCheck(version, 4208, 10)) {
			ans = "Office 97 SR1"; 		// 8.0.4208
		}
		else if (withinCheck(version, 5507, 10)) {
			ans = "Office 97 SR2"; 		// 8.0.5507
		}
		return ans;
	}

	unsigned short olessoffice::GetPubVersion(POLE::Stream* stream){
		unsigned short version = 0;

		PubHeader* header = helper::GetStructFromStream<PubHeader>(stream);
		version = header->version0;

		if (version == 300) {
			version = header->version2;
		}
		delete header;
		return version;
	}
	std::string olessoffice::GetVersion_Publisher(unsigned short version){
		std::string ans;

		switch (version) {
		case 125: ans = "Publisher 3"; break;
		case 136: ans = "Publisher 4"; break;
		case 616: ans = "Publisher 5"; break;
		case 717: ans = "Publisher 6"; break;
		case 1000: ans = "Publisher 10"; break;
		default:  ans = "Unknown"; break;
		}
		return ans;
	}

	unsigned short olessoffice::GetVisoVersion(POLE::Stream* stream){
		unsigned short version = 0;
		
		VsdHeader* header = helper::GetStructFromStream<VsdHeader>(stream);
		version = header->major;
		
		delete header;
		return version;
	}
	std::string olessoffice::GetVersion_Visio(unsigned short version) {
		std::string ans;

		switch (version) {	
		case 0xB: ans = "Visio 2003 or greater"; break;
		case 0xA: ans = "Visio 2002"; break;
		case 0x6: ans = "Visio 2000"; break;
		case 0x5: ans = "Visio 5"; break;
		case 0x4: ans = "Visio 4"; break;
		case 0x3: ans = "Visio 3"; break;
		case 0x2: ans = "Visio 2"; break;
		case 0x1: ans = "Visio 1"; break;
		default:  ans = "Unknown"; break;
		}
		return ans;
	}

	olessoffice::olessoffice()
	{
	}


	olessoffice::~olessoffice()
	{
	}
}