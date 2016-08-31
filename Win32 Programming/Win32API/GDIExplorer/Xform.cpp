#include "stdafx.h"
#include "math.h"

#include "xform.h"

/****************************************************************************
*                               CXform::CXform
* Effect: 
*       Creates a matrix and initializes it as an identity matrix
****************************************************************************/

CXform::CXform()
    {
     SetIdentity();
    }

/****************************************************************************
*                               CXform::~CXform
* Effect: 
*       Destructor
****************************************************************************/

CXform::~CXform()
    {
     
    }

/****************************************************************************
*                               CXform::CXform
* Inputs:
*       int M11kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M11: MATRIX_NONE: the value of M11
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M12kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M12: MATRIX_NONE: the value of M12
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M21kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M21: MATRIX_NONE: the value of M21
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M22kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M22: MATRIX_NONE: the value of M22
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
* Effect: 
*       Creates and initializes the matrix with the specified values
****************************************************************************/

CXform::CXform(int M11kind, float M11, int M12kind, float M12,
                  int M21kind, float M21, int M22kind, float M22, 
                               float DX,               float DY)
    {
     SetMatrix(M11kind, M11, M12kind, M12,
               M21kind, M21, M22kind, M22,
                        DX,           DY);
    }

/****************************************************************************
*                               CXform::convert
* Inputs:
*       int kind:
*       float value:
* Result: float
*       kind == MATRIX_NONE    =>  value
*       kind == MATRIX_SIN     =>  sin(value)
*       kind == MATRIX_NEG_SIN => -sin(value);
*       kind == MATRIX_COS     =>  cos(value)
*       kind == MATRIX_NEG_COS => -cos(value)
****************************************************************************/

float CXform::convert(int kind, float value)
    {
     
     switch(kind)
        { /* kind */
         case MATRIX_NONE:
                 return value;
         case MATRIX_SIN:
                 return (float)sin(deg_to_rad(value));
         case MATRIX_NEG_SIN:
                 return -(float)sin(deg_to_rad(value));
         case MATRIX_COS:
                 return (float)cos(deg_to_rad(value));
         case MATRIX_NEG_COS:
                 return -(float)cos(deg_to_rad(value));
        } /* kind */
     return value; // default if nonsense given
    }

/****************************************************************************
*                                  CXform::unconvert
* Inputs:
*       int kind: Kind of transformation
*       float value: Transformed value
* Result: float
*       The value (modulo floating point accuracy) that produced
*       the input value, that is
*               unconvert(kind, convert(kind, val)) == val
*       except for floating-point roundoff
****************************************************************************/

float CXform::unconvert(int kind, float value)
    {
     switch(kind)
        { /* kind */
         case MATRIX_NONE:
                 return value;
         case MATRIX_SIN:
                 return rad_to_deg((float)asin(value));
         case MATRIX_NEG_SIN:
                 return rad_to_deg((float)asin(-value));
         case MATRIX_COS:
                 return rad_to_deg((float)acos(value));
         case MATRIX_NEG_COS:
                 return rad_to_deg((float)acos(-value));
        } /* kind */
     return value; // default if nonsense given
     
    }

/****************************************************************************
*                              CXform::SetMatrix
* Inputs:
*       int M11kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M11: MATRIX_NONE: the value of M11
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M12kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M12: MATRIX_NONE: the value of M12
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M21kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M21: MATRIX_NONE: the value of M21
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
*       int M22kind: MATRIX_NONE MATRIX_SIN, MATRIX_COS, MATRIX_NEG_SIN,
*                    MATRIX_NEG_COS
*       float M22: MATRIX_NONE: the value of M22
*                  MATRIX_angle: the angle whose value is to be taken,
*                  in degrees.
* Result: void
*       
* Effect: 
*       Sets the matrix to the indicated values
****************************************************************************/

void CXform::SetMatrix(int M11kind, float M11, int M12kind, float M12,
                  int M21kind, float M21, int M22kind, float M22, 
                               float DX,               float DY)
    {
     kind.M11 = M11kind;
     kind.M12 = M12kind;
     kind.M21 = M21kind;
     kind.M22 = M22kind;
     matrix.eM11 = M11;
     matrix.eM12 = M12;
     matrix.eM21 = M21;
     matrix.eM22 = M22;
     matrix.eDx = DX;
     matrix.eDy = DY;
    }

/****************************************************************************
*                              CXform::GetMatrix
* Inputs:
*       XFORM * mat: The matrix we are to fill in from our matrix
* Result: void
*       
* Effect: 
*       Fills in the values in the matrix. Does appropriate conversions
*       The result is a matrix that can be used for a real transform.
****************************************************************************/

void CXform::GetMatrix(XFORM * mat)
    {
     mat->eM11 = convert(kind.M11, matrix.eM11);
     mat->eM12 = convert(kind.M12, matrix.eM12);
     mat->eM21 = convert(kind.M21, matrix.eM21);
     mat->eM22 = convert(kind.M22, matrix.eM22);
     mat->eDx  = matrix.eDx;
     mat->eDy  = matrix.eDy;
    }

/****************************************************************************
*                             CXform::SetIdentity
* Result: void
*       
* Effect: 
*       Sets the current matrix to the identity matrix
****************************************************************************/

void CXform::SetIdentity()
    {
     SetMatrix(MATRIX_NONE, (float)1.0, MATRIX_NONE, (float)0.0,
               MATRIX_NONE, (float)0.0, MATRIX_NONE, (float)1.0,
                            (float)0.0,              (float)0.0);
     
    }
