#include "stdafx.h"
#include "polydraw.h"

/****************************************************************************
*                                  polyDraw
* Inputs:
*       CDC * dc: Display context
*	int width: Width of region in which to draw classic polygon
*	int height: Height of region in which to draw classic polygon
* Result: void
*       
* Effect: 
*       Draws the classic polygon using the current DC parameters
****************************************************************************/

void polyDraw(CDC * dc, CRect & r)
    {
     int hunit = r.Width() / 6;
     int vunit = r.Height() / 6;

     /*
	0    1    2    3    4    5    6
	|    |    |    |    |    |    |
	:    :    :    :    :    :    : 0
	:    :    :    :    :    :    :
	:    :    :    :    :    :    :
	:    :    :    9----0    :    : 1
	:    :    :    IXXXXI    :    :
	:    :    :    IXXXXI    :    :
	:    3---------+----+----4    : 2
	:    IXXXXXXXXXI    IXXXXI    :
	:    IXXXXXXXXXI    IXXXXI    :
	:    IXXXX6----+----+----5    : 3
	:    IXXXXI    IXXXXI    :    :
	:    IXXXXI    IXXXXI    :    :
	:    IXXXX7----8XXXXI    :    : 4
	:    IXXXXXXXXXXXXXXI    :    :
	:    IXXXXXXXXXXXXXXI    :    :
	:    2--------------1    :    : 5
	:    :    :    :    :    :    :
     */

     // The base points are given in this array, which is copied to
     // the stack array of the same size, adjusting for the units

     static POINT points[] = {
	     { 4, 1},
	     { 4, 5},
	     { 1, 5},
	     { 1, 2},
	     { 5, 2},
	     { 5, 3},
	     { 2, 3}, 
	     { 2, 4},
	     { 3, 4},
	     { 3, 1}
			     };

     POINT apoints[sizeof(points) / sizeof(points[0])];

     int i;
     for(i = 0; i < sizeof(points) / sizeof(points[0]); i++)
	{ /* copy */
	 apoints[i].x = r.left + points[i].x * hunit;
	 apoints[i].y = r.top + points[i].y * vunit;
	} /* copy */

     dc->Polygon(apoints, sizeof(points) / sizeof(points[0]));

    }
