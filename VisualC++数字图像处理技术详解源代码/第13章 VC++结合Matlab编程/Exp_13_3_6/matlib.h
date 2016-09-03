
#ifndef matlibH
#define matlibH

/*
 *    MATLIB header file
 *
 *    Copyright (c) MathTools Ltd. 1994-1999. All rights reserved.
 */

#define MATCOM_VERSION 4501
#define MATCOM_BUILD   1027

#if defined(__WINDOWS_386__) || defined(WIN32) || defined(_WIN32) || defined(__NT__)||defined(_MSC_VER)
#ifndef __WIN32__
#define __WIN32__ 1
#endif
#endif

#if !defined(_Windows) && (defined(_WINDOWS) || defined(__WIN32__))
#define _Windows 1
#endif

#if !defined(BIT16) && (defined(__BORLANDC__) && !defined(__WIN32__))
#define BIT16 1
#define HELPER
#else
#define HELPER template
#endif

#if !defined(__OS2__) && defined(__TOS_OS2__)
#define __OS2__ 1
#endif

#if defined (_MSC_VER) || defined(__MINGW32__)
  #if defined (MLIBDLL)
    #define DLLI __declspec(dllexport)
  #else
    #define DLLI __declspec(dllimport)
  #endif
  #define DLLX __declspec(dllexport)
  #define DLLW
#elif defined (__BORLANDC__) && defined(__WIN32__)
  #if defined (MLIBDLL)
    #define DLLI __export
  #else
    #define DLLI __import
  #endif
  #define DLLX __export
  #define DLLW
#elif defined (__WATCOMC__) && defined(__WIN32__)
  #if defined (MLIBDLL)
    #define DLLI __export
    #define DLLW
  #else
    #define DLLI
    #define DLLW __declspec(dllimport)
  #endif
  #define DLLX __export
#else
  #define DLLI
  #define DLLX
  #define DLLW
#endif

#ifdef _MSC_VER
  #define PPDLLI DLLI **
  #define PDLLI  DLLI *
  #define RDLLI  DLLI &
  #define PDLLX  DLLX *
  #define RDLLX  DLLX &
#else
  #define PPDLLI ** DLLI
  #define PDLLI  *  DLLI
  #define RDLLI  &  DLLI
  #define PDLLX  *  DLLX
  #define RDLLX  &  DLLX
#endif


#ifdef __BORLANDC__
#define __MFC_COMPAT__
#if __BORLANDC__<=0x520
#define __MINMAX_DEFINED
#define _WINNETWK_
#endif
#endif

#ifdef _MSC_VER
#if _MSC_VER<1100
#define NO_BUILT_IN_SUPPORT_FOR_BOOL
#endif
#endif

#ifdef __WATCOMC__
#if __WATCOMC__<1100
#define NO_BUILT_IN_SUPPORT_FOR_BOOL
#endif
#pragma warning 379 9
#endif

typedef unsigned char		uint8;
typedef float			single;

#define max_ndims 4
#define mt_stack_max_depth 513
#define m_type double

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef TMP_MAX
#undef abs
#undef acos
#undef and
#undef asin
#undef atan
#undef atan2
#undef bitand
#undef bitor
#undef clear
#undef cos
#undef exp
#undef fabs
#undef feof
#undef ferror
#undef finite
#undef isinf
#undef islower
#undef isnan
#undef isspace
#undef isupper
#undef log
#undef log10
#undef log2
#undef max
#undef min
#undef not
#undef or
#undef sin
#undef sin
#undef sqrt
#undef strcmp
#undef strcmpi
#undef strncmp
#undef strncmpi
#undef tan
#undef xor


#ifndef mH
#define mH

enum op_t {
  OP_DIFF,
  OP_DEL2,
  OP_GRADIENT,
  OP_ALL,
  OP_ANY,
  OP_CUMPROD,
  OP_CUMSUM,
  OP_MAX_1,
  OP_MEDIAN,
  OP_MIN_1,
  OP_PROD,
  OP_SUM,
  OP_UNWRAP,
  OP_ABS,
  OP_ACOS,
  OP_ACOSH,
  OP_ACOT,
  OP_ACOTH,
  OP_ACSC,
  OP_ACSCH,
  OP_AND,
  OP_ANGLE,
  OP_ASEC,
  OP_ASECH,
  OP_ASIN,
  OP_ASINH,
  OP_ATAN,
  OP_ATAN2,
  OP_ATANH,
  OP_BETA,
  OP_BETALN,
  OP_BITAND,
  OP_BITCMP,
  OP_BITGET,
  OP_BITOR,
  OP_BITRESET,
  OP_BITSET,
  OP_BITSHIFT,
  OP_BITXOR,
  OP_CEIL,
  OP_CONJ,
  OP_COS,
  OP_COSH,
  OP_COT,
  OP_COTH,
  OP_CSC,
  OP_CSCH,
  OP_EQ,
  OP_ERFC,
  OP_ERFCX,
  OP_ERFINV,
  OP_ERF,
  OP_EXP,
  OP_EXPINT,
  OP_FIX,
  OP_FLOOR,
  OP_GAMMA,
  OP_GAMMAINC,
  OP_GAMMALN,
  OP_GE,
  OP_GT,
  OP_IMAG,
  OP_ISFINITE,
  OP_ISINF,
  OP_ISNAN,
  OP_LDIVIDE,
  OP_LE,
  OP_LOG,
  OP_LOG10,
  OP_LOG2,
  OP_LT,
  OP_MAX_2,
  OP_MINUS,
  OP_MIN_2,
  OP_MOD,
  OP_MTIMES,
  OP_NE,
  OP_NOT,
  OP_OR,
  OP_PLUS,
  OP_POW2_1,
  OP_POW2_2,
  OP_POWER,
  OP_RDIVIDE,
  OP_REM,
  OP_ROUND,
  OP_SEC,
  OP_SECH,
  OP_SIGN,
  OP_SIN,
  OP_SINC,
  OP_SINH,
  OP_SQRT,
  OP_TAN,
  OP_TANH,
  OP_TIMES,
  OP_UMINUS,
  OP_UPLUS,
  OP_XOR
};

#undef TMP_MAX
#undef abs
#undef acos
#undef and
#undef asin
#undef atan
#undef atan2
#undef bitand
#undef bitor
#undef clear
#undef cos
#undef exp
#undef fabs
#undef feof
#undef ferror
#undef finite
#undef isinf
#undef islower
#undef isnan
#undef isspace
#undef isupper
#undef log
#undef log10
#undef log2
#undef max
#undef min
#undef not
#undef or
#undef sin
#undef sin
#undef sqrt
#undef strcmp
#undef strcmpi
#undef strncmp
#undef strncmpi
#undef tan
#undef xor


class Mm;
typedef const Mm& cMm;
class Mr;
typedef const Mr& cMr;

#define dMm(x) Mm x(i_o,#x,0)

#define M_VECTOR(x,v) x=zeros(1,int(sizeof(v)/sizeof(v[0]))); \
		    copy_data(x.addr(),v,int(sizeof(v)/sizeof(v[0])));

#define mswitch(x)	switchdepth++; switchvar.br(switchdepth)=switchinit(x); do
#define beginmswitch	if (switchfalse)
#define mcase(x)	else if (isequal(x,switchvar.br(switchdepth)))
#define otherwise	else
#define endmswitch	while (0);switchdepth--

struct i_o_t {
  int i;
  int o;
};

#define Mcinitsz 16

class Mc {
  friend class Mm;

  const Mm* initd[Mcinitsz];
  int initdyn[Mcinitsz];
  const Mm** d;
  int* dyn;
  int siz;
  int last;
  int cl;
  void DLLI operator =(const Mc&);
public:
  DLLI Mc();
  DLLI Mc(const Mc& x);
  DLLI Mc(int new_cl, int new_dyn, cMm x);
  DLLI ~Mc();
  const Mm* br(int idx) const { return d[idx-1]; }
  int size() const { return last+1; }
  Mc RDLLI operator ,(cMm x);
  Mc RDLLI operator ,(const char* x);
  Mc RDLLI operator ,(double x);
};

enum Mr_idx {
  Mr_idxx=-1, Mr_idx0=0, Mr_idx1=1, Mr_idx2=2, Mr_idx3=3, Mr_idx4=4
};

enum Mr_idx_type {
  Mr_idx_paren='(', Mr_idx_br='{', Mr_idx_struct='.', Mr_idx_error='?'
};

class Mr {
public:
  friend class Mm;
  Mr_idx       indexed;
  Mr_idx_type  how;
  const char*  field;
  int          isr;
  Mm*          data;
  Mr*          datar;
  int          idx_free[max_ndims];
  Mm*          idx[max_ndims];

  DLLI Mr();
  DLLI Mr(cMm src);
  DLLI Mr(cMr src);
  DLLI Mr(cMm src, const char* f);
  DLLI Mr(cMr src, const char* f);
  DLLI Mr(Mr_idx_type new_how, cMm src, cMm i0);
  DLLI Mr(Mr_idx_type new_how, cMm src, cMm i0, cMm i1);
  DLLI Mr(Mr_idx_type new_how, cMm src, cMm i0, cMm i1, cMm i2);
  DLLI Mr(Mr_idx_type new_how, cMm src, cMm i0, cMm i1, cMm i2, cMm i3);
  DLLI Mr(Mr_idx_type new_how, cMr src, cMm i0);
  DLLI Mr(Mr_idx_type new_how, cMr src, cMm i0, cMm i1);
  DLLI Mr(Mr_idx_type new_how, cMr src, cMm i0, cMm i1, cMm i2);
  DLLI Mr(Mr_idx_type new_how, cMr src, cMm i0, cMm i1, cMm i2, cMm i3);
  DLLI ~Mr();
  int DLLI size() const;
  Mr DLLI member(const char* field_name) const { return Mr(*this, field_name); }
  Mr DLLI operator ()(cMm i0) const { return Mr(Mr_idx_paren, *this, i0); }
  Mr DLLI operator ()(cMm i0, cMm i1) const { return Mr(Mr_idx_paren, *this, i0, i1); }
  Mr DLLI operator ()(cMm i0, cMm i1, cMm i2) const { return Mr(Mr_idx_paren, *this, i0, i1, i2); }
  Mr DLLI operator ()(cMm i0, cMm i1, cMm i2, cMm i3) const { return Mr(Mr_idx_paren, *this, i0, i1, i2, i3); }

  Mr DLLI br(cMm i0) const { return Mr(Mr_idx_br,*this, i0); }
  Mr DLLI br(cMm i0, cMm i1) const { return Mr(Mr_idx_br,*this, i0, i1); }
  Mr DLLI br(cMm i0, cMm i1, cMm i2) const { return Mr(Mr_idx_br,*this, i0, i1, i2); }
  Mr DLLI br(cMm i0, cMm i1, cMm i2, cMm i3) const { return Mr(Mr_idx_br,*this, i0, i1, i2, i3); }

  void DLLI operator =(cMm src);
  void DLLI operator =(cMr src);
  int find_index_sizes(int sub_index_sizes[max_ndims]) const;
  int advance_linear_index(int sub_index_sizes[max_ndims], int sub_index[max_ndims], int sub_indexed[max_ndims],int find_limits, int index_limits[max_ndims], int err) const;
}; // Mr

/* TODO
template <class T>class Sr {
  friend class M<T>;

  int indexed;
  M<T>& data;
  int idx[max_ndims];
public:
  DLLI Sr();
  DLLI Sr(const Sr<T>& src);
  DLLI Sr(const M<T>& src);
  DLLI Sr(const M<T>& src, m_type i0);
  DLLI Sr(const M<T>& src, m_type i0, m_type i1);
  DLLI Sr(const M<T>& src, m_type i0, m_type i1, m_type i2);
  DLLI Sr(const M<T>& src, m_type i0, m_type i1, m_type i2, m_type i3);
  DLLI ~Sr();
  void DLLI operator =(const M<T>& src);
  void DLLI operator =(const Sr<T>& src);
}; // Sr
*/

enum mt_matrix_types {
  mt_uninit_matrix=0, mt_double_matrix, mt_uint8_matrix, mt_matrix_matrix
};


struct M_types {
  unsigned int str;
  unsigned int sparse;
  unsigned int logical;
  unsigned int scalar;
  unsigned int persistent;
  unsigned int global;
  mt_matrix_types type;
};


class Mm {
  friend class Mr;

protected:
  int ndims;
  int dims[max_ndims];
  int p;
  int* pc;
  m_type* pr;
  m_type* pi;
  int nnz;
  int* index;
  const char* self_name;
  const char** fields;
  int nfields;
  int classid;
public:
  Mm* old_me;
  M_types flags;

  void new_p(int isc, int iss);
  void delete_p();

  static void PDLLI operator new(size_t sz);
  static void PDLLI operator new(size_t sz, void* p);
  static void DLLI operator delete(void *p);

  DLLI Mm();
  DLLI Mm(int isc, int iss, int nonzeros, int nrows, int ncols, mt_matrix_types new_type=mt_double_matrix);
  DLLI Mm(int isc, int iss, int nonzeros, int new_ndims, const int new_dims[max_ndims], mt_matrix_types new_type=mt_double_matrix);
  DLLI Mm(i_o_t, const char* mname, int isglobal);
  DLLI Mm(const char* mname, int m, int n);
  DLLI Mm(int aisc, cMm x, cMm y, cMm dim1, op_t op, int do_dim, Mm& minmax_idx);

  DLLI Mm(m_type src);
  DLLI Mm(cMm src);
  DLLI Mm(cMm src, const char* mname);
  DLLI Mm(const Mc& src);
  DLLI Mm(cMr src, int err=1);
  DLLI ~Mm();

  Mm RDLLI operator =(cMm src);
  void DLLI deepcopy(cMm src, mt_matrix_types new_type=mt_uninit_matrix);
  void DLLI deepcopy(int isc, cMm src, mt_matrix_types new_type=mt_uninit_matrix);
  void DLLI deepcopy(int isc, int iss, cMm src, mt_matrix_types new_type=mt_uninit_matrix);
  int DLLI getreal(int force=0) const;
  int DLLI getcomplex();
  void DLLI collapse();

  inline int DLLI rows() const { return dims[0]; }
  inline int DLLI cols() const { return dims[1]; }
  int DLLI size() const;
  int DLLI size(int dim) const;
  int DLLI nsingleton() const;
  int DLLI vectordim() const;
  int DLLI length() const;
  const char PDLLI getname() const { return self_name; }
  void DLLI setname(const char* new_name);
  int DLLI isstr() const { return (flags.str!=0); }
  void DLLI setstr(int newd);
  inline int DLLI issparse() const { return (flags.sparse!=0); }
  void DLLI setsparse(int sp);
  inline int DLLI islogical() const { return (flags.logical!=0); }
  void DLLI setlogical(int newd);
  inline int DLLI isglobal() const { return (flags.global!=0); }
  inline int DLLI isstruct() const { return (fields!=NULL); }
  inline int RDLLI getndims() const { return (int&)ndims; }
  inline int PDLLI getdims() const { return (int*)dims; }
  inline M_types RDLLI getflags() { return flags; }
  inline mt_matrix_types DLLI gettype() const { return flags.type; }
  inline int DLLI getnfields() const { return nfields; }
  inline const char PPDLLI getfields() const { return fields; }
  inline int DLLI isc() const { return (pi!=NULL); }
  int DLLI issamename(const char *s) const { return s==self_name; }
  int DLLI dirty() const;
  int DLLI getp() const { return p; }

  Mm DLLI safebr(int i0) const;

  inline m_type PDLLI getpr(m_type*) const { return (m_type*)pr; }
  inline m_type PDLLI getpi(m_type*) const { return (m_type*)pi; }
  inline uint8  PDLLI getpr(uint8*)  const { return (uint8*)pr; }
  inline uint8  PDLLI getpi(uint8*)  const { return (uint8*)pi; }
  inline Mm     PDLLI getpr(Mm*)     const { return (Mm*)pr; }
  inline Mm     PDLLI getpi(Mm*)     const { return (Mm*)pi; }

  inline m_type PDLLI getpr(m_type*,int i0) const { return i0-1+(m_type*)pr; }
  inline m_type PDLLI getpi(m_type*,int i0) const { return i0-1+(m_type*)pi; }
  inline uint8  PDLLI getpr(uint8*,int i0)  const { return i0-1+(uint8*)pr; }
  inline uint8  PDLLI getpi(uint8*,int i0)  const { return i0-1+(uint8*)pi; }
  inline Mm     PDLLI getpr(Mm*,int i0)     const { return i0-1+(Mm*)pr; }
  inline Mm     PDLLI getpi(Mm*,int i0)     const { return i0-1+(Mm*)pi; }

  inline m_type PDLLI getpr(m_type*,int i0,int i1) const { return i0-1+(i1-1)*dims[0]+(m_type*)pr; }
  inline m_type PDLLI getpi(m_type*,int i0,int i1) const { return i0-1+(i1-1)*dims[0]+(m_type*)pi; }
  inline uint8  PDLLI getpr(uint8*,int i0,int i1)  const { return i0-1+(i1-1)*dims[0]+(uint8*)pr; }
  inline uint8  PDLLI getpi(uint8*,int i0,int i1)  const { return i0-1+(i1-1)*dims[0]+(uint8*)pi; }
  inline Mm     PDLLI getpr(Mm*,int i0,int i1)     const { return i0-1+(i1-1)*dims[0]+(Mm*)pr; }
  inline Mm     PDLLI getpi(Mm*,int i0,int i1)     const { return i0-1+(i1-1)*dims[0]+(Mm*)pi; }

  m_type PDLLI addr() const;
  m_type PDLLI addr(int i0) const;
  m_type PDLLI addr(int i0,int i1) const;
  m_type PDLLI addi() const;
  m_type PDLLI addi(int i0) const;
  m_type PDLLI addi(int i0,int i1) const;
  inline int PDLLI getindex() const { return index; }
  inline int RDLLI getnnz() const { return (int&)nnz; }

