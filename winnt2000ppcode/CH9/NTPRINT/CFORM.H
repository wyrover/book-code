
//  CFORM.H     Windows NT, MS Visual C/C++ (32-bit) 1.1
#include <windows.h>
#define BUF_SIZE 10240

class CPrinterForm {
public:
   CPrinterForm(void);
   CPrinterForm(LPSTR lpName);
   virtual ~CPrinterForm(void);
   BOOL Enumerate(DWORD iIndex, LPSTR lpName);
   BOOL Add(LPSTR, long, long, long, long, long, long);
   BOOL Delete(LPSTR);
   BOOL Get(LPSTR, long*, long*, long*, long*, long*, long*); 
   BOOL Set(LPSTR, long, long, long, long, long, long);
   DWORD dwForms;
private:       
      // prevent copy by declaring without defining
   CPrinterForm(const CPrinterForm&);
   const CPrinterForm& operator=(const CPrinterForm&);
   void CPrinterForm::FillForm(FORM_INFO_1 *, LPSTR, long, 
      long, long, long, long, long);
   HANDLE hPrinter;
   BYTE lpData[BUF_SIZE];
   FORM_INFO_1 *pFI;
   DWORD dwSize;
};  // CPrinterForm

/* end of file */
