#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "pole.h"
#include "oless.h"

void ShowHelp() {
   std::cout << "oless {cmd} [CommandSpecificParameters]" << std::endl;
   std::cout << "Available Commands:" << std::endl;
   std::cout << "\tlist {file} [outputformat] - Enumerates the streams and storage in the OLESS file, default output format is text" << std::endl;
   std::cout << "\tguess {file} [outputformat] - Guess the extension of the file based on the OLESS contents, default output format is text" << std::endl;
   std::cout << "\tdump {file} {streamname} {outfile} - Extracts the given stream/storage name to an external file" << std::endl;
   std::cout << "Available Output Formats:" << std::endl;
   std::cout << "\txml,csv,json,txt" << std::endl;
}


OutputFormat ParseOutputFormat(std::string input) {
   std::unordered_map<std::string, OutputFormat> map = {
      { "xml", OutputFormat::XML },
      { "csv", OutputFormat::CSV },
      { "txt", OutputFormat::TEXT },
      { "json",OutputFormat::JSON }
   };
   std::unordered_map<std::string, OutputFormat>::const_iterator got = map.find(input);
   if (got == map.end()) {
      return OutputFormat::TEXT;
   }
   else {
      return got->second;
   }
}

template<class T>
void Output(OutputFormat format, std::string file, std::string headerName, std::vector<T*> summary) {
   typename std::vector<T*>::iterator it;
   switch (format) {
   case OutputFormat::TEXT:
      std::cout << file << std::endl;
      for (it = summary.begin(); it != summary.end(); it++){
         std::cout << "\t" << (*it)->ToText() << std::endl;
      }
      break;
   case OutputFormat::XML:
      std::cout << "<file>" << std::endl;
      std::cout << "\t<name>" << file << "</name>" << std::endl;
      std::cout << "\t<" << headerName << ">" << std::endl;
      for (it = summary.begin(); it != summary.end(); it++){
         std::cout << "\t\t" << (*it)->ToXml() << std::endl;
      }
      std::cout << "\t</"<< headerName << ">" << std::endl;
      std::cout << "</file>" << std::endl;
      break;
   case OutputFormat::JSON:
      std::cout << "{ \"name\" : \"" << helper::JsonEscape(file) << "\"," << std::endl;
      std::cout << "\t \""<< headerName << "\": [" << std::endl;
      for (it = summary.begin(); it != summary.end(); it++){
         if (it != summary.begin()) {
            std::cout << "," << std::endl;
         }
         std::cout << "\t\t" << (*it)->ToJson() << std::endl;
      }
      std::cout << "\n\t]\n}" << std::endl;
      break;
      break;
   case OutputFormat::CSV:
      for (it = summary.begin(); it != summary.end(); it++){
         std::cout << (*it)->ToCsv() << std::endl;
      }
      break;
   default:
      std::cout << "Unknown Output Format!" << std::endl;
   }
}

int main(int argc, char *argv[]) {

   if (argc > 2) {
      std::string command = argv[1];
      std::string file = argv[2];

      OutputFormat outFormat = OutputFormat::TEXT;
      if (argc > 3) { outFormat = ParseOutputFormat(argv[3]); }

      OleStructuredStorage::Oless *oleItem = new OleStructuredStorage::Oless(argv[2]);
      if (oleItem->IsOless()) {

         if (command == "list") {
            std::vector<OleSummary*> summary = oleItem->List();
            Output<OleSummary>(outFormat, file, "summary", summary);
         }
         else if (command == "guess") {
            std::vector<ExtensionInfo*> extensions = oleItem->Guess();
            Output<ExtensionInfo>(outFormat, file, "extensions", extensions);
         }
         else if (command == "dump") {
            if (argc > 4) {
               std::string outFile = argv[4];
               oleItem->Dump(argv[3], outFile);
            }
            else {
               ShowHelp();
            }
         }
         else {
            ShowHelp();
         }
      }
      else {
         std::cerr << file << ": Invalid OLESS header" << std::endl;
      }

   }
   else {
      ShowHelp();
   }

   return 0;
}
