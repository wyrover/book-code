#ifndef OLESSOFFICE_H
#define OLESSOFFICE_H

#include "pole.h"
#include "MSWord.h"
#include "MSPowerPoint.h"
#include "MSExcel.h"
#include "MSVisio.h"
#include "MSPublisher.h"
#include "common.hpp"

namespace OleStructuredStorage {
   class olessoffice
   {
   private:

   public:

      //Word (.doc) specific
      unsigned short GetnFib(POLE::Stream*);
      std::string GetVersion_Word(unsigned short);

      //Excel (.xls) specific
      unsigned short GetXlsVersion(POLE::Stream*);
      std::string GetVersion_Excel(unsigned short);

      //Powerpoint (.ppt) specific
      unsigned short GetPptVersion(POLE::Storage*, POLE::Stream*);
      std::string GetVersion_Powerpoint(unsigned short);

      //Publisher (.pub) specific
      unsigned short GetPubVersion(POLE::Stream*);
      std::string GetVersion_Publisher(unsigned short);

      //Visio (.vsd) specific
      unsigned short GetVisoVersion(POLE::Stream*);
      std::string GetVersion_Visio(unsigned short);

      olessoffice();
      ~olessoffice();
   };
}
#endif