  m_type RDLLI r() const;
  m_type RDLLI r(double i0) const;
  m_type RDLLI r(double i0, double i1) const;
  m_type RDLLI r(double i0, double i1, double i2) const;
  m_type RDLLI i() const;
  m_type RDLLI i(double i0) const;
  m_type RDLLI i(double i0, double i1) const;
  m_type RDLLI i(double i0, double i1, double i2) const;
  uint8 RDLLI ur(int i0) const;
  uint8 RDLLI ur(int i0, int i1) const;
  Mm RDLLI mr(int i0) const;
  Mm RDLLI mr(int i0, int i1) const;
/*
  Sr<T> DLLI operator ()(m_type i0) const { return Sr<T>(*this, i0); }
  Sr<T> DLLI operator ()(m_type i0, m_type i1) const { return Sr<T>(*this, i0, i1); }
  Sr<T> DLLI operator ()(m_type i0, m_type i1, m_type i2) const { return Sr<T>(*this, i0, i1, i2); }
  Sr<T> DLLI operator ()(m_type i0, m_type i1, m_type i2, m_type i3) const { return Sr<T>(*this, i0, i1, i2, i3); }
*/
  Mr DLLI member(const char* field) const { return Mr(*this, field); }
  Mr DLLI operator ()(cMm i0) const { return Mr(Mr_idx_paren, *this, i0); }
  Mr DLLI operator ()(cMm i0, cMm i1) const { return Mr(Mr_idx_paren, *this, i0, i1); }
  Mr DLLI operator ()(cMm i0, cMm i1, cMm i2) const { return Mr(Mr_idx_paren, *this, i0, i1, i2); }
  Mr DLLI operator ()(cMm i0, cMm i1, cMm i2, cMm i3) const { return Mr(Mr_idx_paren, *this, i0, i1, i2, i3); }

  Mr DLLI br(cMm i0) const {  return Mr(Mr_idx_br,*this, i0); }
  Mr DLLI br(cMm i0, cMm i1) const { return Mr(Mr_idx_br,*this, i0, i1); }
  Mr DLLI br(cMm i0, cMm i1, cMm i2) const { return Mr(Mr_idx_br,*this, i0, i1, i2); }
  Mr DLLI br(cMm i0, cMm i1, cMm i2, cMm i3) const { return Mr(Mr_idx_br,*this, i0, i1, i2, i3); }

  m_type& fastindex(double i0) const { return pr[int(i0)-1]; }
  m_type& fastindex(double i0, double i1) const { return pr[int(i0)-1+(int(i1)-1)*dims[0]]; }

  void DLLI vwcopy1(cMm src, cMm v);
  void DLLI vwcopy2(cMm src, cMm v, cMm w);
  void DLLI vwcopyn(cMr src, cMm rhs);
  void DLLI vwcopy0(cMr src);

  int DLLI findfield(const char* field, int err) const;
  const char PDLLI getfield(int i) const;
  int DLLI addfield(const char* field,int quick);
  int DLLI rmfield(const char* field);
  void DLLI extend_nfields(int new_nfields);

  int DLLI getclassid() const { return classid; }
  void DLLI setclassid(int new_classid) { classid=new_classid; }

  void DLLI reshape(const int m, const int n);
  void DLLI reshape(const int new_ndims, const int new_dims[max_ndims]);

  void DLLI print(int full) const;

  void DLLI warn_uninit() const;

  void DLLI resparse();
  int  DLLI search(int idx) const;
  void DLLI sort();
  void DLLI extend_nnz(int new_nnz);

}; // M

Mc DLLI BR(cMm src);
Mc DLLI BR(const char* src);
Mc DLLI BR(double src);
Mc DLLI CL(cMm src);
Mc DLLI CL(const char* src);
Mc DLLI CL(double src);
m_type DLLI scalar(m_type x);
m_type DLLI scalar(cMm x);
Mm DLLI switchinit(cMm x);

char* op_name(op_t op);
#endif // mH


extern const long      Mmaxsize;

extern DLLW i_o_t      DLLI i_o;
extern DLLW int        DLLI switchfalse;
extern DLLW int        DLLI switchdepth;

typedef int eval_type(const char*);
typedef Mm feval_type(Mm , Mm, i_o_t, Mm&, Mm&, Mm&, Mm& , Mm& , Mm& );
typedef Mm callback_type(Mm);
extern eval_type    	PDLLI eval_func;
extern feval_type 	PDLLI feval_func;
extern callback_type 	PDLLI callback_func;
Mm feval_func_1(cMm fun, cMm varargin);

extern DLLW void PDLLI mpath_pointer;

extern DLLW double     DLLI nargin_val;
extern DLLW double     DLLI nargout_val;
extern DLLW int        DLLI nargin_set;
extern DLLW int        DLLI nargout_set;
extern DLLW Mm	DLLI TICTOC;
extern DLLW Mm	DLLI ans;
extern DLLW Mm	DLLI i;
extern DLLW Mm	DLLI j;
extern DLLW Mm	DLLI pi;
extern DLLW Mm	DLLI Inf;
extern DLLW Mm	DLLI NaN;
extern DLLW Mm	DLLI eps;
extern DLLW Mm	DLLI x_M;
extern DLLW Mm	DLLI semi;
extern DLLW Mm	DLLI c_p;
extern DLLW Mm	DLLI nop_M;
extern DLLW Mm	DLLI zero_M;
extern DLLW Mm	DLLI one_M;
extern DLLW Mm	DLLI l_M;
extern DLLW Mm	DLLI page_screen_output;
extern DLLW Mm	DLLI implicit_str_to_num_ok;
extern DLLW Mm	DLLI empty_list_elements_ok;
extern DLLW Mm	DLLI switchvar;

Mm DLLI airy(cMm z);
Mm DLLI airy(cMm k, cMm z);
Mm DLLI airy(cMm z, i_o_t, Mm& w, Mm& err);
Mm DLLI airy(cMm k, cMm z, i_o_t, Mm& w, Mm& err);

Mm DLLI bessel(cMm nu);
Mm DLLI bessel(cMm nu, cMm z);
Mm DLLI bessel(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);

Mm DLLI bessela(cMm nu);
Mm DLLI bessela(cMm nu, cMm z);
Mm DLLI bessela(cMm nu, cMm z, i_o_t, Mm& J, Mm& ndigits);

