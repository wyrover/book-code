#ifndef OLESS_H
#define OLESS_H

#include <list>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include "pole.h"
#include "common.hpp"

namespace OleStructuredStorage {

	class Oless {
	private:
		char* m_file;
		std::vector<IExportable*> m_results;      

		void printStreamInfo(POLE::Storage*, std::string, std::string);
		void guessStreamInfo(POLE::Storage*, std::string, std::string);
	  void recurse(POLE::Storage*, const std::string, void(Oless::*)(POLE::Storage*, std::string, std::string));

	public:
		Oless() {};
		Oless(char*);
		std::vector<ExtensionInfo*> Guess();
		std::vector<OleSummary*> List();
		void Dump(char*, std::string);
		bool IsOless();

	};
}
#endif