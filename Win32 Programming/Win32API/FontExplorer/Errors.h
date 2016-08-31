//{{NO_DEPENDENCIES}}
/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 21-Jan-93 | <jmn> Created log
* 21-Jan-93 | <jmn> Added 4027, 4029, 4047, 4049 as errors
*  8-Jan-94 | <jmn> added 4101: C++ unref local var
* 19-Oct-94 | <jmn> added 4028 error
*  9-Nov-94 | <jmn> added 4211 error
*  2-Feb-95 | <jmn> Added 4098, 4070 errors
*  2-Feb-95 | <jmn> added 4087, 4088, 4089
*  2-Feb-95 | <jmn> added 4024
* 14-Feb-95 | <jmn> added 4100
*****************************************************************************/
/* Custom error level stuff */
/* We can modify any message in the range 1..699 or 4001..4699 */
#pragma warning(error:4002) /* too many actual parameters to macro */
#pragma warning(error:4003) /* too few actual parameters to macro */
#pragma warning(1:4010)     /* single-line comment ends with \ */
#pragma warning(error:4013)     /* missing prototype */
#pragma warning(1:4016)     /* no function return type */
#pragma warning(error:4020) /* too many actual parameters */
#pragma warning(error:4021) /* too few actual parameters */
#pragma warning(error:4024) /* different types for formal and actual */
#pragma warning(error:4027) /* function declared w/o formal parameter list*/ 
#pragma warning(error:4028) /* formal parameter different from declaration */
#pragma warning(error:4029) /* declared formal list different from defn */
#pragma warning(error:4035) /* no return value */
#pragma warning(error:4033) /* 'return;' in value-returning proc */
#pragma warning(error:4045) /* array bounds overflow */
#pragma warning(error:4047) /* different levels of indirection */
#pragma warning(error:4049) /* indirection to different types */
#pragma warning(1:4050)     /* different code attributes */
#pragma warning(error:4053) /* one void operand for ?: */
#pragma warning(error:4087) /* <fn> declared with void parameter list */
#pragma warning(error:4070) /* return of a 'void' expression */
#pragma warning(error:4071) /* no function prototype given */
#pragma warning(error:4088) /* <fn> pointer mismatch in actual parameter */
#pragma warning(error:4089) /* <fn> different types in actual, formal */
#pragma warning(error:4090) /* operation: different const or volatile quals */
#pragma warning(error:4098) /* '<fn>' : 'void' function returning a value */
#pragma warning(disable:4100) /* unreferenced formal parameter */
#pragma warning(disable:4101) /* unreferenced local variable */
#pragma warning(error:4150) /*  deletion of pointer to incomplete type '...'; no destructor called */
#pragma warning(error:4211)  /*nonstandard extension used : redefined extern to static */
#pragma warning(1:4306)  /* conversion from <integer type> to <pointer type> of greater size */
//#pragma warning(1:4505)  /* unreferenced local function has been removed */
/* #pragma warning(error:4759) */ /* segment lost in conversion */

