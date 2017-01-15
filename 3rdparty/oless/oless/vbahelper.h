#ifndef VBAHELPER_H
#define VBAHELPER_H

#include <vector>
#include <tuple>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "pole.h"
#include "common.hpp"

struct MODULEOFFSET {
	unsigned short Id;
	unsigned int Size;
	unsigned int TextOffset;
};
struct VBAProjectStreamHeader {
	unsigned short Reserved1;
	unsigned short Version;
	unsigned short Reserved2;
	unsigned short Reserved3;
};
struct ProjectSysKind{
	unsigned short Id;
	unsigned int Size;
	unsigned int SysKind;
};
struct ProjectLCID {
	unsigned short Id;
	unsigned int Size;
	unsigned int Lcid;
};
struct ProjectLCIDInvoke{
	unsigned short Id;
	unsigned int Size;
	unsigned int LcidInvoke;
};
struct ProjectCodePage{
	unsigned short Id;
	unsigned int Size;
	unsigned short CodePage;
};
struct ProjectName {
	unsigned short Id;
	unsigned int SizeOfProjectName;
	std::string Name;
};
struct ProjectDocString{
	unsigned short Id;
	unsigned int SizeOfDocString;
	std::string DocString;
	unsigned short Reserved;
	unsigned int SizeOfDocStringUnicode;
	std::string DocStringUnicode;
};
struct ProjectHelpFilePath{
	unsigned short Id;
	unsigned int SizeOfHelpFile1;
	std::string HelpFile1;
	unsigned short Reserved;
	unsigned int SizeOfHelpFile2;
	std::string HelpFile2;
};
struct ProjectHelpContext{
	unsigned short Id;
	unsigned int Size;
	unsigned int HelpContext;
};
struct ProjectLibFlags{
	unsigned short Id;
	unsigned int Size;
	unsigned int Flags;
};
struct ProjectVersion{
	unsigned short Id;
	unsigned int Size;
	unsigned int VersionMajor;
	unsigned short VersionMinor;
};
struct ProjectConstants{
	unsigned short Id;
	unsigned int SizeOfConstants;
	std::string Constants;
	unsigned short Reserved;
	unsigned int SizeOfConstantsUnicode;
	std::string ConstantsUnicode;
};


struct ProjectInformation {
	ProjectSysKind SysKindRecord;
	ProjectLCID LcidRecord;
	ProjectLCIDInvoke LcidInvokeRecord;
	ProjectCodePage CodePageRecord;
	ProjectName NameRecord;
	ProjectDocString DocStringRecord;
	ProjectHelpFilePath HelpFilePathRecord;
	ProjectHelpContext HelpContextRecord;
	ProjectLibFlags LibFlagsRecord;
	ProjectVersion VersionRecord;
	ProjectConstants ConstantsRecord;
};

struct ProjectModule {
	std::string Name;
	std::string StreamName;
	unsigned int Offset;
	std::string Code;
	bool isPrivate;
	bool isReadOnly;
};


class VbaExtensionInfo : public ExtensionInfo {
public:
	std::string ProjectName;
	std::vector<std::string> References;
	std::vector<ProjectModule*> Modules;
	unsigned int VBAVersionMajor;
	unsigned short VBAVersionMinor;
	VbaExtensionInfo() {
		this->Extension = ".vba";
		this->VersionName = "Visual Basic for Applications";
	};
	virtual std::string ToJson() {
		std::ostringstream str;
		str << "{";
		str << "\"extension\" : \"" << this->Extension << "\"";
		str << ", \"name\" : \"" << this->VersionName << "\"";
		str << ", \"ProjectName\" : \"" << this->ProjectName << "\"";
		str << ", \"ProjectVersion\" : " << this->Version;
		str << ", \"VBAVersionMajor\" : " << this->VBAVersionMajor;
		str << ", \"VBAVersionMinor\" : " << this->VBAVersionMinor;

		str << ", \"References\" : [" << helper::vector_join(this->References, ",", true) << "]";

		str << ", \"Modules\" : [";
		for (std::vector<ProjectModule*>::const_iterator i = this->Modules.begin(); i != this->Modules.end(); i++){
			if (i != this->Modules.begin()) str << ",";
			str << "{";
			str << "\"Name\":\"" << (*i)->Name << "\"";
			str << ",\"StreamName\":\"" << (*i)->StreamName<< "\"";
			str << ",\"Offset\":" << (*i)->Offset;
			str << ",\"isPrivate\":" << (*i)->isPrivate;
			str << ",\"isReadOnly\":" << (*i)->isReadOnly;
			std::string code = (*i)->Code;
			str << ",\"Code\":\"" << helper::JsonEscape((*i)->Code) << "\"";
			str << "}";						
		}
		str << "]";
		
		str << "}";
		return str.str();
	}
	virtual std::string ToXml() {
		std::ostringstream str;
		str << "<item>";
		str << "<extension>" << this->Extension << "</extension>";
		str << "<name>" << this->VersionName << "</name>";
		str << "<ProjectName>" << this->ProjectName << "</ProjectName>";
		str << "<ProjectVersion>" << this->Version << "</ProjectVersion>";
		str << "<VBAVersionMajor>" << this->VBAVersionMajor << "</VBAVersionMajor>";
		str << "<VBAVersionMinor>" << this->VBAVersionMinor << "</VBAVersionMinor>";

		str << "<References>";
		for (std::vector<std::string>::const_iterator i = this->References.begin(); i != this->References.end(); i++) {
			str << "<Reference>" << *i << "</Reference>";
		}
		str << "</References>";
		

		str << "<Modules>";
		for (std::vector<ProjectModule*>::const_iterator i = this->Modules.begin(); i != this->Modules.end(); i++){
			if (i != this->Modules.begin()) str << ",";
			str << "<Module>";
			str << "<Name>" << (*i)->Name << "</Name>";
			str << "<StreamName>" << (*i)->StreamName << "</StreamName>";
			str << "<Offset>" << (*i)->Offset << "</Offset>";
			str << "<isPrivate>" << (*i)->isPrivate << "</isPrivate>";
			str << "<isReadOnly>" << (*i)->isReadOnly << "</isReadOnly>";
			str << "<Code><![CDATA[" << (*i)->Code << "]]></Code>";
			str << "</Module>";
		}
		str << "</Modules>";
		str << "</item>"; 
		return str.str();
	}
	virtual std::string ToText() {
		std::ostringstream str;
		str << this->Extension << "\t" << this->Version << "\t" << this->VersionName;
		str << "\t" << this->ProjectName;
		str << "\t" << this->Version;
		str << "\t" << this->VBAVersionMajor;
		str << "\t" << this->VBAVersionMinor;
		return str.str();
	}
	virtual std::string ToCsv() {
		std::ostringstream str;
		str << this->Extension << "," << this->Version << "," << this->VersionName;
		str << "," << this->ProjectName;
		str << "," << this->Version;
		str << "," << this->VBAVersionMajor;
		str << "," << this->VBAVersionMinor;
		return str.str();
	}

};

class vbahelper
{
private:
	friend void compressionTest(std::string, POLE::Storage*);
public:
	vbahelper();
	~vbahelper();

	VbaExtensionInfo* Analyze(std::string, POLE::Storage*);

	static std::tuple<unsigned int, unsigned char*> Decompress(const unsigned int, const unsigned char*);
};

#endif