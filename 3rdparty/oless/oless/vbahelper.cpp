#include "vbahelper.h"
#include <math.h>
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))

vbahelper::vbahelper(){}
vbahelper::~vbahelper(){}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

unsigned int Extract_CompressedChunkSize(unsigned int header) {
	return (header & 0x0FFF) + 3;
}
unsigned char Extract_CompressedChunkFlag(unsigned int header) {
	return  (header >> 15) & 0x1; //(header & 0x8000) << 15;
}

std::tuple<unsigned int, unsigned char*> FetchStream(POLE::Storage *storage, std::string streamFullName, unsigned int offset) {
	POLE::Stream* stream = new POLE::Stream(storage, streamFullName);
	if (stream) {
		unsigned int len = (unsigned int)(stream->size() - offset);
		unsigned char* buffer = new unsigned char[len];
		stream->seek(offset);
		unsigned int actualLen = (unsigned int) stream->read(buffer, len);
		delete stream;
		return std::make_tuple(actualLen, buffer);
	}
	else {
		return std::make_tuple<unsigned int, unsigned char*>(0, NULL);
	}
}

std::tuple<unsigned int, unsigned char*> FetchAndDecompress(POLE::Storage *storage, std::string streamFullName, unsigned int offset) {
	unsigned int compressedLength;
	unsigned char* compressed;
	std::tie(compressedLength, compressed) = FetchStream(storage, streamFullName, offset);
	std::tuple<unsigned int, unsigned char*> ans;
	if (compressed) {
		ans = vbahelper::Decompress(compressedLength, compressed);
	}
	else {
		ans = std::make_tuple<unsigned int, unsigned char*>(0, NULL);
	}
	delete compressed;
	return ans;
}

std::pair<unsigned int, std::string> ReadString(unsigned char* buffer, unsigned int startIndex) {
	unsigned int len = helper::GetItem4Byte(buffer, startIndex);
	std::string  str = helper::GetItemString(buffer, startIndex + 4, len);
	return std::make_pair(startIndex + len + 4, str);
}


std::pair<unsigned int, std::wstring> ReadStringW(unsigned char* buffer, unsigned int startIndex) {
	unsigned int len = helper::GetItem4Byte(buffer, startIndex);
	std::wstring  str = helper::GetItemStringW(buffer, startIndex + 4, len);
	return std::make_pair(startIndex + len + 4, str);
}

std::string GetCode(POLE::Storage* storage, std::string fullname, unsigned int offset) {
	unsigned int len;
	unsigned char* buffer;
	std::tie(len, buffer) = FetchAndDecompress(storage, fullname, offset);
	return helper::GetItemString(buffer, 0, len);
}

VbaExtensionInfo* vbahelper::Analyze(std::string fullname, POLE::Storage* vbaStorage) {

	VbaExtensionInfo* ans = new VbaExtensionInfo();
	unsigned int len;
	unsigned char* buffer;

	//Read VBA Version
	POLE::Stream* stream = new POLE::Stream(vbaStorage, fullname + "/VBA/_VBA_PROJECT");
	VBAProjectStreamHeader* header = helper::GetStructFromStream<VBAProjectStreamHeader>(stream);
	ans->Version = header->Version;
	delete stream;


	//Read Project Information
	std::tie(len, buffer) = FetchAndDecompress(vbaStorage, fullname + "/VBA/dir", 0);
	unsigned int index = 40;

	std::string tmpStr;
	std::wstring tmpwStr;
	std::tie(index, tmpStr) = ReadString(buffer, index); //Project Name
	ans->ProjectName = tmpStr;
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //DocString
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //DocStringUnicode
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //ProjectHelpFilePath1
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //ProjectHelpFilePath2
	index += 10; //ProjectHelpContext
	index += 10; //ProjectLibFlags
	ProjectVersion* pVersion = (ProjectVersion*)&buffer[index];
	ans->VBAVersionMajor = pVersion->VersionMajor;
	ans->VBAVersionMinor = pVersion->VersionMinor;
	index += 12; //ProjectVersion
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //ProjectConstants
	std::tie(index, tmpStr) = ReadString(buffer, index + 2); //ProjectConstants Unicode

	//Read References
	while (helper::GetItem2Byte(buffer, index) == 0x16) {
		std::tie(index, tmpStr) = ReadString(buffer, index + 2); //Reference Name
		ans->References.push_back(tmpStr);
		std::tie(index, tmpStr) = ReadString(buffer, index + 2); //Reference Unicode Name

		unsigned short recordType = helper::GetItem2Byte(buffer, index);
		index += 2;
		switch (recordType) {
		case 0x33://REFRENCECONTROL		
			std::tie(index, tmpStr) = ReadString(buffer, index); //LibTwiddled
			break;
		case 0x2f://REFRENCECONTROL		
			index += 4;
			std::tie(index, tmpStr) = ReadString(buffer, index); //LibTwiddled
			index += 6;
			std::tie(index, tmpStr) = ReadString(buffer, index + 2); //Reference Name
			std::tie(index, tmpStr) = ReadString(buffer, index + 2); //Reference Unicode Name
			index += 6;
			std::tie(index, tmpStr) = ReadString(buffer, index + 2); //LibExtended
			index += 26;
			break;
		case 0x0d: //REFERENCEREGISTERED
			index += 4;
			std::tie(index, tmpStr) = ReadString(buffer, index); //Libid
			index += 6;
			break;
		case 0x0e: //REFERENCEPROJECT
			index += 4;
			std::tie(index, tmpStr) = ReadString(buffer, index); //LibidAbsolute
			std::tie(index, tmpStr) = ReadString(buffer, index); //LibidRelative
			index += 6;
			break;
		default:
			break;
		}
	}

	//Read Modules	
	index += 16;
	while (index < len) {

		ProjectModule* mod = new ProjectModule();		
		mod->Offset = 0;
		mod->isPrivate = false;
		mod->isReadOnly = false;
		
		//Read Records
		unsigned short id = helper::GetItem2Byte(buffer, index);

		while(id != 0x2B && index < len) {
			id = helper::GetItem2Byte(buffer, index);
			index += 2;
			switch (id) {
			case 0x19: //ModuleName
				std::tie(index, tmpStr) = ReadString(buffer, index);
				mod->Name = tmpStr;
				break;
			case 0x47: //ModuleNameUnicode
				std::tie(index, tmpStr) = ReadString(buffer, index);
				break;
			case 0x1A: //ModuleStreamName
				std::tie(index, tmpStr) = ReadString(buffer, index);
				mod->StreamName = tmpStr;
				index += 2;
				std::tie(index, tmpStr) = ReadString(buffer, index);
				break;
			case 0x1C: //ModuleDocString
				std::tie(index, tmpStr) = ReadString(buffer, index);
				index += 2;
				std::tie(index, tmpStr) = ReadString(buffer, index);
				break;
			case 0x31: //ModuleOffset
				index += 4;
				mod->Offset = helper::GetItem4Byte(buffer, index);
				index += 4;
				break;
			case 0x1E: //ModuleHelpContext
				index += 8;
				break;
			case 0x2C: //ModuleCookie
				index += 6;
				break;
			case 0x21: //ModuleType = procedural
			case 0x22: //ModuleType = document, class, or designer
				index += 4;
				break;
			case 0x25: //ModuleReadOnly
				mod->isReadOnly = true;
				index += 4;
				break;
			case 0x28: //ModulePrivate
				mod->isPrivate = true;
				index += 4;
				break;
			}
		}
		index += 4;
		if (!(mod->StreamName.empty())) {
			std::string newStreamName = fullname + "/VBA/" + (std::string)(mod->StreamName);
			mod->Code = GetCode(vbaStorage, newStreamName, (unsigned int)(mod->Offset));
		}
		ans->Modules.push_back(mod);
	}
	return ans;
}

