  #include "matlib.h"
  #pragma hdrstop
  
  #include "myeig.h"
  
  
  #line 1 "c:/matcom45/samples/m-files/myeig.m"
  // Çó¾ØÕóXÌØÕ÷ÖµE
  Mm myeig(Mm X) {
    begin_scope
    X.setname("X"); 
    dMm(E); 
    
    #line 2 "c:/matcom45/samples/m-files/myeig.m"
    call_stack_begin;
    #line 2 "c:/matcom45/samples/m-files/myeig.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=1.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 3 "c:/matcom45/samples/m-files/myeig.m"
_   E = eig(X);
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    X.setname(NULL); 
    return E;
    end_scope
  }
  
  
