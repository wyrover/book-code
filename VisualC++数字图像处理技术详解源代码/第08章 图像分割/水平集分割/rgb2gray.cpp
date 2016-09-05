  #include "matlib.h"
  #pragma hdrstop
  
  #include "rgb2gray.h"
  
  
  Mm rgb2gray(Mm r, Mm g, Mm b) {
    begin_scope
    r.setname("r"); g.setname("g"); b.setname("b"); 
    dMm(a); dMm(threeD); dMm(rgb); dMm(T); 
    
    #line 1 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    call_stack_begin;
    #line 1 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=3.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    #line 2 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //RGB2GRAY Convert RGB image or colormap to grayscale.
    #line 3 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   RGB2GRAY converts RGB images to grayscale by eliminating the
    #line 4 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   hue and saturation information while retaining the
    #line 5 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   luminance.
    #line 6 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //
    #line 7 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   I = RGB2GRAY(RGB) converts the truecolor image RGB to the
    #line 8 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   grayscale intensity image I.
    #line 9 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //
    #line 10 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   NEWMAP = RGB2GRAY(MAP) returns a grayscale colormap
    #line 11 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   equivalent to MAP.
    #line 12 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //
    #line 13 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   Class Support
    #line 14 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   -------------
    #line 15 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   If the input is an RGB image, it can be of class uint8, 
    #line 16 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   uint16 or double; the output image I is of the same class 
    #line 17 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   as the input image. If the input is a colormap, the input 
    #line 18 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   and output colormaps are both of class double.
    #line 19 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //
    #line 20 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   See also IND2GRAY, NTSC2RGB, RGB2IND, RGB2NTSC.
    
    #line 22 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   Copyright 1993-2002 The MathWorks, Inc.  
    #line 23 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    //   $Revision: 5.20 $  $Date: 2002/03/15 15:29:04 $
    
    #line 25 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   if (istrue(nargin()==0.0)) {
      #line 26 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     error(TM("Need input arguments."));
      #line 27 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    }
    #line 28 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   threeD = (ndims(r)==3.0);
    #line 28 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    // Determine if input includes a 3-D array 
    
    #line 30 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   if (istrue(nargin()==1.0)) {
      #line 31 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     if (istrue(threeD)) {
        #line 32 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       rgb = reshape(r(c_p),size(r,1.0)*size(r,2.0),3.0);
        #line 33 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       a = zeros((BR(size(r,1.0)),size(r,2.0)));
        #line 34 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
      } else {
        #line 34 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
        // Colormap
        #line 35 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       rgb = r;
        #line 36 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       a = zeros(size(r,1.0),1.0);
        #line 37 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
      }
      
      #line 39 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    } else
    #line 39 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    if (istrue(nargin()==2.0)) {
      #line 40 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     error(TM("Wrong number of arguments."));
      
      #line 42 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    } else
    #line 42 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    if (istrue(nargin()==3.0)) {
      #line 43 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     warning((BR(TM("RGB2GRAY(R,G,B) is an obsolete syntax.")),TM("Use a three-dimensional array to represent RGB image.") \
        ));
      #line 45 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     if (istrue(any(size(r)!=size(g)))||istrue(any(size(r)!=size(b)))) {
        #line 46 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       display( error(TM("R, G, and B must all be the same size.")) );
        #line 47 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
      }
      #line 48 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     rgb = (BR(r(c_p)),g(c_p),b(c_p));
      #line 49 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = zeros(size(r));
      #line 50 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    } else {
      
      #line 51 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     error(TM("Invalid input arguments."));
      #line 52 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    }
    
    #line 54 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   T = inv((BR(1.0),0.956,0.621,semi,
    1.0,-0.272,-0.647,semi,
    1.0,-1.106,1.703));
    
    #line 56 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   if (istrue(isa(rgb,TM("uint8")))) {
      #line 57 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = muint8(reshape(mdouble(rgb)*ctranspose(T(1.0,c_p)),size(a)));
      #line 58 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    } else
    #line 58 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    if (istrue(isa(rgb,TM("uint16")))) {
      #line 59 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = muint16(reshape(mdouble(rgb)*ctranspose(T(1.0,c_p)),size(a)));
      #line 60 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    } else
    #line 60 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    if (istrue(isa(rgb,TM("double")))) {
      #line 61 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = reshape(rgb*ctranspose(T(1.0,c_p)),size(a));
      #line 62 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = min(max(a,0.0),1.0);
      #line 63 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    }
    
    #line 65 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_   if (istrue((nargin()==1.0))&&istrue((!threeD))) {
      #line 65 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
      // rgb2gray(MAP)
      #line 66 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     if (istrue(!isa(a,TM("double")))) {
        #line 67 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_       a = im2double(a);
        #line 68 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
      }
      #line 69 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
_     a = (BR(a),a,a);
      #line 70 "d:/matlab6p5/toolbox/images/images/rgb2gray.m"
    }
    
    
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    r.setname(NULL); g.setname(NULL); b.setname(NULL); 
    return a;
    end_scope
  }
  
  
  Mm rgb2gray() {
    begin_scope
    double old_nargin=nargin_val; nargin_val=0.0; nargin_set=1;
    dMm(r); dMm(g); dMm(b); 
    dMm(ret1a);
    ret1a=rgb2gray(r, g, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm rgb2gray(Mm r) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
    dMm(g); dMm(b); 
    dMm(ret1a);
    ret1a=rgb2gray(r, g, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm rgb2gray(Mm r, Mm g) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=2.0; nargin_set=1;
    dMm(b); 
    dMm(ret1a);
    ret1a=rgb2gray(r, g, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