std::tuple<unsigned int, unsigned char*> vbahelper::Decompress(const unsigned int compressedBufferLength, const unsigned char* compressedBuffer) {

	unsigned int decompressedChunkStart = 0;
	unsigned int decompressedCurrent = 0;
	unsigned int compressedRecordEnd = compressedBufferLength;
	unsigned int compressedCurrent = 0;
	unsigned int compressedChunkStart = 0;
	unsigned int compressedEnd = 0;

	unsigned char* decompressedBuffer = new unsigned char[compressedBufferLength * 1024];

	if (compressedBuffer[compressedCurrent] != 0x01) {
		return std::make_tuple<unsigned int, unsigned char*>(0, NULL);
	}
	compressedCurrent++;

	while (compressedCurrent < compressedRecordEnd) {
		compressedChunkStart = compressedCurrent;

		//2.4.1.3.2		
		unsigned short headerblock = helper::GetItem2Byte(compressedBuffer, compressedCurrent);
		unsigned int size = Extract_CompressedChunkSize(headerblock);
		unsigned int sig = (headerblock >> 12) & 0x07;
		if (sig != 3) {
			return std::make_tuple<unsigned int, unsigned char*>(0, NULL);
		}
		unsigned char compressedFlag = Extract_CompressedChunkFlag(headerblock);
		decompressedChunkStart = decompressedCurrent;
		compressedEnd = __min(compressedRecordEnd, compressedChunkStart + size);
		compressedCurrent = compressedChunkStart + 2;
		if (compressedFlag == 1) {
			while (compressedCurrent < compressedEnd) {

				//2.4.1.3.4 - Decompress Token Sequence
				unsigned short byte = compressedBuffer[compressedCurrent];
				compressedCurrent++;
				if (compressedCurrent < compressedEnd) {
					for (unsigned int i = 0; i <= 7; i++) {
						if (compressedCurrent < compressedEnd) {
							//2.4.1.3.5 - Decompress a Token
							unsigned char flag = (byte >> i) & 1;
							if (flag == 0) {
								decompressedBuffer[decompressedCurrent] = compressedBuffer[compressedCurrent];
								decompressedCurrent++;
								compressedCurrent++;
							}
							else {
								//2.4.1.3.19.2 - Unpack CopyToken

								//2.4.1.3.19.1 - CopyToken Help								
								unsigned short bitCount = (unsigned short)__max(ceil(log2(decompressedCurrent - decompressedChunkStart)), (double)4);
								unsigned short lengthMask = 0xFFFF >> bitCount;
								unsigned short offsetMask = ~lengthMask;
								unsigned short maximumLength = (0xFFFF >> bitCount) + 3;

								unsigned short token = helper::GetItem2Byte(compressedBuffer, compressedCurrent);

								unsigned short copyTokenLength = (token & lengthMask) + 3;
								unsigned short copyTokenOffset = ((token & offsetMask) >> (16 - bitCount)) + 1;

								unsigned int copySource = decompressedCurrent - copyTokenOffset;

								for (unsigned int n = copySource; n < (copySource + copyTokenLength); n++) {
									decompressedBuffer[decompressedCurrent] = decompressedBuffer[n];
									decompressedCurrent++;
								}
								compressedCurrent = compressedCurrent + 2;
							}
						}
					}
				}

			}
		}
		else {
			//decompress Raw Chunk
			for (unsigned int i = 0; i < compressedEnd; i++) {
				decompressedBuffer[decompressedCurrent] = compressedBuffer[compressedCurrent];
				decompressedCurrent++;
				compressedCurrent++;
			}
		}
	}

	return std::make_tuple(decompressedCurrent, decompressedBuffer);

}
