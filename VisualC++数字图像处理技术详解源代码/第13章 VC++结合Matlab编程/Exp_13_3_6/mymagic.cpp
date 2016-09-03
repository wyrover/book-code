  #include "matlib.h"
  #pragma hdrstop
  
  #include "mymagic.h"
  
  
  #line 1 "c:/matcom45/samples/m-files/mymagic.m"
  // …˙≥…nxnª√∑Ωæÿ’ÛX
  Mm mymagic(Mm n) {
    begin_scope
    n.setname("n"); 
    dMm(X); 
    
    #line 2 "c:/matcom45/samples/m-files/mymagic.m"
    call_stack_begin;
    #line 2 "c:/matcom45/samples/m-files/mymagic.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=1.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 3 "c:/matcom45/samples/m-files/mymagic.m"
_   X = magic(n);
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    n.setname(NULL); 
    return X;
    end_scope
  }
  
  
