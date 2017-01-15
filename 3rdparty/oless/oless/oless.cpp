#include <iostream>
#include <fstream>
#include <unordered_map>
#include "oless.h"
#include "pole.h"
#include "olessoffice.h"
#include "vbahelper.h"

namespace OleStructuredStorage {
	POLE::Storage* OpenFile(char* file) {
		POLE::Storage *storage = new POLE::Storage(file);
		storage->open();
		if (storage->result() != POLE::Storage::Ok)
		{
			storage->close();
			std::cerr << "Unable to open OLESS file" << std::endl;
			delete storage;
		}
		return storage;
	}

	void Oless::recurse(POLE::Storage* storage, const std::string path, void(Oless::*pCallback)(POLE::Storage*, std::string, std::string))
	{
		std::list<std::string> entries;
		entries = storage->entries(path);

		std::list<std::string>::iterator it;
		for (it = entries.begin(); it != entries.end(); ++it)
		{
			std::string name = *it;
			std::string fullname = path + name;

			(this->*pCallback)(storage, name, fullname);
			if (storage->isDirectory(fullname))
			{
				this->recurse(storage, fullname + "/", pCallback);
			}
		}
	}

	void Oless::printStreamInfo(POLE::Storage* storage, std::string name, std::string fullname) {
		if (!storage->isDirectory(fullname)) {
			POLE::Stream *stream = new POLE::Stream(storage, fullname.c_str());
			if (!stream->fail()) {
				OleSummary *summary = new OleSummary();
				summary->FullName = stream->fullName();
				summary->Size = stream->size();
				this->m_results.push_back((IExportable*)summary);
			}
		}
	}

	template<class T>
	std::vector<T*> convert(std::vector<IExportable*> vector) {
		std::vector<T*> ans;
		std::vector<IExportable*>::iterator it;
		for (it = vector.begin(); it != vector.end(); ++it){
			ans.push_back((T*)(*it));
		}
		return ans;
	}



	std::vector<OleSummary*> Oless::List() {
		this->m_results.clear();
		POLE::Storage *storage = new POLE::Storage(this->m_file);
		storage->open();

		if (storage->result() != POLE::Storage::Ok)
		{
			std::cerr << "Unable to open OLESS file" << std::endl;
		}
		else {
			this->recurse(storage, "/", &Oless::printStreamInfo);
		}

		storage->close();
		delete storage;

		return convert<OleSummary>(this->m_results);
	}

	void Oless::guessStreamInfo(POLE::Storage* storage, std::string name, std::string fullname) {
		olessoffice *oo = new olessoffice();
		ExtensionInfo *ei = new ExtensionInfo();
		
		if (storage->isDirectory(fullname)) {
			//Storage Analysis
			if (name == "Macros" || name == "_VBA_PROJECT_CUR" ) {
				vbahelper* vba = new vbahelper(); 
				delete ei;
				ei = vba->Analyze(fullname, storage);				
			}
		}
		else {
			//Stream Analysis
			POLE::Stream *stream = new POLE::Stream(storage, fullname);
			if (fullname == "/\011DRMContent") {
				ei->Extension = ".irm";
				ei->VersionName = "Information Rights Managed file (drm)";
			}
			else if (fullname == "/PowerPoint Document") {
				ei->Extension = ".ppt";
				ei->Version = oo->GetPptVersion(storage, stream);
				ei->VersionName = oo->GetVersion_Powerpoint(ei->Version);
			}
			else if (fullname == "/PP40") {
				ei->Extension = ".ppt";
				ei->Version = 4;
				ei->VersionName = "PowerPoint 4.0";
			}
			else if (fullname == "/Workbook" || fullname == "/Book") {
				ei->Extension = ".xls";
				ei->Version = oo->GetXlsVersion(stream);
				ei->VersionName = oo->GetVersion_Excel(ei->Version);
			}
			else if (fullname == "/WordDocument") {
				ei->Extension = ".doc";
				ei->Version = oo->GetnFib(stream);
				ei->VersionName = oo->GetVersion_Word(ei->Version);
			}
			else if (fullname == "/Contents") {
				ei->Extension = ".pub";
				ei->Version = oo->GetPubVersion(stream);
				ei->VersionName = oo->GetVersion_Publisher(ei->Version);
			}
			else if (fullname == "/VisioDocument") {
				ei->Extension = ".vsd";
				ei->Version = oo->GetVisoVersion(stream);
				ei->VersionName = oo->GetVersion_Visio(ei->Version);
			}
			else if (fullname == "/Details") {
				ei->Extension = ".bup";
			}

			delete stream;
		}
		if (!ei->Extension.empty()) {
			this->m_results.push_back((IExportable*)ei);
		}
		delete oo;
	}

	std::vector<ExtensionInfo*> Oless::Guess() {
		this->m_results.clear();
		POLE::Storage *storage = OpenFile(this->m_file);
		if (storage){
			this->recurse(storage, "/", &Oless::guessStreamInfo);
			storage->close();
			delete storage;
		}
		return convert<ExtensionInfo>(this->m_results);
	}

	void Oless::Dump(char* name, std::string outFile) {
		POLE::Storage *storage = OpenFile(this->m_file);
		if (storage) {
			POLE::Stream *stream = new POLE::Stream(storage, name);
			if (stream && !stream->fail()) {
				std::ofstream file;
				file.open(outFile, std::ios::out | std::ios::binary);
				unsigned char buffer[16];
				for (;;){
					POLE::uint64 read = stream->read(buffer, sizeof(buffer));
					file.write((const char*)buffer, read);
					if (read < sizeof(buffer)) break;
				}
				file.close();
			}
			else {
				std::cerr << "Unable to open stream" << std::endl;
			}
			storage->close();
			delete storage;
		}
	}

	bool Oless::IsOless() {
		bool ans = true;
		std::ifstream file;
		file.open(this->m_file, std::ios::in | std::ios::binary);
		unsigned char mustbe[] = { 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 };
		char *buffer = new char[8];
		file.read(buffer, 8);
		for (int i = 0; i < 8; i++) {
			ans &= (((unsigned char)buffer[i]) == mustbe[i]);
		}
		return ans;
	};

	Oless::Oless(char* file) {
		m_file = file;
	}
}