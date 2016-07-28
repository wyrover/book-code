(lisp:clines "
static double (*optionProbFinishAboveStrike_)(double,double,double,int) = NULL;
static double (*optionProbFinishBellowStrike_)(double,double,double,int) = NULL;
static double (*optionProbFinishBetweenPrices_)(double,double,double,int,double,double) = NULL;
__declspec (dllimport) void *__stdcall LoadLibraryA(const char *);
void *__stdcall GetProcAddress(void *,const char *);
__declspec (dllimport) unsigned int __stdcall GetLastError(void);

static int libraryLoaded = 0;
static const char *libName = \"optprob.dll\";

static int loadLibrary() {
   void *lib = LoadLibraryA(libName);
   if (!lib) return 0;
   optionProbFinishAboveStrike_ = GetProcAddress(lib, \"optionProbFinishAboveStrike\");
   optionProbFinishBellowStrike_ = GetProcAddress(lib, \"optionProbFinishBellowStrike\");
   optionProbFinishBetweenPrices_ = GetProcAddress(lib, \"optionProbFinishBetweenPrices\");  
   libraryLoaded = 1;
   return 1;
}

double l_optionProbFinishAboveStrike(double a,double b,double c,int d) {
   if (!libraryLoaded && !loadLibrary()) return -1; /* error code */
   if (!optionProbFinishAboveStrike_) return -2;
   return optionProbFinishAboveStrike_(a, b, c, d);

}
double l_optionProbFinishBellowStrike(double a,double b,double c,int d) {
   if (!libraryLoaded && !loadLibrary()) return -1; /* error code */
   return optionProbFinishAboveStrike_(a, b, c, d);
}
double l_optionProbFinishBetweenPrices(double a,double b,double c,int d,double e,double f) {
   if (!libraryLoaded && !loadLibrary()) return -1; /* error code */
   return optionProbFinishBetweenPrices_(a, b, c, d, e, f);
}
")

(lisp:defentry $optionProbFinishAboveStrike  (lisp:double lisp:double lisp:double lisp:int) 
   (lisp:double "l_optionProbFinishAboveStrike"))

(lisp:defentry $optionProbFinishBellowStrike (lisp:double lisp:double lisp:double lisp:int)
   (lisp:double "l_optionProbFinishBellowStrike"))

(lisp:defentry $optionProbFinishBetweenPrices(lisp:double lisp:double lisp:double lisp:int lisp:double lisp:double)
   (lisp:double "l_optionProbFinishBetweenPrices"))