Mm DLLI besselh(cMm nu);
Mm DLLI besselh(cMm nu, cMm z);
Mm DLLI besselh(cMm nu, cMm k, cMm z);
Mm DLLI besselh(cMm nu, cMm k, cMm z, cMm scale1);
Mm DLLI besselh(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI besselh(cMm nu, cMm k, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI besselh(cMm nu, cMm k, cMm z, cMm scale1, i_o_t,  Mm& w, Mm& err);

Mm DLLI besseli(cMm nu);
Mm DLLI besseli(cMm nu, cMm z);
Mm DLLI besseli(cMm nu, cMm z, cMm scale1);
Mm DLLI besseli(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI besseli(cMm nu, cMm z, cMm scale1, i_o_t,  Mm& w, Mm& err);

Mm DLLI besselj(cMm nu);
Mm DLLI besselj(cMm nu, cMm z);
Mm DLLI besselj(cMm nu, cMm z, cMm scale1);
Mm DLLI besselj(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI besselj(cMm nu, cMm z, cMm scale1, i_o_t,  Mm& w, Mm& err);

Mm DLLI besselk(cMm nu);
Mm DLLI besselk(cMm nu, cMm z);
Mm DLLI besselk(cMm nu, cMm z, cMm scale1);
Mm DLLI besselk(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI besselk(cMm nu, cMm z, cMm scale1, i_o_t,  Mm& w, Mm& err);

Mm DLLI bessely(cMm nu);
Mm DLLI bessely(cMm nu, cMm z);
Mm DLLI bessely(cMm nu, cMm z, cMm scale1);
Mm DLLI bessely(cMm nu, cMm z, i_o_t,  Mm& w, Mm& err);
Mm DLLI bessely(cMm nu, cMm z, cMm scale1, i_o_t,  Mm& w, Mm& err);

Mm DLLI colon();
Mm DLLI colon(cMm j);
Mm DLLI colon(double j, double k);
Mm DLLI colon(cMm j, cMm k);
Mm DLLI colon(double j, double i, double k);
Mm DLLI colon(cMm j, cMm i, cMm k);
#define ramp colon


Mm DLLI cputime();
Mm DLLI date();
Mm DLLI datenum(cMm x);
Mm DLLI datenum(cMm year, cMm month);
Mm DLLI datenum(cMm year, cMm month, cMm day);
Mm DLLI datenum(cMm year, cMm month, cMm day, cMm hour);
Mm DLLI datenum(cMm year, cMm month, cMm day, cMm hour, cMm minute);
Mm DLLI datenum(Mm year, Mm month, cMm day, cMm hour, cMm minute, cMm second);
Mm DLLI datevec(cMm J);
Mm DLLI datevec(cMm J, i_o_t, Mm& year, Mm& month);
Mm DLLI datevec(cMm J, i_o_t, Mm& year, Mm& month, Mm& day);
Mm DLLI datevec(cMm J, i_o_t, Mm& year, Mm& month, Mm& day, Mm& hour);
Mm DLLI datevec(cMm J, i_o_t, Mm& year, Mm& month, Mm& day, Mm& hour, Mm& minute);
Mm DLLI datevec(Mm J, i_o_t, Mm& year, Mm& month, Mm& day, Mm& hour, Mm& minute, Mm& second);
Mm DLLI eomday(cMm y);
Mm DLLI eomday(cMm y, cMm m);
Mm DLLI etime(cMm );
Mm DLLI etime(Mm t1, Mm t0);
Mm DLLI mclock();
Mm DLLI now();
Mm DLLI tic();
Mm DLLI toc();
Mm DLLI weekday(cMm j);
Mm DLLI weekday(Mm j, i_o_t, Mm& D, Mm& W);
Mm DLLI diag(cMm x);
Mm DLLI diag(cMm x, cMm kk);

Mm DLLI row2col(Mm x);
Mm DLLI permute(cMm x);
Mm DLLI permute(Mm x, Mm order);
Mm DLLI ipermute(cMm x);
Mm DLLI ipermute(cMm x, Mm order);
Mm DLLI ndims(cMm x);
Mm DLLI squeeze(Mm x);
Mm DLLI ind2sub(cMm sz);
Mm DLLI ind2sub(cMm sz, i_o_t, Mm& i0, Mm& i1);
Mm DLLI ind2sub(cMm sz, i_o_t, Mm& i0, Mm& i1, Mm& i2);
Mm DLLI ind2sub(cMm sz, i_o_t, Mm& i0, Mm& i1, Mm& i2, Mm& i3);
Mm DLLI ind2sub(cMm sz, cMm i);
Mm DLLI ind2sub(cMm sz, Mm i, i_o_t, Mm& i0, Mm& i1);
Mm DLLI ind2sub(cMm sz, Mm i, i_o_t, Mm& i0, Mm& i1, Mm& i2);
Mm DLLI ind2sub(cMm sz, Mm i, i_o_t, Mm& i0, Mm& i1, Mm& i2, Mm& i3);
Mm DLLI sub2ind(cMm sz);
Mm DLLI sub2ind(cMm sz, cMm i0);
Mm DLLI sub2ind(cMm sz, cMm i0, cMm i1);
Mm DLLI sub2ind(cMm sz, cMm i0, cMm i1, cMm i2);
Mm DLLI sub2ind(cMm sz, cMm i0, cMm i1, cMm i2, cMm i3);
Mm DLLI subsasgn(cMm a);
Mm DLLI subsasgn(cMm a, cMm s);
Mm DLLI subsasgn(cMm a, cMm s, cMm b);
Mm DLLI subsindex(cMm x);
Mm DLLI subsref(cMm a);
Mm DLLI subsref(cMm a, cMm s);



#ifdef MATLIB_MEMDEBUG


#define LOWER_FENCE 0x55
#define UPPER_FENCE 0x33
#define new_data(p,siz) 		\
  {					\
    int i=((siz)+1)*sizeof(*p); 	\
    void* v=malloc(sizeof(int)+1+i+1);	\
    if (v==NULL) {			\
      error(out_of_memory);		\
    }					\
    char* c=(char*)v;			\
    *(int*)c=i; c+=sizeof(int); 	\
    *c=LOWER_FENCE; c++;		\
    v=c; c+=i;				\
    *c=UPPER_FENCE;			\
    *((void **)&p)=v;			\
  }

#define delete_data(p)			\
  {					\
    void* v;				\
    assert(p!=NULL);			\
    char* c=(char*)p;			\
    c--;				\
    c-=sizeof(int);			\
    v=c;				\
    int i=*(int*)c;			\
    c+=sizeof(int);			\
    assert(*c==LOWER_FENCE);		\
    c++;				\
    c+=i;				\
    assert(*c==UPPER_FENCE);		\
    free(v);				\
  }

#else

#define new_data(p,siz) 				\
  *((void **)&p)=malloc(((siz)+1)*sizeof(*p));	\
  if (p==NULL) {					\
    error(s_out_of_memory);				\
  }

#define delete_data(p)	\
  free(p);

#endif

#define copy_data(dest, src, n) \
  {for (int zcdi=0;zcdi<(n);zcdi++) (dest)[zcdi]=(src)[zcdi];}

#define zero_data(dest, n)	\
  { for (int zzdi=0;(zzdi<(n))&&(sizeof(*(dest))<=12);zzdi++) (dest)[zzdi]=0;}

#define fill_data(dest, n, val) \
  {for (int zfdi=0;zfdi<(n);zfdi++) (dest)[zfdi]=(val);}

#define neg_data(dest, n)	\
  {for (int zndi=0;zndi<(n);zndi++) (dest)[zndi]=-(dest)[zndi];}

typedef Mm func11M(Mm);
typedef Mm func12M(Mm, Mm);
typedef Mm func13M(Mm, Mm, Mm);
typedef Mm func22M(Mm, Mm, i_o_t, Mm&, Mm&);
void DLLI register_class_function(const char* classname, const char* funcname, func11M* func);
void DLLI register_class_function(const char* classname, const char* funcname, func12M* func);
void DLLI register_class_function(const char* classname, const char* funcname, func13M* func);
void DLLI register_class_function(const char* classname, const char* funcname, func22M* func);
void DLLI unregister_class_function(const char* classname, const char* funcname);
int  DLLI is_class_function(const char* classname, const char* funcname);
Mm   DLLI call_class_function(const char* funcname, cMm a);
Mm   DLLI call_class_function(const char* funcname, cMm a, cMm b);
Mm   DLLI call_class_function(const char* funcname, cMm a, cMm b, cMm c);
Mm   DLLI call_class_function(const char* funcname, cMm a, cMm b, Mm& x, Mm& y);

Mm DLLI cell(cMm x);
Mm DLLI cell(cMm x, cMm y);
Mm DLLI cell(cMm x, cMm y, cMm o);
Mm DLLI cell(cMm x, cMm y, cMm o, cMm p);
Mm DLLI cells(cMm x);
Mm DLLI cells(cMm x, cMm y);
Mm DLLI cellstr(cMm x);
Mm DLLI cell_from_array(int n, const Mm* x[]);
Mm DLLI cell2struct(cMm x);
Mm DLLI cell2struct(cMm x, cMm f);
Mm DLLI cell2struct(cMm x, cMm f, cMm dim1);
int DLLI iscellstr(cMm x);
int DLLI isa(cMm x);
int DLLI isa(cMm x, cMm cls);
Mm DLLI mclass(cMm x);
Mm DLLI mclass(cMm x, cMm class_name);
Mm DLLI mchar(cMm varargin);
Mm DLLI mdouble(cMm x);
Mm DLLI mlogical(cMm x);
Mm DLLI muint8(cMm x);
Mm DLLI muint16(cMm x);
Mm DLLI fieldnames(cMm s);
Mm DLLI isfield(cMm s);
Mm DLLI isfield(cMm s, cMm f);
Mm DLLI getfield(cMm s);
Mm DLLI getfield(cMm s, cMm varargin);
Mm DLLI mstruct(cMm varargin);
Mm DLLI setfield(cMm s);
Mm DLLI setfield(cMm s, cMm field);
Mm DLLI setfield(cMm s, cMm field, cMm v);
Mm DLLI struct2cell(cMm s);
Mm DLLI rmfield(cMm s);
Mm DLLI rmfield(cMm s, cMm fields);

Mm DLLI minline(cMm x);
Mm DLLI minline(cMm x, cMm l);
Mm DLLI inlinefcn(cMm x);
Mm DLLI inlinefcn(cMm x, cMm l);
Mm DLLI formula(cMm f);
Mm DLLI fcnchk(cMm f);
Mm DLLI fcnchk(cMm f, cMm x);
Mm DLLI fcnchk(cMm f, cMm x, i_o_t, Mm& fo, Mm& msg);
Mm DLLI prefcnchk(cMm f, cMm x, cMm y, cMm z, i_o_t, Mm& fo, Mm& msg);


#define tostr1(x) #x
#define tostr2(x) tostr1(x)
#define m_type_name tostr2(m_type)

extern DLLW unsigned int DLLI mt_default_error_flags;

#define mt_default_error_none	      0
#define mt_default_error_text_output  1
#define mt_default_error_message_box  2
#define mt_default_error_abort	      4
#define mt_default_error_report       8
#define mt_default_error_throw	     16

void DLLI mt_set_default_error_flags(unsigned int f, unsigned int v);
void DLLI mt_set_default_warning_flags(unsigned int f, unsigned int v);

typedef void mt_print_handler_type(const char* msg);
typedef void mt_error_handler_type(const char* error_msg);
typedef void mt_warning_handler_type(const char* error_msg);
typedef void mt_message_handler_type(const char* s);
typedef void mt_logmessage_handler_type(const char* s);
typedef void mt_debug_handler_type(int c, const char* error_msg);

extern mt_print_handler_type PDLLI mt_print_handler;
extern mt_error_handler_type PDLLI mt_error_handler;
extern mt_warning_handler_type PDLLI mt_warning_handler;
extern mt_logmessage_handler_type PDLLI mt_logmessage_handler;
extern mt_message_handler_type PDLLI mt_message_handler;
extern mt_debug_handler_type PDLLI mt_debug_handler;

mt_print_handler_type PDLLI mt_set_print_handler(mt_print_handler_type* new_print_handler);
mt_error_handler_type PDLLI mt_set_error_handler(mt_error_handler_type* new_error_handler);
mt_warning_handler_type PDLLI mt_set_warning_handler(mt_warning_handler_type* new_error_handler);
mt_message_handler_type PDLLI mt_set_message_handler(mt_message_handler_type* new_message_handler);
mt_logmessage_handler_type PDLLI mt_set_logmessage_handler(mt_logmessage_handler_type* new_logmessage_handler);
mt_debug_handler_type PDLLI mt_set_debug_handler(mt_debug_handler_type* new_debug_handler);

int DLLI mt_was_handler_set();

extern char error_mname[];
extern int error_line_no;
extern char* error_mStream;

void DLLI error_lasterr(void);
void DLLI error_call_stack(char* b);

#ifdef BIT16
#define ERROR_BUF_LEN 512
#else
#define ERROR_BUF_LEN 2048
#endif

void DLLI error(const char* format,...);
void DLLI warning(const char* format,...);
void DLLI message(const char* format,...);
void DLLI logmessage(const char* format,...);

void DLLI error(int line, const char* format,...);
void DLLI warning(int line, const char* format,...);

#define mt_debug(x,y) if (mt_default_error_flags&mt_default_error_report) mt_debug_handler(x,y)

#ifndef mt_debug
#define mt_debug(x,y) ;
#endif

class mt_error_stack_t {
public:
  const char* names[mt_stack_max_depth];
  int  lines[mt_stack_max_depth];
  int  depth;
  int* current_line;
  mt_error_stack_t() {
    current_line=lines;
    reset();
  }
  void reset() {
    depth=0;
  }
  void begin(const char* s) {
    depth++;
    if (depth==mt_stack_max_depth) {
      depth=1;
    }
    names[depth]=s;
    current_line=lines+depth;
    mt_debug('+',s);
  }
  void end() {
    depth--;
    current_line=lines+depth;
    mt_debug('-',NULL);
  }
};


#define call_stack_begin    mt_error_stack.begin(__FILE__); *mt_error_stack.current_line=0;
#define call_stack_end	    mt_error_stack.end();
#define call_stack_depth    mt_error_stack.depth
#define _		    *mt_error_stack.current_line=__LINE__; mt_debug('l',NULL);

extern DLLW mt_error_stack_t DLLI mt_error_stack;

Mm DLLI czeros(int isc, int m, int n);
Mm DLLI czeros(int isc, int ndims, const int dims[max_ndims]);
Mm DLLI ceye(int isc, int m, int n);
Mm DLLI eye();
Mm DLLI eye(cMm x);
Mm DLLI eye(m_type x);
Mm DLLI eye(m_type m, m_type n);
Mm DLLI eye(cMm m, cMm n);
Mm DLLI eye(cMm m, cMm n, cMm o);
Mm DLLI eye(cMm m, cMm n, cMm o, cMm p);
Mm DLLI randM(cMm x);
Mm DLLI randM(m_type x);
Mm DLLI randM(m_type m, m_type n);
Mm DLLI randM(cMm m, cMm n);
Mm DLLI randM(cMm m, cMm n, cMm o);
Mm DLLI randM(cMm m, cMm n, cMm o, cMm p);
Mm DLLI randn(cMm x);
Mm DLLI randn(m_type x);
Mm DLLI randn(m_type m, m_type n);
Mm DLLI randn(cMm m, cMm n);
Mm DLLI randn(cMm m, cMm n, cMm o);
Mm DLLI randn(cMm m, cMm n, cMm o, cMm p);
Mm DLLI ones();
Mm DLLI ones(m_type x);
Mm DLLI ones(m_type m, m_type n);
Mm DLLI ones(cMm x);
Mm DLLI ones(cMm m, cMm n);
Mm DLLI ones(cMm m, cMm n, cMm o);
Mm DLLI ones(cMm m, cMm n, cMm o, cMm p);
Mm DLLI zeros();
Mm DLLI zeros(m_type x);
Mm DLLI zeros(m_type m, m_type n);
Mm DLLI zeros(cMm x);
Mm DLLI zeros(cMm m, cMm n);
Mm DLLI zeros(cMm m, cMm n, cMm o);
Mm DLLI zeros(cMm m, cMm n, cMm o, cMm p);

m_type    DLLI randM();
m_type    DLLI randn();

#define rand randM

Mm DLLI fft(cMm x);
Mm DLLI fft(cMm x, cMm n);
Mm DLLI fft(cMm x, cMm n, cMm dim1);
Mm DLLI ifft(cMm x);
Mm DLLI ifft(cMm x, cMm n);
Mm DLLI ifft(cMm x, cMm n, cMm dim1);
Mm DLLI dft(cMm x);

Mm DLLI fft2(cMm x);
Mm DLLI fft2(cMm x, cMm m);
Mm DLLI fft2(cMm x, cMm m, cMm n);
Mm DLLI ifft2(cMm x);
Mm DLLI ifft2(cMm x, cMm m);
Mm DLLI ifft2(cMm x, cMm m, cMm n);

Mm DLLI fftshift(cMm x);
Mm DLLI ifftshift(cMm x);


Mm DLLI cd();
Mm DLLI cd(cMm dir1);
Mm DLLI chdir(cMm dir1);
Mm DLLI copyfile(cMm src);
Mm DLLI copyfile(cMm src, cMm dest);
Mm DLLI deleteM(cMm filename);
Mm DLLI dos(cMm command);
Mm DLLI dos(cMm command, i_o_t, Mm& status, Mm& sout);
Mm DLLI dos(cMm command, cMm echo);
Mm DLLI dos(cMm command, cMm echo, i_o_t, Mm& status, Mm& sout);
Mm DLLI fclose(cMm fid);
Mm DLLI feof(cMm fid);
Mm DLLI ferror(cMm fid);
Mm DLLI ferror(cMm fid, cMm clear);
Mm DLLI ferror(cMm fid, cMm clear, i_o_t, Mm& msg, Mm& errnum);
Mm DLLI ferror(cMm fid, i_o_t, Mm& msg, Mm& errnum);
Mm DLLI fflush(cMm fid);
Mm DLLI fgetl(cMm fid);
Mm DLLI fgets(cMm fid);
Mm DLLI fgets(cMm fid, cMm nchar);
Mm DLLI filesep();
Mm DLLI fopen(cMm filename);
Mm DLLI fopen(cMm filename, cMm permission);
Mm DLLI fopen(cMm filename, cMm permission, cMm machine);
Mm DLLI fopen(cMm filename, cMm permission, cMm machine, i_o_t, Mm& fid, Mm& msg);
Mm DLLI fopen(cMm filename, cMm permission, i_o_t, Mm& fid, Mm& msg);
Mm DLLI fopen(cMm filename, i_o_t, Mm& fid, Mm& msg);
Mm DLLI fopen(cMm fid, i_o_t, Mm& filename, Mm& permission, Mm& machine);
Mm DLLI fprintf(cMm fid, cMm format1);
Mm DLLI fprintf(cMm fid, cMm format1, cMm varargin);
Mm DLLI fprintf(cMm x);
Mm DLLI fread(cMm fid);
Mm DLLI fread(cMm fid, cMm size);
Mm DLLI fread(cMm fid, cMm size, cMm precision);
Mm DLLI fread(cMm fid, cMm size, cMm precision, cMm skip);
Mm DLLI fread(cMm fid, cMm size, cMm precision, cMm skip, cMm machine);
Mm DLLI fread(cMm fid, cMm size, cMm precision, cMm skip, cMm machine, i_o_t, Mm& A, Mm& count);
Mm DLLI fread(cMm fid, cMm size, cMm precision, cMm skip, i_o_t, Mm& A, Mm& count);
Mm DLLI fread(cMm fid, cMm size, cMm precision, i_o_t, Mm& A, Mm& count);
Mm DLLI fread(cMm fid, cMm size, i_o_t, Mm& A, Mm& count);
Mm DLLI fread(cMm fid, i_o_t, Mm& A, Mm& count);
Mm DLLI frewind(cMm fid);
Mm DLLI fscanf(cMm fid);
Mm DLLI fscanf(cMm fid, cMm format1);
Mm DLLI fscanf(cMm fid, cMm format1, cMm size);
Mm DLLI fscanf(cMm fid, cMm format1, cMm size, i_o_t, Mm& A, Mm& count);
Mm DLLI fscanf(cMm fid, cMm format1, cMm size, i_o_t, Mm& A, Mm& count, Mm& errmsg);
Mm DLLI fscanf(cMm fid, cMm format1, i_o_t, Mm& A, Mm& count);
Mm DLLI fseek(cMm fid);
Mm DLLI fseek(cMm fid, cMm offset);
Mm DLLI fseek(cMm fid, cMm offset, cMm origin);
Mm DLLI ftell(cMm fid);
Mm DLLI fullfile(cMm varargin);
Mm DLLI fwrite(cMm fid);
Mm DLLI fwrite(cMm fid, cMm A);
Mm DLLI fwrite(cMm fid, cMm A, cMm precision);
Mm DLLI fwrite(cMm fid, cMm A, cMm precision, cMm skip);
Mm DLLI fwrite(cMm fid, Mm A, cMm precision, cMm skip, cMm machine);
Mm DLLI help(cMm keyword);
Mm DLLI mkdir(cMm dir1);
Mm DLLI pathsep();
Mm DLLI printf(cMm format1);
Mm DLLI printf(cMm format1, cMm varargin);
Mm DLLI rmdir(cMm dir1);
Mm DLLI stderrM();
Mm DLLI stdinM();
Mm DLLI stdoutM();
Mm DLLI system(cMm cmd);
Mm DLLI type(cMm fname);
Mm DLLI unixM(cMm command);
Mm DLLI unixM(cMm command, i_o_t, Mm& status, Mm& sout);

Mm DLLX wavread(cMm filename);
Mm DLLX wavread(cMm filename, i_o_t, Mm& wavedata, Mm& rate);
Mm DLLX wavread(Mm filename, i_o_t, Mm& wavedata_o, Mm& rate, Mm& bits);
Mm DLLI wavwrite(cMm wavedata);
Mm DLLI wavwrite(cMm wavedata, cMm filename);
Mm DLLI wavwrite(cMm wavedata, cMm rate, cMm filename);
Mm DLLX wavwrite(cMm wavedata, cMm rate, cMm bits, Mm filename);

Mm DLLI conv(cMm a);
Mm DLLI conv(Mm a, Mm b);
Mm DLLI conv2(cMm a);
Mm DLLI conv2(cMm a, cMm b);
Mm DLLI conv2(Mm A, Mm B, cMm shape);
Mm DLLI conv2(Mm hcol, Mm hrow, Mm A, cMm shape);
Mm DLLI cov(cMm a);
Mm DLLI cov(cMm a, cMm b);
Mm DLLI cov(cMm a, cMm b, cMm c);
Mm DLLI corrcoef(cMm a);
Mm DLLI corrcoef(cMm a, cMm b);
Mm DLLI cplxpair(cMm x);
Mm DLLI cplxpair(cMm x, cMm tol);
Mm DLLI cplxpair(Mm x, Mm tol, cMm dim1);
Mm DLLI deconv(cMm a);
Mm DLLI deconv(cMm a, i_o_t, Mm& q, Mm& r);
Mm DLLI deconv(cMm a, cMm b);
Mm DLLI deconv(cMm a, cMm b, i_o_t, Mm& q, Mm& r);
Mm DLLI ltitr(cMm a);
Mm DLLI ltitr(cMm a, cMm b);
Mm DLLI ltitr(cMm a, cMm b, cMm u);
Mm DLLI ltitr(Mm a, Mm b, Mm u, Mm x0);
Mm DLLI ltifr(cMm a);
Mm DLLI ltifr(cMm a, cMm b);
Mm DLLI ltifr(cMm a, cMm b, cMm s);
Mm DLLI filter(cMm b);
Mm DLLI filter(cMm b, cMm a);
Mm DLLI filter(cMm b, cMm a, cMm x);
Mm DLLI filter(cMm b, cMm a, cMm x, cMm zi);
Mm DLLI filter(cMm b, cMm a, cMm x, cMm zi, cMm dim1);
Mm DLLI filter(cMm b, cMm a, cMm x, i_o_t, Mm& y, Mm& zf);
Mm DLLI filter(cMm b, cMm a, cMm x, cMm zi, i_o_t, Mm& y, Mm& zf);
Mm DLLI filter(cMm b, cMm a, cMm x, cMm zi, cMm dim1, i_o_t, Mm& y, Mm& zf);
Mm DLLI filter2(cMm b);
Mm DLLI filter2(cMm b, cMm a);
Mm DLLI filter2(cMm b, cMm a, cMm s);
Mm DLLI freqspace(cMm n);
Mm DLLI freqspace(cMm n, cMm m);
Mm DLLI freqspace(cMm n, i_o_t, Mm& f1, Mm& f2);
Mm DLLI freqspace(cMm n, cMm m, i_o_t, Mm& f1, Mm& f2);
Mm DLLI kron(cMm a);
Mm DLLI kron(cMm a, cMm b);
Mm DLLI mean(cMm a);
Mm DLLI mean(cMm a, cMm dim1);
Mm DLLI meshdom(Mm x, Mm y, i_o_t, Mm& xx, Mm& yy);
Mm DLLI meshgrid(cMm x);
Mm DLLI meshgrid(cMm x, cMm y);
Mm DLLI meshgrid(cMm x, cMm y, cMm z);
Mm DLLI meshgrid(cMm x, i_o_t, Mm& xx, Mm& yy);
Mm DLLI meshgrid(cMm x, cMm y, i_o_t, Mm& xx, Mm& yy);
Mm DLLI meshgrid(cMm x, cMm y, cMm z, i_o_t, Mm& xx, Mm& yy);
Mm DLLI meshgrid(cMm x, i_o_t, Mm& xx, Mm& yy, Mm& zz);
Mm DLLI meshgrid(cMm x, cMm y, i_o_t, Mm& xx, Mm& yy, Mm& zz);
Mm DLLI meshgrid(cMm x, cMm y, cMm z, i_o_t, Mm& xx, Mm& yy, Mm& zz);
Mm DLLI ndgrid(cMm x);
Mm DLLI ndgrid(cMm x, cMm y);
Mm DLLI ndgrid(cMm x, i_o_t, Mm& xx, Mm& yy);
Mm DLLI ndgrid(cMm x, cMm y, i_o_t, Mm& xx, Mm& yy);
Mm DLLI ndgrid(cMm x, i_o_t, Mm& xx, Mm& yy, Mm& zz);
Mm DLLI ndgrid(cMm x, cMm y, cMm z, i_o_t, Mm& xx, Mm& yy, Mm& zz);
Mm DLLI nested(cMm n);
Mm DLLI nextpow2(cMm x);
Mm DLLI remezf(cMm nfilt, cMm edge, cMm fx, cMm wtx, cMm jtype);
Mm DLLI remezf(cMm nfilt, cMm edge, cMm grid, cMm fx, cMm wtx, cMm jtype, i_o_t, Mm& h, Mm& err, Mm& iext);
Mm DLLI stdM(cMm a);
Mm DLLI stdM(cMm a, cMm b);
Mm DLLI stdM(cMm a, cMm b, cMm dim1);
Mm DLLI trapz(cMm a);
Mm DLLI trapz(cMm a, cMm b);
Mm DLLI upfirdn(cMm x);
Mm DLLI upfirdn(cMm x, cMm h);
Mm DLLI upfirdn(cMm x, cMm h, cMm p);
Mm DLLI upfirdn(Mm x, Mm h, cMm p, cMm q);

#define STUBH(x) Mm DLLI x(Mm a0=l_M,Mm a1=l_M,Mm a2=l_M,Mm a3=l_M,Mm a4=l_M,Mm a5=l_M,Mm a6=l_M,Mm a7=l_M,Mm a8=l_M,Mm a9=l_M,Mm a10=l_M,Mm a11=l_M,Mm a12=l_M,Mm a13=l_M,Mm a14=l_M,Mm a15=l_M,Mm a16=l_M);
#define STUBH_2_2(x) Mm DLLI x(Mm a0,Mm a1,i_o_t,Mm& o0,Mm& o1);
#define STUBCXX(x)  Mm DLLX x(Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm) { return TM("Stub called."); }
#define STUBCXX_2_2(x)  Mm DLLX x(Mm,Mm,i_o_t,Mm&,Mm&) { return TM("Stub called."); }
#define STUBCXXx(x) Mm DLLX x(Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm,Mm) { return x_M; }

#define STUBCXX_CMM(x)  Mm DLLX x(cMMm) { return TM("Stub called."); }
Mm DLLI guiexit(int ivemode);
Mm DLLI designer();
Mm DLLI guide();
Mm DLLI consolemode(cMm m);
Mm DLLI allchild(cMm h);
Mm DLLI setstatus(cMm f, cMm s);
Mm DLLI setstatus(cMm s);
Mm DLLI axes();
Mm DLLI axes(cMm varargin);
Mm DLLI winaxes(void *winparent);
Mm DLLI vbaxes(cMm handle);
Mm DLLI axesposition(cMm left, cMm top, cMm width, cMm height);
Mm DLLI setapplicationhandle(void *hndl);
Mm DLLI setapplicationkeyhandle(void *hndl);
Mm DLLI agocrash();
Mm DLLI axis();
Mm DLLI axis(cMm a);
Mm DLLI axis(cMm a, cMm b);
Mm DLLI axis(cMm a, cMm b, cMm c);
Mm DLLI brighten(cMm b);
Mm DLLI brighten(cMm f, cMm b);
Mm DLLI reset(cMm h);
Mm DLLI cla();
Mm DLLI cla(cMm reset);
Mm DLLI clf();
Mm DLLI clf(cMm a);
Mm DLLI clg();
Mm DLLI drawnow();
Mm DLLI figure();
Mm DLLI figure(cMm varargin);
Mm DLLI figure_close();
Mm DLLI figure_close(cMm fig);
Mm DLLI figure_close(cMm fig, cMm force);
Mm DLLI dialog();
Mm DLLI dialog(cMm varargin);
Mm DLLI findobj();
Mm DLLI findobj(cMm varargin);
Mm DLLI findall(cMm varargin);
Mm DLLI findfigs();
Mm DLLI gca();
Mm DLLI gcbo();
Mm DLLI gcbo(i_o_t, Mm& cbo, Mm& fig);
Mm DLLI gcbf();
Mm DLLI ishandle(cMm h);
Mm DLLI ishghandle(cMm h);
Mm DLLI gcf();
Mm DLLI deleteobj(cMm h);
Mm DLLI gco();
Mm DLLI gco(cMm fig);
Mm DLLI capture();
Mm DLLI capture(i_o_t, Mm& image__o, Mm& map__o);
Mm DLLI capture(cMm fig);
Mm DLLI capture(cMm fig, i_o_t, Mm& image__o, Mm& map__o);
Mm DLLI getframe();
Mm DLLI getframe(i_o_t, Mm& image__o, Mm& map__o);
Mm DLLI getframe(cMm fig);
Mm DLLI getframe(cMm fig, i_o_t, Mm& image__o, Mm& map__o);
Mm DLLI get(cMm h);
Mm DLLI get(cMm h,cMm name);
Mm DLLI get(cMm h,const char* name);
Mm DLLI grid();
Mm DLLI grid(cMm g);
Mm DLLI waitforbuttonpress();
Mm DLLI checkforbuttonpress();
Mm DLLI waitfor(cMm h);
Mm DLLI waitfor(cMm h, cMm);
Mm DLLI waitfor(cMm h, cMm, cMm);
Mm DLLI ginput(cMm numpoints, i_o_t, Mm& x, Mm& y, Mm& buttons);
Mm DLLI ginput(cMm numpoints, i_o_t, Mm& x, Mm& y);
Mm DLLI ginput(cMm numpoints);
Mm DLLI ginput(i_o_t, Mm& x, Mm& y, Mm& buttons);
Mm DLLI ginput(i_o_t, Mm& x, Mm& y);
Mm DLLI ginput();
Mm DLLI gtext(cMm str);
Mm DLLI getpts(cMm fig, i_o_t, Mm& x, Mm& y);
Mm DLLI getpts(i_o_t, Mm& x, Mm& y);
Mm DLLI hold();
Mm DLLI hold(cMm g);
Mm DLLI whitebg();
Mm DLLI whitebg(cMm fig);
Mm DLLI whitebg(cMm fig, cMm c);
Mm DLLI ishold();
Mm DLLI patch();
Mm DLLI patch(cMm varargin);
Mm DLLI rectangle(cMm varargin);
Mm DLLI fill(cMm varargin);
Mm DLLI line();
Mm DLLI line(cMm varargin);
Mm DLLI surface();
Mm DLLI surface(cMm varargin);
Mm DLLI spy(cMm a);
Mm DLLI spy(cMm a, cMm linetype);
Mm DLLI newplot();
Mm DLLI print();
Mm DLLI print(cMm varargin);
Mm DLLI orient();
Mm DLLI orient(cMm pos);
Mm DLLI orient(cMm fig, cMm pos);
Mm DLLI zoom();
Mm DLLI zoom(cMm a);
Mm DLLI zoom(cMm fig, cMm a);
Mm DLLI rotate3d();
Mm DLLI rotate3d(cMm a);
Mm DLLI box(cMm a);
Mm DLLI refresh();
Mm DLLI refresh(cMm fig);
Mm DLLI set(cMm h);
Mm DLLI set(cMm h,cMm varargin);
Mm DLLI set(cMm h,cMm name,cMm value);
Mm DLLI setuprop(cMm h, cMm name, cMm value);
Mm DLLI getuprop(cMm h, cMm name);
Mm DLLI clruprop(cMm h, cMm name);
Mm DLLI shg();
Mm DLLI subplot(cMm m,cMm n,cMm p);
Mm DLLI subplot(cMm SPos, cMm pos);
Mm DLLI subplot(cMm mnp);
Mm DLLI subplot();
Mm DLLI text();
Mm DLLI text(cMm varargin);
Mm DLLI xlabel();
Mm DLLI xlabel(cMm varargin);
Mm DLLI ylabel();
Mm DLLI ylabel(cMm varargin);
Mm DLLI zlabel();
Mm DLLI zlabel(cMm varargin);
Mm DLLI title();
Mm DLLI title(cMm varargin);
Mm DLLI legend();
Mm DLLI legend(cMm varargin);
Mm DLLI datetick();
Mm DLLI datetick(cMm taxis);
Mm DLLI datetick(cMm taxis, cMm format);

Mm DLLI area(cMm varargin);
Mm DLLI bar(cMm a1, cMm a2, cMm a3);
Mm DLLI bar(cMm a1, cMm a2);
Mm DLLI bar(cMm y);
Mm DLLI barh(cMm a1, cMm a2, cMm a3);
Mm DLLI barh(cMm a1, cMm a2);
Mm DLLI barh(cMm y);
Mm DLLI errorbar(cMm y);
Mm DLLI errorbar(cMm y, cMm l);
Mm DLLI errorbar(cMm x, cMm y, cMm l);
Mm DLLI errorbar(cMm x, cMm y, cMm l, cMm u);
Mm DLLI errorbar(cMm x, cMm y, cMm l, cMm u, cMm s);
Mm DLLI pie(cMm varargin);
Mm DLLI plot(cMm varargin);
Mm DLLI scatter(cMm x);
Mm DLLI scatter(cMm x, cMm y);
Mm DLLI scatter(cMm x, cMm y, cMm varargin);
Mm DLLI scatter3(cMm x);
Mm DLLI scatter3(cMm x, cMm y);
Mm DLLI scatter3(cMm x, cMm y, cMm z);
Mm DLLI scatter3(cMm x, cMm y, cMm z, cMm varargin);
Mm DLLI polar(cMm varargin);
Mm DLLI quiver(cMm uin);
Mm DLLI quiver(cMm uin, cMm vin);
Mm DLLI quiver(cMm uin, cMm vin, cMm si);
Mm DLLI quiver(cMm xin, cMm yin, cMm uin, cMm vin);
Mm DLLI quiver(cMm xin, cMm yin, cMm uin, cMm vin, cMm si);
Mm DLLI feather(cMm x);
Mm DLLI feather(cMm u,cMm v);
Mm DLLI feather(cMm u, cMm v, cMm s);
Mm DLLI plotyy(cMm x1);
Mm DLLI plotyy(cMm x1, cMm y1);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2, cMm y2);
Mm DLLI plotyy(cMm x1, i_o_t, Mm& ax, Mm& h1);
Mm DLLI plotyy(cMm x1, cMm y1, i_o_t, Mm& ax, Mm& h1);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2, i_o_t, Mm& ax, Mm& h1);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2, cMm y2, i_o_t, Mm& ax, Mm& h1);
Mm DLLI plotyy(cMm x1, i_o_t, Mm& ax, Mm& h1, Mm& h2);
Mm DLLI plotyy(cMm x1, cMm y1, i_o_t, Mm& ax, Mm& h1, Mm& h2);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2, i_o_t, Mm& ax, Mm& h1, Mm& h2);
Mm DLLI plotyy(cMm x1, cMm y1, cMm x2, cMm y2, i_o_t, Mm& ax, Mm& h1, Mm& h2);
Mm DLLI loglog(cMm varargin);
Mm DLLI semilogx(cMm varargin);
Mm DLLI semilogy(cMm varargin);
Mm DLLI stairs(cMm varargin);
Mm DLLI stairs(cMm varargin, i_o_t, Mm& xout, Mm& yout);
Mm DLLI stem(cMm varargin);
Mm DLLI hist(cMm y);
Mm DLLI hist(cMm y, cMm m);
Mm DLLI hist(cMm y, i_o_t, Mm& xn, Mm& x);
Mm DLLI hist(cMm y, cMm m, i_o_t, Mm& xn, Mm& x);
Mm DLLI rose(cMm y);
Mm DLLI rose(cMm y, cMm m);
Mm DLLI rose(cMm y, i_o_t, Mm &theta, Mm &rad);
Mm DLLI rose(cMm y, cMm m, i_o_t, Mm &theta, Mm &rad);
Mm DLLI compass(cMm z);
Mm DLLI compass(cMm u,cMm v);
Mm DLLI compass(cMm u,cMm v,cMm s);
Mm DLLI ribbon(cMm y);
Mm DLLI ribbon(cMm x, cMm y);
Mm DLLI ribbon(cMm x, cMm y, cMm w);

Mm DLLI bar3(cMm y);
Mm DLLI bar3(cMm y, cMm z);
Mm DLLI bar3(cMm x, cMm y, cMm z);
Mm DLLI bar3h(cMm y);
Mm DLLI bar3h(cMm y, cMm z);
Mm DLLI bar3h(cMm x, cMm y, cMm z);
Mm DLLI contour(cMm varargin);
Mm DLLI contour(cMm varargin,i_o_t, Mm& cmatrix, Mm& hmatrix);
Mm DLLI contour3(cMm varargin);
Mm DLLI contour3(cMm varargin,i_o_t, Mm& cmatrix, Mm& hmatrix);
Mm DLLI contourf(cMm varargin);
Mm DLLI contourf(cMm varargin,i_o_t, Mm& cmatrix, Mm& hmatrix);
Mm DLLI clabel(cMm cmatrix);
Mm DLLI clabel(cMm cmatrix, cMm h);
Mm DLLI clabel(cMm cmatrix, cMm h, cMm v);
Mm DLLI mesh(cMm varargin);
Mm DLLI meshc(cMm varargin);
Mm DLLI pcolor(cMm varargin);
Mm DLLI pie3(cMm varargin);
Mm DLLI plot3(cMm varargin);
Mm DLLI surf(cMm varargin);
Mm DLLI surfc(cMm varargin);
Mm DLLI surfl(cMm varargin);
Mm DLLI view();
Mm DLLI view(cMm a1);
Mm DLLI view(cMm az, cMm el);
Mm DLLI view(i_o_t, Mm& az, Mm& el);

Mm DLLI autumn();
Mm DLLI autumn(cMm m);
Mm DLLI black();
Mm DLLI black(cMm m);
Mm DLLI bone();
Mm DLLI bone(cMm m);
Mm DLLI caxis();
Mm DLLI caxis(cMm m);
Mm DLLI caxis(i_o_t, Mm& cmin, Mm& cmax);
Mm DLLI caxis(cMm m, i_o_t, Mm& cmin, Mm& cmax);
Mm DLLI colordef(cMm a1);
Mm DLLI colordef(cMm a1, cMm a2);
Mm DLLI colorbar();
Mm DLLI rgbplot(cMm map);
Mm DLLI colorbar(cMm h);
Mm DLLI colormap();
Mm DLLI colormap(cMm map);
Mm DLLI contrast(cMm im);
Mm DLLI contrast(cMm im, cMm l);
Mm DLLI cool();
Mm DLLI cool(cMm m);
Mm DLLI copper();
Mm DLLI copper(cMm m);
Mm DLLI flag();
Mm DLLI flag(cMm m);
Mm DLLI gray();
Mm DLLI gray(cMm m);
Mm DLLI grey();
Mm DLLI grey(cMm m);
Mm DLLI hot();
Mm DLLI hot(cMm m);
Mm DLLI hsv();
Mm DLLI hsv(cMm m);
Mm DLLI im2double(cMm data);
Mm DLLI im2double(cMm data, cMm indexed);
Mm DLLI image(cMm varargin);
Mm DLLI imagesc(cMm varargin);
Mm DLLI getimage();
Mm DLLI getimage(cMm h);
Mm DLLI getimage(i_o_t, Mm& xdata,Mm& ydata);
Mm DLLI getimage(cMm h,i_o_t, Mm& xdata,Mm& ydata);
Mm DLLI getimage(i_o_t, Mm& xdata,Mm& ydata,Mm& A);
Mm DLLI getimage(cMm h,i_o_t, Mm& xdata,Mm& ydata,Mm& A);
Mm DLLI getimage(i_o_t, Mm& xdata,Mm& ydata,Mm& A, Mm& typ);
Mm DLLI getimage(cMm h,i_o_t, Mm& xdata,Mm& ydata,Mm& A, Mm& typ);
Mm DLLI imshow(cMm a);
Mm DLLI subimage(cMm a);
Mm DLLI truesize();
Mm DLLI truesize(cMm fig);
Mm DLLI truesize(cMm fig, cMm sz);
Mm DLLI imread(cMm filename);
Mm DLLI imread(cMm filename, cMm fmt);
Mm DLLI imread(cMm filename, cMm fmt, i_o_t, Mm& cdata, Mm& map);
Mm DLLI imread(cMm filename, i_o_t, Mm& cdata, Mm& map);
Mm DLLI imwrite(cMm varargin);
Mm DLLI imfinfo(cMm filename);
Mm DLLI imfinfo(cMm filename, cMm fmt);
Mm DLLI bmpwrite(cMm varargin);
Mm DLLI bmpread(cMm filename);
Mm DLLI bmpread(cMm filename, i_o_t, Mm& cdata, Mm& map);
Mm DLLI tiffwrite(cMm varargin);
Mm DLLI tiffread(cMm filename);
Mm DLLI tiffread(cMm filename, i_o_t, Mm& cdata, Mm& map);
Mm DLLI pcxwrite(cMm varargin);
Mm DLLI pcxread(cMm filename);
Mm DLLI pcxread(cMm filename, i_o_t, Mm& cdata, Mm& map);
Mm DLLI gifwrite(cMm varargin);
Mm DLLI gifread(cMm filename);
Mm DLLI gifread(cMm filename, i_o_t, Mm& cdata, Mm& map);
Mm DLLI jet();
Mm DLLI jet(cMm m);
Mm DLLI lines();
Mm DLLI lines(cMm m);
Mm DLLI pink();
Mm DLLI pink(cMm m);
Mm DLLI prism();
Mm DLLI prism(cMm m);
Mm DLLI shading(cMm l);
Mm DLLI spring();
Mm DLLI spring(cMm m);
Mm DLLI summer();
Mm DLLI summer(cMm m);
Mm DLLI white();
Mm DLLI white(cMm m);
Mm DLLI winter();
Mm DLLI winter(cMm m);

Mm DLLI errordlg(cMm varargin);
Mm DLLI helpdlg(cMm varargin);
Mm DLLI inputdlg(cMm varargin);
Mm DLLI menu(cMm varargin);
Mm DLLI msgbox(cMm varargin);
Mm DLLI helpwin(cMm str);
Mm DLLI helpwin(cMm str, cMm tit);
Mm DLLI helpwin(cMm str, cMm topic, cMm tit);
Mm DLLI printdlg(cMm varargin);
Mm DLLI questdlg(cMm varargin);
Mm DLLI textwrap(cMm , cMm ins);
Mm DLLI uicontrol();
Mm DLLI uicontrol(cMm varargin);
Mm DLLI hwcontrol();
Mm DLLI hwcontrol(cMm varargin);
Mm DLLI uigetfile();
Mm DLLI uigetfile(i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uigetfile(cMm initname);
Mm DLLI uigetfile(cMm initname, cMm inittitle);
Mm DLLI uigetfile(cMm initname, cMm inittitle, cMm x);
Mm DLLI uigetfile(cMm initname, cMm inittitle, cMm x, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uigetfile(cMm initname, cMm inittitle, cMm x, cMm y);
Mm DLLI uigetfile(cMm initname, cMm inittitle, cMm x, cMm y, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uigetfile(cMm initname, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uigetfile(cMm initname, cMm inittitle, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uimenu();
Mm DLLI uimenu(cMm varargin);
Mm DLLI makemenu(cMm varargin);
Mm DLLI uicontextmenu();
Mm DLLI uicontextmenu(cMm varargin);
Mm DLLI popupstr(cMm h);
Mm DLLI uiputfile();
Mm DLLI uiputfile(i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uiputfile(cMm initname);
Mm DLLI uiputfile(cMm initname, cMm inittitle);
Mm DLLI uiputfile(cMm initname, cMm inittitle, cMm x);
Mm DLLI uiputfile(cMm initname, cMm inittitle, cMm x, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uiputfile(cMm initname, cMm inittitle, cMm x, cMm y);
Mm DLLI uiputfile(cMm initname, cMm inittitle, cMm x, cMm y, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uiputfile(cMm initname, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uiputfile(cMm initname, cMm inittitle, i_o_t, Mm& filename__o, Mm& pathname__o);
Mm DLLI uisetcolor(cMm h);
Mm DLLI uisetcolor(cMm h, cMm inittitle);
Mm DLLI uisetfont(cMm h);
Mm DLLI uisetfont(cMm h, cMm inittitle);
Mm DLLI uiwait(cMm fig);
Mm DLLI uiwait();
Mm DLLI uiresume(cMm fig);
Mm DLLI uiresume();
Mm DLLI umtoggle(cMm h);
Mm DLLI waitbar(cMm );
Mm DLLI waitbar(cMm x, cMm t);
Mm DLLI warndlg(cMm varargin);
Mm DLLI watchoff(cMm fig);
Mm DLLI watchoff();
Mm DLLI watchon();


STUBH(frame2im)
STUBH(im2frame)
STUBH(movie)
STUBH(spinmap)
STUBH(contourc)
STUBH(doc)
STUBH(getline)
STUBH_2_2(getline)
STUBH(getrect)
STUBH(helpfun)
STUBH(hthelp)
STUBH(htpp)
STUBH(light)
STUBH(lightangle)
STUBH(lighting)
STUBH(loadhtml)
STUBH(material)
STUBH(uirestore)
STUBH(uisuspend)
STUBH(web)
STUBH(rbbox)
STUBH(dragrect)
STUBH(copyobj)
STUBH(selectmoveresize)
STUBH(setptr)
STUBH(struct2handle)
STUBH(handle2struct)
STUBH(hardcopy)
STUBH(pagedlg)
STUBH(ctlpanel)

#define mget get
#define mset set
#define vbrefresh refresh 
#define mline line
#define mprint print

const char* surename(cMm x);
double DLLI error_input(const char* func, int n);
void DLLI validate_narg(const char* funcname, int nlhs, int nrhs, int nlhs_min, int nlhs_max, int nrhs_min, int nrhs_max);
int DLLI error_square(cMm x, const char* funcname);
int DLLI error_2d(cMm x, const char* funcname);
int DLLI error_sparse(cMm x, const char* funcname);
int DLLI error_struct(cMm x, const char* funcname);
int DLLI error_numeric(cMm x, const char* funcname);

int DLLI isieee();
int DLLI isstudent();
int DLLI is_square(cMm x);
int DLLI is_struct(cMm x);
int DLLI isobject(cMm x);
int DLLI isstruct(cMm x);
int DLLI isempty(cMm x);
int DLLI is_scalar(cMm x);
int DLLI isscalar(cMm x);
int DLLI isnegative(m_type* pr, int s);
int DLLI islogical(cMm x);
int DLLI iscomplex(m_type* pi, int s);
int DLLI iscomplex(cMm x);
int DLLI isreal(cMm x);
int DLLI isint(cMm x);
int DLLI isnumeric(cMm x);
#define  ischar isstr
int DLLI iscell(cMm x);
int DLLI isstr(cMm x);
int DLLI istrue(cMm x);
int DLLI istrue(m_type x);
int DLLI isdouble(cMm x);
int DLLI isuint8(cMm x);
int DLLI is_vector(cMm x);
int DLLI isvector(cMm x);
int DLLI isglobal(cMm);
int DLLI issparse(cMm);
int DLLI is_same_dims(cMm x, cMm y);
int DLLI is_same_dims(const int xdims[], const int ydims[]);

int DLLI ishermitian(cMm x);

Mm DLLI axxbc(Mm a);
Mm DLLI axxbc(Mm a, Mm b);
Mm DLLI axxbc(Mm a, Mm b, Mm c);
Mm DLLI balance(cMm a);
Mm DLLI balance(Mm a, i_o_t, Mm& t, Mm& b);
Mm DLLI cdf2rdf(cMm v);
Mm DLLI cdf2rdf(cMm v, cMm d);
Mm DLLI cdf2rdf(cMm v, cMm d, i_o_t, Mm& vv, Mm& dd);
Mm DLLI cdf2rdf(cMm v, i_o_t, Mm& vv, Mm& dd);
Mm DLLI chol(Mm b);
Mm DLLI chol(Mm x, i_o_t, Mm& R, Mm& p);
Mm DLLI cond(cMm x);
Mm DLLI condeig(cMm x);
Mm DLLI condeig(cMm x, i_o_t, Mm& v, Mm& d);
Mm DLLI condeig(cMm x, i_o_t, Mm& v, Mm& d, Mm& s);
Mm DLLI det(Mm x);
Mm DLLI dongarra(Mm x);
Mm DLLI eig(cMm a, cMm b);
Mm DLLI eig(cMm a, cMm b, i_o_t, Mm& v, Mm& d);
Mm DLLI eig(Mm x);
Mm DLLI eig(Mm x, i_o_t, Mm& v, Mm& d);
Mm DLLI eig_D(cMm x);
Mm DLLI eig_W(cMm x);
Mm DLLI expm(Mm A);
Mm DLLI givens(cMm a);
Mm DLLI givens(cMm a, cMm b);
Mm DLLI givens(cMm a, cMm b, i_o_t, Mm& c, Mm& s);
Mm DLLI givens(cMm a, i_o_t, Mm& c, Mm& s);
Mm DLLI hess(Mm a);
Mm DLLI hess(Mm a, i_o_t, Mm& p, Mm& h);
Mm DLLI inv(Mm x);
Mm DLLI invdiv(cMm a);
Mm DLLI invdiv(cMm a, cMm b);
Mm DLLI logm(cMm x);
Mm DLLI logm(cMm x, i_o_t, Mm& y, Mm& e);
Mm DLLI lu(Mm x);
Mm DLLI lu(Mm x, i_o_t, Mm& l, Mm& u);
Mm DLLI lu(Mm x, i_o_t, Mm& l, Mm& u, Mm& p);
Mm DLLI lu_L(cMm x);
Mm DLLI lu_U(cMm x);
Mm DLLI lyap(cMm a);
Mm DLLI lyap(cMm a, cMm b, cMm c);
Mm DLLI lyap(cMm a, cMm c);
Mm DLLI mldivide(cMm a);
Mm DLLI mldivide(cMm a, Mm b);
Mm DLLI mpower(cMm x);
Mm DLLI mpower(cMm x,Mm y);
Mm DLLI mrdivide(cMm x);
Mm DLLI mrdivide(cMm x, cMm y);
Mm DLLI norm(cMm x);
Mm DLLI norm(cMm x, cMm P);
Mm DLLI null(cMm x);
Mm DLLI orth(cMm x);
Mm DLLI pinv(cMm x);
Mm DLLI pinv(cMm x, cMm tol);
Mm DLLI planerot(cMm x);
Mm DLLI planerot(cMm x, i_o_t,  Mm& G, Mm& Y);
Mm DLLI qr(Mm x);
Mm DLLI qr(cMm x, cMm e);
Mm DLLI qr(Mm x, Mm e, i_o_t, Mm& q, Mm& r);
Mm DLLI qr(Mm x, Mm f, i_o_t, Mm& q, Mm& r, Mm& e);
Mm DLLI qr(Mm x, i_o_t, Mm& q, Mm& r);
Mm DLLI qr(Mm x, i_o_t, Mm& q, Mm& r, Mm& e);
Mm DLLI qrdelete(cMm q, cMm r, cMm j, i_o_t, Mm& qq, Mm& rr);
Mm DLLI qrinsert(cMm q, cMm r, cMm j, cMm x, i_o_t, Mm& qq, Mm& rr);
Mm DLLI qz(cMm a, cMm b);
Mm DLLI qz(cMm a, cMm b, i_o_t, Mm& aa, Mm& bb);
Mm DLLI qz(cMm a, cMm b, i_o_t, Mm& aa, Mm& bb, Mm& q);
Mm DLLI qz(cMm a, cMm b, i_o_t, Mm& aa, Mm& bb, Mm& q, Mm& z);
Mm DLLI qz(cMm a, cMm b, i_o_t, Mm& aa, Mm& bb, Mm& q, Mm& z, Mm& v);
Mm DLLI qz(cMm a);
Mm DLLI qz(cMm a, i_o_t, Mm& aa, Mm& bb);
Mm DLLI qz(cMm a, i_o_t, Mm& aa, Mm& bb, Mm& q);
Mm DLLI qz(cMm a, i_o_t, Mm& aa, Mm& bb, Mm& q, Mm& z);
Mm DLLI qz(cMm a, i_o_t, Mm& aa, Mm& bb, Mm& q, Mm& z, Mm& v);
Mm DLLI qzval(cMm a);
Mm DLLI qzval(cMm a, cMm b);
Mm DLLI rank(cMm x);
Mm DLLI rank(cMm x, cMm tol);
Mm DLLI rcond(Mm x);
Mm DLLI rref(cMm A);
Mm DLLI rref(cMm A, cMm tol);
Mm DLLI rref(cMm A, cMm tol_in, i_o_t, Mm& A_o, Mm& jb_o);
Mm DLLI rref(cMm A, i_o_t, Mm& A_o, Mm& jb_o);
Mm DLLI rsf2csf(Mm u, Mm t, i_o_t, Mm& U_out, Mm& T_out);
Mm DLLI schur(cMm x);
Mm DLLI schur(Mm x, i_o_t, Mm& u, Mm& t);
Mm DLLI schur_U(cMm x);
Mm DLLI schur_T(cMm x);
Mm DLLI sqrtm(cMm x);
Mm DLLI sqrtm(cMm x, i_o_t, Mm& S, Mm& err);
Mm DLLI subspace(cMm x);
Mm DLLI subspace(cMm x, cMm y);
Mm DLLI svd(cMm x);
Mm DLLI svd(cMm x, cMm z);
Mm DLLI svd(cMm x, cMm z, i_o_t, Mm& U, Mm& S);
Mm DLLI svd(cMm x, cMm z, i_o_t, Mm& U, Mm& S, Mm& V);
Mm DLLI svd(cMm x, i_o_t, Mm& U, Mm& S);
Mm DLLI svd(cMm x, i_o_t, Mm& U, Mm& S, Mm& V);
Mm DLLI svd_U(cMm x);
Mm DLLI svd_S(cMm x);
Mm DLLI svd_W(cMm x);
Mm DLLI sylv(cMm a);
Mm DLLI sylv(cMm a, cMm b);
Mm DLLI sylv(cMm a, cMm b, cMm c);
Mm DLLI trace(cMm x);


#define loadV4 load
#define loadascii load

Mm DLLI load(cMm fname, const Mc& varargin);

Mm DLLI saveV4(cMm fname, const Mc& varargin);
Mm DLLI save(cMm fname, const Mc& varargin);
Mm DLLI saveascii(cMm fname, const Mc& varargin);

Mm DLLI vbload(cMm fname);
Mm DLLI vbsave(cMm varargin);

Mm DLLI mbint(cMm x);
Mm DLLI mbintreal(cMm x);
Mm DLLI mbintrealscalar(cMm x);
Mm DLLI mbintrealvector(cMm x);
Mm DLLI mbintscalar(cMm x);
Mm DLLI mbintvector(cMm x);
Mm DLLI mbrealvector(cMm x);
Mm DLLI mbscalar(cMm x);
Mm DLLI mbvector(cMm x);
Mm DLLI inbounds();
Mm DLLI ivdep();
Mm DLLI realonly();

void DLLI call_mex(const char* mex_name, m_type nlhs, Mm* plhs[], m_type nrhs, Mm* prhs[]);
void DLLI clear_mex(const char* mex_name);
void DLLI call_mex_prhs(Mm*& a, cMm b);
void DLLI call_mex_plhs(Mm& a, Mm* b);
#define call_mex_max_arguments 64

#if !defined(MATLAB_MEX_FILE) && !defined(MODEL)

#define mxDOUBLE_CLASS 0
#define mxFLOAT_CLASS  1

typedef Mm mxArray;
typedef m_type mxChar;

enum { mxREAL=0, mxCOMPLEX=1 };

typedef void (*mexFunction_type)(int nlhs, Mm* plhs[], int nrhs, Mm* prhs[]);

extern "C" {
void DLLX mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
}

#define STUB(name)			    \
int DLLI name();

typedef void (*mexAtExit_function)(void);

extern "C" {

void DLLX mexPrintAssertion(const char* testname, const char* filename, int line, const char* msg);
void DLLX mexAddFlops(int );
int DLLX mexAtExit(mexAtExit_function f);
int DLLX mexCallMATLAB(int nlhs, const Mm* plhs[], int nrhs, const Mm* prhs[], const char* func);
void DLLX mexErrMsgTxt(const char* s);
STUB(mexEvalString)
STUB(mexGet)
STUB(mexGetArray)
STUB(mexGetArrayPtr)
m_type DLLX mexGetEps();
STUB(mexGetFull)
STUB(mexGetGlobal)
m_type DLLX mexGetInf();
STUB(mexGetMatrix)
STUB(mexGetMatrixPtr)
m_type DLLX mexGetNaN();
int DLLX mexIsFinite(m_type x);
STUB(mexIsGlobal)
int DLLX mexIsInf(m_type x);
STUB(mexIsLocked)
int DLLX mexIsNaN(m_type x);
STUB(mexLock)
STUB(mexMakeArrayPersistent)
void DLLX mexMakeMemoryPersistent(void* s);
int DLLX mexPrintf(const char* s,...);
STUB(mexPutArray)
STUB(mexPutFull)
STUB(mexPutMatrix)
STUB(mexSet)
STUB(mexSetTrapFlag)
STUB(mexUnlock)
void DLLX mexWarnMsgTxt(const char* s);
char PDLLX mxArrayToString(Mm* x);
void DLLX mxAssert(int e, char* s);
void DLLX mxAssertS(int e, char* s);
int DLLX mxCalcSingleSubscript(const Mm* x, int ndims, int* dims);
void PDLLX mxCalloc(size_t n, size_t sz);
Mm PDLLX mxCreateCellArray(int ndims, const int* dims);
Mm PDLLX mxCreateCellMatrix(int rows, int cols);
Mm PDLLX mxCreateCharArray(int ndims, const int* dims);
STUB(mxCreateCharMatrixFromStrings)
Mm PDLLX mxCreateDoubleMatrix(int rows, int cols, int isc);
Mm PDLLX mxCreateFull(int rows, int cols, int isc);
Mm PDLLX mxCreateNumericArray(int new_ndims, const int* new_dims, int cls, int isc);
Mm PDLLX mxCreateSparse(int rows, int cols, int nz, int isc);
Mm PDLLX mxCreateString(const char* s);
Mm PDLLX mxCreateStructArray(int ndims, const int* dims, int nf, const char** fields);
Mm PDLLX mxCreateStructMatrix(int rows, int cols, int nf, const char** fields);
void DLLX mxDestroyArray(Mm* x);
Mm PDLLX mxDuplicateArray(const Mm* x);
void DLLX mxFree(void* p);
void DLLX mxFreeMatrix(Mm* x);
Mm PDLLX mxGetCell(const Mm* x, int i);
int DLLX mxGetClassID(const Mm* x);
const char PDLLX mxGetClassName(const Mm *x);
m_type PDLLX mxGetData(const Mm* x);
const int PDLLX mxGetDimensions(const Mm* x);
int DLLX mxGetElementSize(const Mm* x);
m_type DLLX mxGetEps();
Mm PDLLX mxGetField(const Mm* x, int i, const char* field);
Mm PDLLX mxGetFieldByNumber(const Mm* x, int i, int field);
const char PDLLX mxGetFieldNameByNumber(const Mm* x, int i);
int DLLX mxGetFieldNumber(const Mm* x, const char* field);
void DLLX mxGetI(const Mm* x, int row, int col, m_type* y);
m_type PDLLX mxGetImagData(const Mm* x);
m_type DLLX mxGetInf();
int PDLLX mxGetIr(const Mm* x);
int PDLLX mxGetJc(const Mm* x);
int DLLX mxGetM(const Mm* x);
int DLLX mxGetN(const Mm* x);
m_type DLLX mxGetNaN();
const char PDLLX mxGetName(const Mm* x);
int DLLX mxGetNumberOfDimensions(const Mm* x);
int DLLX mxGetNumberOfElements(const Mm* x);
int DLLX mxGetNumberOfFields(const Mm* x);
int DLLX mxGetNzmax(const Mm* x);
m_type PDLLX mxGetPi(const Mm* x);
m_type PDLLX mxGetPr(const Mm* x);
void DLLX mxGetR(const Mm* x, int row, int col, m_type* y);
m_type DLLX mxGetScalar(const Mm* x);
int DLLX mxGetString(const Mm* x, char* s, int s_len);
int DLLX mxIsCell(const Mm* x);
int DLLX mxIsChar(const Mm* x);
int DLLX mxIsClass(const Mm* x, const char* s);
int DLLX mxIsComplex(const Mm* x);
int DLLX mxIsDouble(const Mm* x);
int DLLX mxIsEmpty(const Mm* x);
int DLLX mxIsFinite(m_type x);
STUB(mxIsFromGlobalWS)
int DLLX mxIsFull(const Mm* x);
int DLLX mxIsInf(m_type x);
int DLLX mxIsInt16(const Mm* x);
int DLLX mxIsInt32(const Mm* x);
int DLLX mxIsInt8(const Mm* x);
int DLLX mxIsLogical(const Mm* x);
int DLLX mxIsNaN(m_type x);
int DLLX mxIsNumeric(const Mm* x);
int DLLX mxIsSingle(const Mm* x);
int DLLX mxIsSparse(const Mm* x);
int DLLX mxIsString(const Mm* x);
int DLLX mxIsStruct(const Mm* x);
int DLLX mxIsUint16(const Mm* x);
int DLLX mxIsUint32(const Mm* x);
int DLLX mxIsUint8(const Mm* x);
void PDLLX mxMalloc(size_t sz);
void PDLLX mxRealloc(void* p, size_t sz);
STUB(mxSetAllocFcns)
void DLLX mxSetCell(Mm* x, int i, Mm* rhs);
STUB(mxSetClassName)
STUB(mxSetData)
int DLLX mxSetDimensions(Mm* x, const int* new_dims, int new_ndims);
void DLLX mxSetField(Mm* x, int i, const char* field, Mm* rhs);
void DLLX mxSetFieldByNumber(Mm* x, int i, int field_no, Mm *rhs);
void DLLX mxSetI(const Mm* x, int row, int col, m_type* y);
STUB(mxSetImagData)
STUB(mxSetIr)
STUB(mxSetJc)
void DLLX mxSetLogical(Mm* x);
STUB(mxSetM)
STUB(mxSetN)
void DLLX mxSetName(Mm* x, const char* s);
STUB(mxSetNzmax)
STUB(mxSetPi)
STUB(mxSetPr)
void DLLX mxSetR(Mm* x, int row, int col, m_type* y);
void DLLX mxSetString(Mm* x);

} // extern "C"

#undef STUB

#endif // MATLAB_MEX_FILE


Mm DLLI cauchy(Mm x);
Mm DLLX cauchy(Mm x, Mm y);
Mm DLLI compan(cMm x);
Mm DLLI gallery(Mm n);
Mm DLLI hadamard(Mm n);
Mm DLLI hankel(Mm c);
Mm DLLI hankel(Mm c, Mm r);
Mm DLLI hilb(Mm n);
Mm DLLI invhilb(Mm n);
Mm DLLI magic(Mm n);
Mm DLLI pascalM(cMm n);
Mm DLLI pascalM(Mm n, Mm r);
Mm DLLI rosser();
Mm DLLI toeplitz(Mm c);
Mm DLLI toeplitz(Mm c, Mm r);
Mm DLLI vander(Mm x);
Mm DLLI wilkinson(cMm n);


Mm DLLI abcdchk(cMm a);
Mm DLLI abcdchk(cMm a, cMm b);
Mm DLLI abcdchk(cMm a, cMm b, i_o_t, Mm& msg, Mm& A, Mm& B);
Mm DLLI abcdchk(cMm a, cMm b, cMm c);
Mm DLLI abcdchk(cMm a, cMm b, cMm c, cMm d);
Mm DLLI abcdchk(cMm a, cMm b, cMm c, cMm d, i_o_t, Mm& msg, Mm& A, Mm& B, Mm& C, Mm& D);
Mm DLLI all();
Mm DLLI argn(i_o_t, Mm& lhs, Mm& rhs);
Mm DLLI bitmax();
Mm DLLI computer(i_o_t, Mm& c, Mm& maxsize);
Mm DLLI computer();
Mm DLLI deal(cMm varargin);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4, Mm& a5);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4, Mm& a5, Mm& a6);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4, Mm& a5, Mm& a6, Mm& a7);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4, Mm& a5, Mm& a6, Mm& a7, Mm& a8);
Mm DLLI deal(cMm varargin, i_o_t, Mm& a0, Mm& a1, Mm& a2, Mm& a3, Mm& a4, Mm& a5, Mm& a6, Mm& a7, Mm& a8, Mm& a9);
Mm DLLI deepcopy(cMm x);
Mm DLLI echo(cMm );
Mm DLLI eval(cMm x);
Mm DLLI eval(cMm x, cMm y);
Mm DLLI eval(cMm x ,i_o_t, Mm& r0, Mm& r1);
Mm DLLI evalin(cMm ws);
Mm DLLI evalin(cMm ws, cMm x);
Mm DLLI evalin(cMm ws, cMm x, cMm y);
int  DLLI forsize(Mm& x);
void DLLI forelem(Mm& var, Mm& x, int index);
Mm DLLI flops(cMm x);
Mm DLLI flops();
Mm DLLI getenv(cMm x);
Mm DLLI inputname(cMm x);
int DLLI isequal(cMm a1);
int DLLI isequal(cMm a1, cMm a2);
int DLLI isequal(cMm a1, cMm a2, cMm varargin);
Mm DLLI lasterr();
Mm DLLI lasterr(cMm err);
Mm DLLI license();
Mm DLLI license(cMm x);
Mm DLLI linspace(cMm x);
Mm DLLI linspace(cMm x1, cMm x2);
Mm DLLI linspace(cMm x1, cMm x2, cMm n);
Mm DLLI logspace(cMm x);
Mm DLLI logspace(cMm x1, cMm x2);
Mm DLLI logspace(cMm x1, cMm x2, cMm n);
Mm DLLI mcomplex(cMm x);
Mm DLLI mcomplex(cMm x, cMm y);
Mm DLLI mfileline();
Mm DLLI mfilename();
Mm DLLI addpath(cMm varargin);
Mm DLLI matlabpath();
Mm DLLI matlabpath(cMm x);
Mm DLLI matlabroot();
Mm DLLI M_from_double_array(m_type* d, int rows, int cols);
Mm DLLI path();
Mm DLLI path(cMm x);
Mm DLLI path(cMm x, cMm y);
Mm DLLI peaks();
Mm DLLI peaks(cMm v);
Mm DLLI peaks(cMm x, cMm y);
Mm DLLI peaks(i_o_t, Mm& X, Mm& Y, Mm& Z);
Mm DLLI peaks(cMm v, i_o_t, Mm& X, Mm& Y, Mm& Z);
Mm DLLI peaks(cMm x, cMm y, i_o_t, Mm& X, Mm& Y, Mm& Z);
Mm DLLI rmpath(cMm x);
Mm DLLI more(cMm);
double DLLI nargin();
double DLLI nargout();
Mm DLLI nargchk(cMm low);
Mm DLLI nargchk(cMm low, cMm high);
Mm DLLI nargchk(cMm low, cMm high, cMm number);

STUBH(assignin)
STUBH(close_system)
STUBH(find_system)
STUBH(open_system)
STUBH(get_param)
STUBH(set_param)
STUBH(simset)
STUBH(system_dependent)
STUBH(miedit)

Mm DLLI pwd();
Mm DLLI sim(cMm , cMm , cMm , cMm , i_o_t, Mm&, Mm&, Mm&);
Mm DLLI sim(cMm , cMm , cMm , cMm , cMm , cMm , cMm , cMm , cMm , cMm , cMm , cMm , i_o_t, Mm&, Mm&, Mm&);
Mm DLLI realmax();
Mm DLLI realmin();
Mm DLLI then();
Mm DLLI ver();
Mm DLLI version();
Mm DLLI version(i_o_t, Mm& v, Mm& d);


Mm DLLI constr(cMm func);
Mm DLLI constr(cMm func, cMm x0);
Mm DLLI constr(cMm func, cMm x0, cMm options);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, cMm varargin);
Mm DLLI constr(cMm func, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, cMm options, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, i_o_t, Mm& x, Mm& options_o);
Mm DLLI constr(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, cMm varargin, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func);
Mm DLLI curvefit(cMm func, cMm x0);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options, cMm grad);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options, cMm grad, cMm varargin);
Mm DLLI curvefit(cMm func, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options, cMm grad, i_o_t, Mm& x, Mm& options_o);
Mm DLLI curvefit(cMm func, cMm x0, cMm xdata, cMm ydata, cMm options, cMm grad, cMm varargin, i_o_t, Mm& x, Mm& options_o);
Mm DLLI fmin(cMm func, cMm a, cMm b);
Mm DLLI fmin(cMm func, cMm a, cMm b, cMm options);
Mm DLLI fmin(cMm func, cMm a, cMm b, cMm options, cMm varargin);
Mm DLLI fmin(cMm func, cMm a, cMm b, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fmin(cMm func, cMm a, cMm b, cMm options, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fmin(cMm func, cMm a, cMm b, cMm options, cMm varargin, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminbnd(cMm func, cMm a, cMm b);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options, cMm varargin);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options, cMm varargin, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fminbnd(cMm func, cMm a, cMm b, cMm options, cMm varargin, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fmins(cMm func, cMm x);
Mm DLLI fmins(cMm func, cMm x, cMm options);
Mm DLLI fmins(cMm func, cMm x, cMm options, cMm grad);
Mm DLLI fmins(cMm func, cMm x, cMm options, cMm grad, cMm varargin);
Mm DLLI fmins(cMm func, cMm x, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fmins(cMm func, cMm x, cMm options, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fmins(cMm func, cMm x, cMm options, cMm grad, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fmins(cMm func, cMm x, cMm options, cMm grad, cMm varargin, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminsearch(cMm func, cMm x);
Mm DLLI fminsearch(cMm func, cMm x, cMm options);
Mm DLLI fminsearch(cMm func, cMm x, cMm options, cMm varargin);
Mm DLLI fminsearch(cMm func, cMm x, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminsearch(cMm func, cMm x, cMm options, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminsearch(cMm func, cMm x, cMm options, cMm varargin, i_o_t, Mm& xo, Mm& fval);
Mm DLLI fminsearch(cMm func, cMm x, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fminsearch(cMm func, cMm x, cMm options, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fminsearch(cMm func, cMm x, cMm options, cMm varargin, i_o_t, Mm& xo, Mm& fval, Mm& exitflag);
Mm DLLI fminu(cMm func);
Mm DLLI fminu(cMm func, cMm x);
Mm DLLI fminu(cMm func, cMm x, cMm options);
Mm DLLI fminu(cMm func, cMm x, cMm options, cMm grad);
Mm DLLI fminu(cMm func, cMm x, cMm options, cMm grad, cMm varargin);
Mm DLLI fminu(cMm func, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminu(cMm func, cMm x, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminu(cMm func, cMm x, cMm options, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminu(cMm func, cMm x, cMm options, cMm grad, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fminu(cMm func, cMm x, cMm options, cMm grad, cMm varargin, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI foptions();
Mm DLLI foptions(Mm options);
Mm DLLI fsolve(cMm func);
Mm DLLI fsolve(cMm func, cMm x);
Mm DLLI fsolve(cMm func, cMm x, cMm options);
Mm DLLI fsolve(cMm func, cMm x, cMm options, cMm grad);
Mm DLLI fsolve(cMm func, cMm x, cMm options, cMm grad, cMm varargin);
Mm DLLI fsolve(cMm func, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fsolve(cMm func, cMm x, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fsolve(cMm func, cMm x, cMm options, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fsolve(cMm func, cMm x, cMm options, cMm grad, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fsolve(cMm func, cMm x, cMm options, cMm grad, cMm varargin, i_o_t, Mm& xo, Mm& options_o);
Mm DLLI fzero(cMm func);
Mm DLLI fzero(cMm func, cMm x);
Mm DLLI fzero(cMm func, cMm x, cMm tol);
Mm DLLI fzero(cMm func, cMm x, cMm tol, cMm trace);
Mm DLLI fzero(cMm func, cMm x, cMm tol, cMm trace, cMm varargin);
Mm DLLI optimget(cMm options, cMm n);
Mm DLLI optimget(cMm options, cMm n, cMm def);
Mm DLLI optimset();
Mm DLLI optimset(cMm fun);
Mm DLLI optimset(cMm options, cMm varargin);
Mm DLLI quad8(cMm func);
Mm DLLI quad8(cMm func, cMm a);
Mm DLLI quad8(cMm func, cMm a, cMm b);
Mm DLLI quad8(cMm func, cMm a, cMm b, cMm tol);
Mm DLLI quad8(cMm func, cMm a, cMm b, cMm tol, cMm trace);
Mm DLLI quad8(cMm func, cMm a, cMm b, cMm tol, cMm trace, cMm varargin);
Mm DLLI conls(cMm C);
Mm DLLI conls(cMm C, cMm d);
Mm DLLI conls(cMm C, cMm d, cMm A);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub, cMm x0);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq);
Mm DLLI conls(cMm C, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, i_o_t, Mm& x, Mm& lam);
Mm DLLI conls(cMm C, cMm d, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c);
Mm DLLI lp(cMm c, cMm A);
Mm DLLI lp(cMm c, cMm A, cMm b);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity);
Mm DLLI lp(cMm c, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity, i_o_t, Mm& x, Mm& lam);
Mm DLLI lp(cMm c, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI lp(cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI nnls(cMm A, cMm b);
Mm DLLI nnls(cMm A, cMm b, i_o_t, Mm& x, Mm& w);
Mm DLLI nnls(cMm A, cMm b, i_o_t, Mm& x, Mm& w, Mm& err);
Mm DLLI nnls(cMm A, cMm b, cMm tol);
Mm DLLI nnls(cMm A, cMm b, cMm tol, i_o_t, Mm& x, Mm& w);
Mm DLLI nnls(cMm A, cMm b, cMm tol, i_o_t, Mm& x, Mm& w, Mm& err);
Mm DLLI lsqnonneg(cMm varargin);
Mm DLLI lsqnonneg(cMm varargin, i_o_t, Mm& x, Mm& resnorm);
Mm DLLI lsqnonneg(cMm varargin, i_o_t, Mm& x, Mm& resnorm, Mm& res);
Mm DLLI lsqnonneg(cMm varargin, i_o_t, Mm& x, Mm& resnorm, Mm& res, Mm& flag);
Mm DLLI lsqnonneg(cMm varargin, i_o_t, Mm& x, Mm& resnorm, Mm& res, Mm& flag, Mm& output);
Mm DLLI lsqnonneg(cMm varargin, i_o_t, Mm& x, Mm& resnorm, Mm& res, Mm& flag, Mm& output, Mm& w);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity, i_o_t, Mm& x, Mm& lam);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI qp(cMm Q, cMm c, cMm A, cMm b, cMm vlb, cMm vub, cMm x0, cMm neq, cMm verbosity, i_o_t, Mm& x, Mm& lam, Mm& how);
Mm DLLI minimax(cMm func);
Mm DLLI minimax(cMm func, cMm x0);
Mm DLLI minimax(cMm func, cMm x0, cMm options);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, cMm varargin);
Mm DLLI minimax(cMm func, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, cMm options, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, i_o_t, Mm& x, Mm& options_o);
Mm DLLI minimax(cMm func, cMm x0, cMm options, cMm vlb, cMm vub, cMm grad, cMm varargin, i_o_t, Mm& x, Mm& options_o);

Mm DLLI odeget(cMm options);
Mm DLLI odeget(cMm options, cMm n);
Mm DLLI odeget(cMm options, cMm n, cMm def);
Mm DLLI odeset();
Mm DLLI odeset(cMm n1);
Mm DLLI odeset(cMm n1, cMm v1);
Mm DLLI odeset(cMm opts, cMm n1, cMm v1);
Mm DLLI odeset(cMm n1, cMm v1, cMm n2, cMm v2);
Mm DLLI odeset(cMm opts, cMm n1, cMm v1, cMm n2, cMm v2);
Mm DLLI odeset(cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3);
Mm DLLI odeset(cMm opts, cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3);
Mm DLLI odeset(cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3, cMm n4, cMm v4);
Mm DLLI odeset(cMm opts, cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3, cMm n4, cMm v4);
Mm DLLI odeset(cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3, cMm n4, cMm v4, cMm n5, cMm v5);
Mm DLLI odeset(cMm opts, cMm n1, cMm v1, cMm n2, cMm v2, cMm n3, cMm v3, cMm n4, cMm v4, cMm n5, cMm v5);
Mm DLLI ode23(cMm func);
Mm DLLI ode23(cMm func, cMm tspan);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0, cMm options);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin);
Mm DLLI ode23(cMm func, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode23(cMm func, cMm tspan, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0, cMm options, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode23(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode45(cMm func);
Mm DLLI ode45(cMm func, cMm tspan);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0, cMm options);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin);
Mm DLLI ode45(cMm func, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode45(cMm func, cMm tspan, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0, cMm options, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode45(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode78(cMm func);
Mm DLLI ode78(cMm func, cMm tspan);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0, cMm options);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin);
Mm DLLI ode78(cMm func, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode78(cMm func, cMm tspan, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0, cMm options, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode78(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode15s(cMm func);
Mm DLLI ode15s(cMm func, cMm tspan);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0, cMm options);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin);
Mm DLLI ode15s(cMm func, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode15s(cMm func, cMm tspan, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0, cMm options, i_o_t, Mm& tout, Mm& yout);
Mm DLLI ode15s(cMm func, cMm tspan, cMm y0, cMm options, cMm varargin, i_o_t, Mm& tout, Mm& yout);

#define ode113  ode23
#define ode23s  ode15s
#define leastsq fsolve
#define quad    quad8

Mm DLLI abs(cMm x);
Mm DLLI acos(cMm x);
Mm DLLI acosh(cMm x);
Mm DLLI acot(cMm x);
Mm DLLI acoth(cMm x);
Mm DLLI acsc(cMm x);
Mm DLLI acsch(cMm x);
Mm DLLI all(cMm x);
Mm DLLI all(cMm x, cMm dim1);
Mm DLLI mand(cMm x);
Mm DLLI mand(cMm x, cMm y);
Mm DLLI angle(cMm x);
Mm DLLI any(cMm x);
Mm DLLI any(cMm x, cMm dim1);
Mm DLLI arg(cMm x);
Mm DLLI asec(cMm x);
Mm DLLI asech(cMm x);
Mm DLLI asin(cMm x);
Mm DLLI asinh(cMm x);
Mm DLLI atan(cMm x);
Mm DLLI atan2(cMm x);
Mm DLLI atan2(cMm x,cMm y);
Mm DLLI atanh(cMm x);
Mm DLLI beta(cMm x);
Mm DLLI beta(cMm x,cMm y);
Mm DLLI beta(cMm x, cMm a, cMm b);
Mm DLLI betaln(cMm x);
Mm DLLI betaln(cMm x, cMm y);
Mm DLLI mbitand(cMm x);
Mm DLLI mbitand(cMm x, cMm y);
Mm DLLI bitcmp(cMm x);
Mm DLLI bitcmp(cMm x, cMm y);
Mm DLLI bitget(cMm x);
Mm DLLI bitget(cMm x, cMm y);
Mm DLLI mbitor(cMm x);
Mm DLLI mbitor(cMm x, cMm y);
Mm DLLI bitreset(cMm x);
Mm DLLI bitreset(cMm x, cMm y);
Mm DLLI bitset(cMm x);
Mm DLLI bitset(cMm x, cMm y);
Mm DLLI bitset(cMm x, cMm n, cMm v);
Mm DLLI bitshift(cMm x);
Mm DLLI bitshift(cMm x, cMm y);
Mm DLLI bitxor(cMm x);
Mm DLLI bitxor(cMm x, cMm y);
Mm DLLI ceil(cMm x);
Mm DLLI conj(cMm x);
Mm DLLI cos(cMm x);
Mm DLLI cosh(cMm x);
Mm DLLI cot(cMm x);
Mm DLLI coth(cMm x);
Mm DLLI csc(cMm x);
Mm DLLI csch(cMm x);
Mm DLLI cumprod(cMm x);
Mm DLLI cumprod(cMm x, cMm dim1);
Mm DLLI cumsum(cMm x);
Mm DLLI cumsum(cMm x, cMm dim1);
Mm DLLI del2(cMm f);
Mm DLLI del2(cMm f, cMm hx);
Mm DLLI del2(cMm f, cMm hx, cMm hy);
Mm DLLI del2(cMm f, cMm hx, cMm hy, cMm hz);
Mm DLLI diff(cMm x);
Mm DLLI diff(cMm x, cMm order);
Mm DLLI diff(cMm x, cMm order, cMm dim1);
Mm DLLI dot_add(cMm x, cMm y);
Mm DLLI dot_div(cMm x,cMm y);
Mm DLLI dot_invdiv(cMm x,cMm y);
Mm DLLI dot_mul(cMm x,cMm y);
Mm DLLI dot_pow(cMm x,cMm y);
Mm DLLI dot_sub(cMm x, cMm y);
Mm DLLI eq(cMm x);
Mm DLLI eq(cMm x, cMm y);
Mm DLLI erf(cMm x);
Mm DLLI erfc(cMm x);
Mm DLLI erfcx(cMm x);
Mm DLLI erfinv(cMm x);
Mm DLLI exp(cMm x);
Mm DLLI expint(cMm x);
Mm DLLI finite(cMm x);
Mm DLLI fix(cMm x);
Mm DLLI floor(cMm x);
Mm DLLI gamma(cMm x);
Mm DLLI gammainc(cMm x);
Mm DLLI gammainc(cMm x,cMm y);
Mm DLLI gammaln(cMm x);
Mm DLLI ge(cMm x);
Mm DLLI ge(cMm x, cMm y);
Mm DLLI gt(cMm x);
Mm DLLI gt(cMm x, cMm y);
Mm DLLI gradient(cMm f);
Mm DLLI gradient(cMm f, cMm hx);
Mm DLLI gradient(cMm f, cMm hx, cMm hy);
Mm DLLI gradient(cMm f, cMm hx, cMm hy, cMm hz);
Mm DLLI gradient(cMm f, i_o_t, Mm& fx, Mm& fy);
Mm DLLI gradient(cMm f, cMm hx, i_o_t, Mm& fx, Mm& fy);
Mm DLLI gradient(cMm f, cMm hx, cMm hy, i_o_t, Mm& fx, Mm& fy);
Mm DLLI gradient(cMm f, cMm hx, cMm hy, cMm hz, i_o_t, Mm& fx, Mm& fy);
Mm DLLI gradient(cMm f, i_o_t, Mm& fx, Mm& fy, Mm& fz);
Mm DLLI gradient(cMm f, cMm hx, i_o_t, Mm& fx, Mm& fy, Mm& fz);
Mm DLLI gradient(cMm f, cMm hx, cMm hy, i_o_t, Mm& fx, Mm& fy, Mm& fz);
Mm DLLI gradient(cMm f, cMm hx, cMm hy, cMm hz, i_o_t, Mm& fx, Mm& fy, Mm& fz);
Mm DLLI imag(cMm x);
Mm DLLI isfinite(cMm x);
Mm DLLI isinf(cMm x);
Mm DLLI isnan(cMm x);
Mm DLLI ldivide(cMm x);
Mm DLLI ldivide(cMm x,cMm y);
Mm DLLI le(cMm x);
Mm DLLI le(cMm x, cMm y);
Mm DLLI log(cMm x);
Mm DLLI log10(cMm x);
Mm DLLI log2(cMm x);
Mm DLLI log2(cMm x, i_o_t, Mm& f, Mm& e);
Mm DLLI lt(cMm x);
Mm DLLI lt(cMm x, cMm y);
Mm DLLI max(cMm x);
Mm DLLI max(cMm x, cMm dummy, cMm dim1);
Mm DLLI max(cMm x, i_o_t, Mm& y, Mm& i);
Mm DLLI max(cMm x, cMm dummy, i_o_t, Mm& y, Mm& i);
Mm DLLI max(cMm x, cMm dummy, cMm dim1, i_o_t, Mm& y, Mm& i);
Mm DLLI max(cMm x,cMm y);
Mm DLLI median(cMm a);
Mm DLLI median(cMm a, cMm dim1);
Mm DLLI min(cMm x);
Mm DLLI min(cMm x, cMm dummy, cMm dim1);
Mm DLLI min(cMm x, i_o_t, Mm& y, Mm& i);
Mm DLLI min(cMm x, cMm dummy, i_o_t, Mm& y, Mm& i);
Mm DLLI min(cMm x, cMm dummy, cMm dim1, i_o_t, Mm& y, Mm& i);
Mm DLLI min(cMm x,cMm y);
Mm DLLI minus(cMm x);
Mm DLLI minus(cMm x, cMm y);
Mm DLLI mod(cMm x);
Mm DLLI mod(cMm x,cMm y);
Mm DLLI mt_isnan(cMm x);
Mm DLLI mtimes(cMm x);
Mm DLLI mtimes(cMm x, cMm y);
Mm DLLI ne(cMm x);
Mm DLLI ne(cMm x, cMm y);
Mm DLLI mnot(cMm x);

Mm DLLI operator !=(cMm x, double y);
Mm DLLI operator !=(double x, cMm y);
Mm DLLI operator *(cMm x, double y);
Mm DLLI operator *(double x, cMm y);
Mm DLLI operator /(cMm x, double y);
Mm DLLI operator /(double x, cMm y);
Mm DLLI operator +(cMm x, double y);
Mm DLLI operator +(double x, cMm y);
Mm DLLI operator -(cMm x, double y);
Mm DLLI operator -(double x, cMm y);
Mm DLLI operator <(cMm x, double y);
Mm DLLI operator <(double x, cMm y);
Mm DLLI operator <=(cMm x, double y);
Mm DLLI operator <=(double x, cMm y);
Mm DLLI operator ==(cMm x, double y);
Mm DLLI operator ==(double x, cMm y);
Mm DLLI operator >(cMm x, double y);
Mm DLLI operator >(double x, cMm y);
Mm DLLI operator >=(cMm x, double y);
Mm DLLI operator >=(double x, cMm y);

Mm DLLI operator !(cMm x);
Mm DLLI operator !=(cMm x, cMm y);
Mm DLLI operator &&(cMm x, cMm y);
Mm DLLI operator *(cMm x, cMm y);
Mm DLLI operator /(cMm x, cMm y);
Mm DLLI operator +(cMm x);
Mm DLLI operator +(cMm x, cMm y);
Mm DLLI operator -(cMm x);
Mm DLLI operator -(cMm x, cMm y);
Mm DLLI operator <(cMm x, cMm y);
Mm DLLI operator <=(cMm x, cMm y);
Mm DLLI operator ==(cMm x, cMm y);
Mm DLLI operator >(cMm x, cMm y);
Mm DLLI operator >=(cMm x, cMm y);
Mm DLLI operator ||(cMm x, cMm y);
Mm DLLI mor(cMm x);
Mm DLLI mor(cMm x, cMm y);
Mm DLLI plus(cMm x);
Mm DLLI plus(cMm x, cMm y);
Mm DLLI pow2(cMm x);
Mm DLLI pow2(cMm x, cMm y);
Mm DLLI power(cMm x);
Mm DLLI power(cMm x,cMm y);
Mm DLLI prod(cMm x);
Mm DLLI prod(cMm x, cMm dim1);
Mm DLLI rdivide(cMm x);
Mm DLLI rdivide(cMm x,cMm y);
Mm DLLI real(cMm x);
Mm DLLI reallog(cMm x);
Mm DLLI realpow(cMm x);
Mm DLLI realpow(cMm x, cMm y);
Mm DLLI realsqrt(cMm x);
Mm DLLI rem(cMm x);
Mm DLLI rem(cMm x,cMm y);
Mm DLLI round(cMm x);
Mm DLLI sec(cMm x);
Mm DLLI sech(cMm x);
Mm DLLI sign(cMm x);
Mm DLLI sign(cMm x, cMm y);
Mm DLLI sin(cMm x);
Mm DLLI sinc(cMm x);
Mm DLLI sinh(cMm x);
Mm DLLI msqrt(cMm x);
Mm DLLI sum(cMm x);
Mm DLLI sum(cMm x, cMm dim1);
Mm DLLI sumabs(cMm x);
Mm DLLI sumabs(cMm x, cMm dim1);
Mm DLLI tan(cMm x);
Mm DLLI tanh(cMm x);
Mm DLLI times(cMm x);
Mm DLLI times(cMm x,cMm y);
Mm DLLI tril(cMm x);
Mm DLLI tril(cMm x, cMm y);
Mm DLLI triu(cMm x);
Mm DLLI triu(cMm x, cMm y);
Mm DLLI uminus(cMm x);
Mm DLLI unwrap(cMm x);
Mm DLLI unwrap(cMm x, cMm y);
Mm DLLI unwrap(cMm x, cMm y, cMm dim1);
Mm DLLI uplus(cMm x);
Mm DLLI mxor(cMm x);
Mm DLLI mxor(cMm x, cMm y);

#define mabs abs
#define many any
#define mcos cos
#define mexp exp
#define mfix fix
#define mlog log
#define mmax max
#define mmin min
#define mmod mod
#define mrem rem
#define msin sin
#define mtan tan


int DLLI automesh(cMm x);
int DLLI automesh(cMm x, cMm y);
int DLLI automesh(cMm x, cMm y, cMm z);
Mm DLLI dsearch(cMm x, cMm y, cMm tri, cMm xi, cMm yi);
Mm DLLI delaunay(cMm x);
Mm DLLI delaunay(cMm x, cMm y);
Mm DLLI delaunay(cMm x, cMm y, cMm sorted);
Mm DLLI griddata(cMm x, cMm y, cMm z, cMm xi, cMm yi);
Mm DLLI griddata(cMm x, cMm y, cMm z, cMm xi, cMm yi, cMm method);
Mm DLLI griddata(cMm x, cMm y, cMm z, cMm xi, cMm yi, i_o_t, Mm& XI, Mm& YI, Mm& ZI);
Mm DLLI griddata(Mm x, Mm y, Mm z, Mm xi, Mm yi, cMm method, i_o_t, Mm& XI, Mm& YI, Mm& ZI);
Mm DLLI interp1(cMm y);
Mm DLLI interp1(cMm y, cMm xi);
Mm DLLI interp1(Mm x, Mm y, Mm xi);
Mm DLLI interp1(Mm x, Mm y, Mm xi, cMm method);
Mm DLLI interp1q(cMm x, cMm y, cMm xi);
Mm DLLI interp2(cMm z);
Mm DLLI interp2(cMm z, Mm D);
Mm DLLI interp2(cMm z, Mm xi, Mm yi);
Mm DLLI interp2(cMm z, cMm xi, cMm yi, cMm method);
Mm DLLI interp2(cMm x, cMm y, cMm z, cMm xi, cMm yi);
Mm DLLI interp2(Mm x, Mm y, Mm z, Mm xi, Mm yi, cMm method);
Mm DLLI interp3(cMm v);
Mm DLLI interp3(cMm v, cMm D);
Mm DLLI interp3(cMm v, cMm D, cMm method);
Mm DLLI interp3(cMm v, cMm xi, cMm yi, cMm zi);
Mm DLLI interp3(cMm v, cMm xi, cMm yi, cMm zi, cMm method);
Mm DLLI interp3(cMm v, cMm xi, cMm yi, cMm zi, cMm method, cMm dummy);
Mm DLLI interp3(cMm x, cMm y, cMm z, cMm v, cMm xi, cMm yi, cMm zi);
Mm DLLI interp3(Mm x, Mm y, Mm z, Mm v, Mm xi, Mm yi, Mm zi, cMm method);
Mm DLLI tsearch(cMm x, cMm y, cMm tri, cMm xi, cMm yi);
Mm DLLI mkpp(Mm b);
Mm DLLI mkpp(Mm b, Mm c);
Mm DLLI poly(cMm x);
Mm DLLI polyder(cMm x);
Mm DLLI polyder(cMm x, cMm y);
Mm DLLI polyder(cMm x, i_o_t, Mm& a, Mm& b);
Mm DLLI polyder(cMm x, cMm y, i_o_t, Mm& a, Mm& b);
Mm DLLI polyfit(cMm x);
Mm DLLI polyfit(cMm x, cMm y);
Mm DLLI polyfit(cMm x, cMm y, cMm n);
Mm DLLI polyfit(Mm x, Mm y, cMm n, i_o_t, Mm& p, Mm& s);
Mm DLLI polyval(Mm p);
Mm DLLI polyval(Mm p, Mm x);
Mm DLLI polyvalm(cMm p);
Mm DLLI polyvalm(cMm p, cMm x);
Mm DLLI ppval(Mm p);
Mm DLLI ppval(cMm p, Mm x);
Mm DLLI roots(Mm p);
Mm DLLI spline(Mm x);
Mm DLLI spline(cMm x, cMm y);
Mm DLLI spline(cMm x, cMm y, cMm x2);
Mm DLLI ss2tf(cMm a, cMm b, cMm c, cMm d, i_o_t, Mm& num, Mm& den);
Mm DLLI ss2tf(cMm a, Mm b, cMm c, Mm d, cMm iu, i_o_t, Mm& num, Mm& den);
Mm DLLI ss2zp(cMm a, cMm b, cMm c, cMm d, i_o_t, Mm& z, Mm& p, Mm& k);
Mm DLLI ss2zp(cMm a, cMm b, cMm c, cMm d, i_o_t, Mm& z, Mm& p);
Mm DLLI ss2zp(cMm a, cMm b, cMm c, cMm d, cMm iu, i_o_t, Mm& z, Mm& p, Mm& k);
Mm DLLI tf2ss(Mm num, Mm den, i_o_t, Mm& a, Mm& b, Mm& c, Mm& d);
Mm DLLI tf2zp(cMm num, cMm den, i_o_t, Mm& z, Mm& p);
Mm DLLI tf2zp(Mm num, Mm den, i_o_t, Mm& z, Mm& p, Mm& k);
Mm DLLI unmkpp(cMm pp, i_o_t, Mm& b, Mm& c, Mm& l, Mm& k);
Mm DLLI zp2tf(cMm z, cMm p, Mm k, i_o_t, Mm& num, Mm& den);
Mm DLLI zp2ss(Mm z, Mm p, cMm k, i_o_t, Mm& a, Mm& b, Mm& c, Mm& d);

Mm DLLI ismember(Mm x);
Mm DLLI ismember(Mm x, Mm s);
Mm DLLI ismember(cMm x, Mm s, Mm f);
Mm DLLI intersect(cMm x);
Mm DLLI intersect(cMm x, cMm y);
Mm DLLI intersect(cMm x, cMm y, cMm f);
Mm DLLI intersect(Mm x, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI intersect(cMm x, cMm y, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI intersect(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI intersect(cMm x, cMm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI intersect(Mm x, Mm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI setdiff(cMm x);
Mm DLLI setdiff(cMm x, cMm y);
Mm DLLI setdiff(cMm x, cMm y, cMm f);
Mm DLLI setdiff(Mm x, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI setdiff(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI setdiff(Mm x, Mm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI setxor(cMm x);
Mm DLLI setxor(cMm x, cMm y);
Mm DLLI setxor(cMm x, cMm y, cMm f);
Mm DLLI setxor(Mm x, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI setxor(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI setxor(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI setxor(Mm x, Mm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI setxor(Mm x, Mm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI munion(cMm x);
Mm DLLI munion(cMm x, cMm y);
Mm DLLI munion(cMm x, cMm y, cMm f);
Mm DLLI munion(Mm x, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI munion(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI munion(cMm x, cMm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o, Mm& idx_y_o);
Mm DLLI munion(Mm x, Mm y, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI munion(cMm x, cMm y, cMm f, i_o_t, Mm& z_o, Mm& idx_x_o);
Mm DLLI unique(cMm x);
Mm DLLI unique(cMm x, cMm f);
Mm DLLI unique(cMm x, i_o_t, Mm& y_o, Mm& idx_o);
Mm DLLI unique(Mm x, i_o_t, Mm& y_o, Mm& idx_o, Mm& pos_o);
Mm DLLI unique(cMm x, cMm f, i_o_t, Mm& y_o, Mm& idx_o, Mm& pos_o);
Mm DLLI unique(cMm x, cMm f, i_o_t, Mm& y_o, Mm& idx_o);

int DLLI columns(cMm x);
int DLLI length(cMm x);
int DLLI rows(cMm x);
Mm DLLI size(cMm x);
Mm DLLI size(cMm x, cMm typ);
Mm DLLI size(cMm x, i_o_t, m_type& m, m_type& n);
Mm DLLI size(cMm x, i_o_t, Mm& m, Mm& n);
Mm DLLI size(cMm x, i_o_t, m_type& m, m_type& n, m_type& o);
Mm DLLI size(cMm x, i_o_t, Mm& m, Mm& n, Mm& o);

Mm DLLI sort(cMm x);
Mm DLLI sort(cMm x, cMm dim1);
Mm DLLI sort(cMm x, i_o_t, Mm& S, Mm& I);
Mm DLLI sort(cMm x, cMm dim1, i_o_t, Mm& S, Mm& I);
Mm DLLI sortrows(cMm x);
Mm DLLI sortrows(cMm x, cMm col);
Mm DLLI sortrows(cMm x, i_o_t, Mm& S, Mm& I);
Mm DLLI sortrows(cMm x, cMm col, i_o_t, Mm& S, Mm& I);

int  PDLLI getJc(cMm x);
int  PDLLI getIr(cMm x);
void DLLI freeIrJc(int* ir, int* jc);
void DLLI setIrJc(Mm& x, int* ir, int* jc);

Mm DLLI colmmd(cMm x);
Mm DLLI colperm(Mm x);
Mm DLLI find(cMm x);
Mm DLLI find(cMm x, i_o_t, Mm& I, Mm& J);
Mm DLLI find(cMm x, i_o_t, Mm& I, Mm& J, Mm& v);
Mm DLLI full(cMm x);
Mm DLLI nnz(cMm x);
Mm DLLI nonzeros(cMm x);
Mm DLLI nzmax(cMm x);
Mm DLLI randperm(cMm x);
Mm DLLI sp2coo(cMm x);
Mm DLLI spalloc(cMm m);
Mm DLLI spalloc(cMm m, cMm n);
Mm DLLI spalloc(cMm m, cMm n, cMm nnzmax);
Mm DLLI sparsfun(cMm x, cMm y);
Mm DLLI sparsfun(cMm x, cMm y, i_o_t, Mm& a, Mm& b);
Mm DLLI sparse(cMm x);
Mm DLLI sparse(cMm m, cMm n);
Mm DLLI sparse(cMm i, cMm j, cMm v);
Mm DLLI sparse(cMm i, cMm j, cMm v, cMm dummy);
Mm DLLI sparse(cMm i, cMm j, cMm v, cMm m, cMm n);
Mm DLLI sparse(Mm i, Mm j, Mm v, Mm m, Mm n, Mm nzmax);
Mm DLLI spconvert(Mm x);
Mm DLLI speye();
Mm DLLI speye(cMm x);
Mm DLLI speye(cMm x, cMm y);
Mm DLLI spfun(cMm func);
Mm DLLI spfun(cMm func, cMm x);
Mm DLLI spones(cMm x);
Mm DLLI spparms(cMm x);
Mm DLLI spparms(cMm x, cMm y);
Mm DLLI sprand(cMm x);
Mm DLLI sprand(cMm m, cMm n);
Mm DLLI sprand(cMm m, cMm n, cMm d);
Mm DLLI sprandn(cMm x);
Mm DLLI sprandn(cMm m, cMm n);
Mm DLLI sprandn(cMm m, cMm n, cMm d);
Mm DLLI spzeros(cMm x);
Mm DLLI spzeros(cMm x, cMm y);


Mm DLLI betainc(Mm x);
Mm DLLI betainc(Mm x, Mm a);
Mm DLLI betainc(Mm x, Mm a, Mm b);
Mm DLLI cart2pol(cMm x, cMm y, i_o_t, Mm& theta, Mm& r);
Mm DLLI cart2pol(cMm x, cMm y, cMm z, i_o_t, Mm& theta, Mm& r, Mm& z_o);
Mm DLLI cart2sph(cMm x, cMm y, cMm z, i_o_t, Mm& az, Mm& el, Mm& r);
Mm DLLI cross(cMm a);
Mm DLLI cross(cMm a, cMm b);
Mm DLLI cross(Mm a, Mm b, cMm dim1);
Mm DLLI dot(cMm a);
Mm DLLI dot(cMm a, cMm b);
Mm DLLI dot(Mm a, Mm b, cMm dim1);
Mm DLLI ellipj(cMm u);
Mm DLLI ellipj(cMm u, cMm m);
Mm DLLI ellipj(cMm u, i_o_t, Mm& sn, Mm& cn);
Mm DLLI ellipj(cMm u, cMm m, i_o_t, Mm& sn, Mm& cn);
Mm DLLI ellipj(cMm u, i_o_t, Mm& sn, Mm& cn, Mm& dn);
Mm DLLI ellipj(Mm u, Mm m, i_o_t, Mm& sn, Mm& cn, Mm& dn);
Mm DLLI ellipke(cMm m);
Mm DLLI ellipke(Mm m, i_o_t, Mm& k, Mm& e);
Mm DLLI factor(cMm n);
Mm DLLI gcd(cMm a);
Mm DLLI gcd(cMm a, cMm b);
Mm DLLI gcd(cMm a, cMm b, i_o_t, Mm& g, Mm& c, Mm& d);
Mm DLLI isprime(cMm n);
Mm DLLI lcm(cMm a);
Mm DLLI lcm(cMm a, cMm b);
Mm DLLI perms(cMm v);
Mm DLLI pol2cart(cMm theta, cMm r, i_o_t, Mm& x, Mm& y);
Mm DLLI pol2cart(cMm theta, cMm r, cMm z, i_o_t, Mm& x, Mm& y, Mm& z_o);
Mm DLLI primes(cMm n);
Mm DLLI nchoosek(cMm n);
Mm DLLI nchoosek(cMm n, cMm k);
Mm DLLI rat(cMm x);
Mm DLLI rat(cMm x, cMm t);
Mm DLLI rat(cMm x, i_o_t, Mm& n, Mm& d);
Mm DLLI rat(cMm x, cMm t, i_o_t, Mm& n, Mm& d);
Mm DLLI sph2cart(cMm az, cMm el, cMm r, i_o_t, Mm& x, Mm& y, Mm& z);

Mm DLLI sscanf(cMm s);
Mm DLLI sscanf(cMm s, cMm format1);
Mm DLLI sscanf(cMm s, cMm format1, i_o_t, Mm& A, Mm& count);
Mm DLLI sscanf(cMm s, cMm format1, i_o_t, Mm& A, Mm& count, Mm& errmsg);
Mm DLLI sscanf(cMm s, cMm format1, i_o_t, Mm& A, Mm& count, Mm& errmsg, Mm& next1);
Mm DLLI sscanf(cMm s, cMm format1, cMm size);
Mm DLLI sscanf(cMm s, cMm format1, cMm size, i_o_t, Mm& A, Mm& count);
Mm DLLI sscanf(cMm s, cMm format1, cMm size, i_o_t, Mm& A, Mm& count, Mm& errmsg);
Mm DLLI sscanf(cMm s, cMm format1, cMm size, i_o_t, Mm& A, Mm& count, Mm& errmsg, Mm& next1);

Mm DLLI sprintf(cMm a);
Mm DLLI sprintf(cMm format1, cMm varargin);

Mm DLLI TM(const char *text);
int DLLI Mstr_size(cMm s);
void DLLI Mstr(cMm x, char *str, int maxlen);
void DLLI Mstr_row(cMm x, char *str, int maxlen);

Mm DLLI base2dec(cMm x);
Mm DLLI base2dec(cMm x, cMm b);
Mm DLLI bin2dec(cMm x);
Mm DLLI blanks(cMm x);
Mm DLLI blanks(cMm m, cMm n);
Mm DLLI deblank(cMm x);
Mm DLLI deblank_leading(cMm x);
Mm DLLI dec2base(cMm x);
Mm DLLI dec2base(cMm x, cMm n);
Mm DLLI dec2base(cMm x, cMm n, cMm width);
Mm DLLI dec2bin(cMm x);
Mm DLLI dec2bin(cMm x, cMm width);
Mm DLLI dec2hex(cMm x);
Mm DLLI dec2hex(cMm x, cMm width);
Mm DLLI findstr(cMm x);
Mm DLLI findstr(cMm x, cMm y);
Mm DLLI hex2dec(cMm x);
Mm DLLI hex2num(cMm x);
Mm DLLI int2str(cMm x);
Mm DLLI isletter(cMm x);
Mm DLLI islower(cMm x);
Mm DLLI isspace(cMm x);
Mm DLLI isupper(cMm x);
Mm DLLI lower(cMm x);
Mm DLLI mat2str(cMm x);
Mm DLLI mat2str(cMm x, cMm prec);
Mm DLLI num2str(cMm x);
Mm DLLI num2str(cMm x, i_o_t, Mm& str, Mm& width);
Mm DLLI num2str(cMm x, cMm prec);
Mm DLLI num2str(cMm x, cMm prec, i_o_t, Mm& str, Mm& width);
Mm DLLI setstr(cMm x);
Mm DLLI str2double(cMm x);
Mm DLLI str2mat(cMm varargin);
Mm DLLI str2num(cMm x);
Mm DLLI strcat(cMm varargin);
Mm DLLI strcmp(cMm a);
Mm DLLI strcmp(cMm a, cMm b);
Mm DLLI strcmpi(cMm a);
Mm DLLI strcmpi(cMm a, cMm b);
Mm DLLI strjust(cMm x);
Mm DLLI strjust(cMm x, cMm how);
Mm DLLI strmatch(cMm a);
Mm DLLI strmatch(cMm a, cMm b);
Mm DLLI strmatch(Mm a, Mm b, cMm f);
Mm DLLI strncmp(cMm a);
Mm DLLI strncmp(cMm a, cMm b);
Mm DLLI strncmp(Mm a, Mm b, cMm n);
Mm DLLI strncmpi(cMm a);
Mm DLLI strncmpi(cMm a, cMm b);
Mm DLLI strncmpi(cMm a, cMm b, cMm n);
Mm DLLI strrep(cMm S);
Mm DLLI strrep(cMm S, cMm from);
Mm DLLI strrep(Mm S, cMm from, cMm to1);
Mm DLLI strtok(cMm a);
Mm DLLI strtok(cMm a, cMm b);
Mm DLLI strtok(cMm a, i_o_t, Mm& s, Mm& r);
Mm DLLI strtok(cMm a, cMm b, i_o_t, Mm& s, Mm& r);
Mm DLLI strvcat(cMm a1, cMm a2);
Mm DLLI strvcat(cMm varargin);
Mm DLLI upper(cMm x);


void DLLI tworeal2complex(int n, m_type* xr, m_type* xi, m_type* xc);
void DLLI complex2tworeal(int n, m_type* xc, m_type* xr, m_type* xi);

/* 
 these functions must not be used on f.p. registers such as
 mt_lt(fabs(..),1.0). they will fail handling NaN since the
 result is still 80 bits. the functions work correctly when
 comparing memory arguments, even for NaNs.

 with both Visual C++ and C++ Builder,

true:   nan<1 nan<=1 1<nan 1<=nan
false:  nan>1 nan>=1 1>nan 1>=nan

 i.e. <,<= with nan is always true but >,>= with nan is always false
*/

#define mt_eq(x,y)  ((x)>=(y) && (y)>=(x))
#define mt_ne(x,y)  ((x)<(y) || (y)<(x))
#define mt_le(x,y)  ((y)>=(x))
#define mt_lt(x,y)  ((y)>(x))
#define mt_is_nz(x) mt_ne((x),0.0)
#define mt_is_z(x)  mt_ne((x),0.0)



#if !defined(_MSC_VER) | (_MSC_VER>=1100)
m_type DLLI abs(m_type x);
#endif

m_type DLLI fix(m_type x);
m_type DLLI log2(m_type x);
m_type DLLI max2(m_type x, m_type y);
m_type DLLI min2(m_type x, m_type y);
m_type DLLI minus(m_type x, m_type y);
m_type DLLI mod(m_type x, m_type y);
m_type DLLI mpower(m_type x, m_type y);
m_type DLLI mrdivide(m_type x, m_type y);
m_type DLLI mtimes(m_type x, m_type y);
m_type DLLI plus(m_type x, m_type y);
m_type DLLI power(m_type x, m_type y);
m_type DLLI rem(m_type x, m_type y);
m_type DLLI round(m_type x);
m_type DLLI sign(m_type x);

m_type mt_acos(m_type x);
m_type mt_acosh(m_type x);
m_type mt_acot(m_type x);
m_type mt_acoth(m_type x);
m_type mt_acsc(m_type x);
m_type mt_acsch(m_type x);
m_type mt_angle(m_type x);
m_type mt_asec(m_type x);
m_type mt_asech(m_type x);
m_type mt_asin(m_type x);
m_type mt_asinh(m_type x);
m_type mt_atan(m_type x);
m_type mt_atan2(m_type x, m_type y);
m_type mt_atanh(m_type x);
m_type mt_besselj(int n, m_type x);
m_type mt_bessely(int n, m_type x);
m_type mt_beta(m_type a, m_type b);
m_type mt_betainc(m_type x, m_type a, m_type b);
m_type mt_betaln(m_type a, m_type b);
m_type mt_bitand(m_type a, m_type b);
m_type mt_bitcmp(m_type a, m_type n);
m_type mt_bitget(m_type a, m_type n);
m_type mt_bitor(m_type a, m_type b);
m_type mt_bitreset(m_type a, m_type n);
m_type mt_bitset(m_type a, m_type n);
m_type mt_bitshift(m_type a, m_type n);
m_type mt_bitxor(m_type a, m_type b);
m_type mt_ceil(m_type x);
m_type mt_cos(m_type x);
m_type mt_cosh(m_type x);
m_type mt_cot(m_type x);
m_type mt_coth(m_type x);
m_type mt_csc(m_type x);
m_type mt_csch(m_type x);
void   mt_ellipj(m_type u, m_type m, m_type* sn, m_type* cn, m_type* dn);
void   mt_ellipke(m_type m, m_type* k, m_type* e);
m_type mt_erf(m_type x);
m_type mt_erfc(m_type x);
m_type mt_erfcx(m_type x);
m_type mt_erfinv(m_type x);
m_type mt_exp(m_type x);
m_type mt_expint(m_type x);
m_type mt_fix(m_type x);
m_type mt_floor(m_type x);
m_type mt_frexp(m_type x, int* y);
m_type mt_gamma(m_type x);
m_type mt_gammainc(m_type x, m_type a);
m_type mt_gammaln(m_type x);
m_type mt_inf();
m_type mt_log(m_type x);
m_type mt_log2(m_type x);
m_type mt_log10(m_type x);
m_type mt_ldexp(m_type x, int y);
m_type mt_max2(m_type x, m_type y);
m_type mt_min2(m_type x, m_type y);
m_type mt_mod(m_type x, m_type y);
m_type mt_nan();
m_type mt_pow(m_type x, m_type y);
m_type mt_pow2_2(m_type x, m_type y);
m_type mt_pow2(m_type y);
m_type mt_pyth(m_type x, m_type y);
m_type mt_rem(m_type x, m_type y);
m_type mt_round(m_type x);
m_type mt_sec(m_type x);
m_type mt_sech(m_type x);
m_type mt_sign(m_type x);
m_type mt_sin(m_type x);
m_type mt_sinc(m_type x);
m_type mt_sinh(m_type x);
m_type mt_sqrt(m_type x);
m_type mt_tan(m_type x);
m_type mt_tanh(m_type x);

Mm DLLI dir();
Mm DLLI dir(cMm filename);
Mm DLLI kbhitM();
Mm DLLI exit();
Mm DLLI isunix();
Mm DLLI quit();
Mm DLLI soundsc(cMm y);
Mm DLLI soundsc(cMm y, cMm rate);
Mm DLLI soundsc(cMm y, cMm rate, cMm bits);
Mm DLLI soundM(cMm y);
Mm DLLI soundM(cMm y, cMm rate);
Mm DLLI soundM(cMm y, cMm rate, cMm bits);
#define playsnd soundM
Mm DLLI tempdir();
Mm DLLI tempname();
Mm DLLI memory();

void DLLI initM2(int version, int sizeofMm);
#define initM(version) initM2(version,sizeof(Mm))
unsigned int DLLI mt_set_MT(int multithread);

void DLLI exitM();


typedef void mt_input_handler_type(char* s, int n);
void DLLI mt_set_input_handler(mt_input_handler_type* new_input_handler);
extern mt_input_handler_type PDLLI mt_input_handler;

typedef int mt_kbhit_handler_type(void);
void DLLI mt_set_kbhit_handler(mt_kbhit_handler_type* new_kbhit_handler);
extern mt_kbhit_handler_type PDLLI mt_kbhit_handler;

typedef int mt_clc_handler_type(void);
void DLLI mt_set_clc_handler(mt_clc_handler_type* new_clc_handler);
extern mt_clc_handler_type PDLLI mt_clc_handler;

Mm DLLI clc();
Mm DLLI celldisp(cMm x);
Mm DLLI dump(cMm x);
Mm DLLI disp(cMm x);
Mm DLLI display(cMm x);
Mm DLLI disp_header(cMm x);
Mm DLLI get_whos_line(cMm x);
Mm DLLI error();
Mm DLLI error(cMm x);
Mm DLLI format();
Mm DLLI format(cMm x);
Mm DLLI format(cMm x, cMm y);
Mm DLLI home();
Mm DLLI input(cMm x);
Mm DLLI input(cMm x, cMm s);
Mm DLLI pauseM();
Mm DLLI pauseM(cMm x);
Mm DLLI puts(cMm x);
Mm DLLI smiley();
Mm DLLI usage(cMm x);
Mm DLLI warning();
Mm DLLI warning(cMm x);
void DLLI whos_line(cMm x);
int DLLI mt_printf(const char* format,...);

Mm  DLLI cat(cMm dim1);
Mm  DLLI cat(cMm dim1, cMm a);
Mm  DLLI cat(cMm dim1, Mm a, Mm b);
Mm  DLLI cat(cMm dim1, cMm a, cMm b, cMm c);
Mm  DLLI ctranspose(cMm x);
Mm  DLLI dot_transpose(cMm x);
Mm  DLLI flipdim(cMm x);
Mm  DLLI flipdim(cMm x, cMm dim1);
Mm  DLLI fliplr(cMm x);
Mm  DLLI flipud(cMm x);
Mm  DLLI horzcat(cMm a);
Mm  DLLI horzcat(cMm a, cMm b);
Mm  DLLI repmat(cMm x);
Mm  DLLI repmat(cMm x, cMm m, cMm n);
Mm  DLLI repmat(cMm x, cMm mn);
Mm  DLLI reshape(cMm x);
Mm  DLLI reshape(cMm x, cMm m, cMm n);
Mm  DLLI reshape(cMm x, cMm m, cMm n, cMm o);
Mm  DLLI reshape(cMm x, cMm m, cMm n, cMm o, cMm p);
Mm  DLLI reshape(cMm x, cMm mn);
Mm  DLLI rot90(cMm x);
Mm  DLLI rot90(cMm x, cMm k);
Mm  DLLI transpose(cMm x);
Mm  DLLI vertcat(cMm a);
Mm  DLLI vertcat(cMm a, cMm b);

class variables_scope_t {
  int depth;
  Mm** vars[mt_stack_max_depth];
  int* dyn_vars[mt_stack_max_depth];
  int num_vars[mt_stack_max_depth];
  int allocated_vars[mt_stack_max_depth];
  int get_depth(Mm* obj) const;
public:
  variables_scope_t();
  ~variables_scope_t();

  void DLLI begin();
  void DLLI reset();
  void DLLI end();
  int GetDepth() const;

  int Count() const;
  int Count(int sdepth) const;
  const char* NameOfIndex(int index) const;
  const char* NameOfIndex(int index, int sdepth) const;
  int IndexOf(const char* name) const;
  int IndexOf(const char* name, int sdepth) const;
  Mm* Get(Mm* obj) const;
  Mm* Get(int index) const;
  Mm* Get(int index, int sdepth) const;
  Mm* Get(const char* name) const;
  Mm* Get(const char* name, int sdepth) const;
  void Add(Mm* obj, int dynamic);
  void Delete(Mm* obj);
  void Clear(int index);
  void Clear(int index, int sdepth);
  void Clear(Mm* obj);
};

extern DLLW variables_scope_t DLLI variables_scope;

class variables_scope_helper_t {
  int begin;
public:
  variables_scope_helper_t() { variables_scope.begin(); begin=1; }
  ~variables_scope_helper_t() { if (begin) variables_scope.end(); }
  void end() { variables_scope.end(); begin=0; }
};

#define begin_scope	variables_scope_helper_t variables_scope_helper; {
#define end_scope	}

Mm DLLI clear();
Mm DLLI clear(const Mc& varargin);
int DLLI exist(cMm x);
int DLLI exist(cMm x, cMm what);
Mm DLLI openvar(cMm name);
Mm DLLI which(cMm name);
Mm DLLI who();
Mm DLLI who(cMm how, cMm filename);
Mm DLLI whos();
Mm DLLI whos(cMm how, cMm filename);

#endif // matlibH

#undef TMP_MAX
#undef abs
#undef acos
#undef and
#undef asin
#undef atan
#undef atan2
#undef bitand
#undef bitor
#undef clear
#undef cos
#undef exp
#undef fabs
#undef feof
#undef ferror
#undef finite
#undef isinf
#undef islower
#undef isnan
#undef isspace
#undef isupper
#undef log
#undef log10
#undef log2
#undef max
#undef min
#undef not
#undef or
#undef sin
#undef sin
#undef sqrt
#undef strcmp
#undef strcmpi
#undef strncmp
#undef strncmpi
#undef tan
#undef xor


