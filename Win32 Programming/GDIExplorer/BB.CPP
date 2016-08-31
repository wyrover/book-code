#include "stdafx.h"
#include "bb.h"

CBoundingBox::CBoundingBox() : CRect(INT_MAX, INT_MAX, INT_MIN, INT_MIN)
   {
    xinvert = yinvert = FALSE;
   }

CBoundingBox::CBoundingBox(BOOL xinv, BOOL yinv) : 
                        CRect(xinv ? INT_MIN : INT_MAX, 
                              yinv ? INT_MIN : INT_MAX, 
                              xinv ? INT_MAX : INT_MIN, 
                              yinv ? INT_MAX : INT_MIN)
   {
    xinvert = xinv;
    yinvert = yinv;
   }

CBoundingBox::~CBoundingBox()
  {
  }

void CBoundingBox::SetX(int x)
  {
   SetLeft(x);
   SetRight(x);
  }

void CBoundingBox::SetY(int y)
  {
   SetTop(y);
   SetBottom(y);
  }

int CBoundingBox::SetTop(int newtop)
  {
   top = yinvert ? max(top, newtop) : min(top, newtop) ;
   return top;
  }

int CBoundingBox::SetLeft(int newleft)
  {
   left = xinvert ? max(left, newleft) : min(left, newleft);
   return left;
  }


int CBoundingBox::SetRight(int newright)
  {
   right = xinvert ? min(right, newright) : max(right, newright);
   return right;
  }

int CBoundingBox::SetBottom(int newbottom)
  {
   bottom = yinvert ? min(bottom, newbottom) : max(bottom, newbottom);
   return bottom;
  }

int CBoundingBox::SetWidth(int newwidth)
  {
   if(newwidth > Width())
      right = left + xinvert ? -newwidth : newwidth;
   return Width();
  }

int CBoundingBox::SetHeight(int newheight)
  {
   if(newheight > Height())
      bottom = top + yinvert ? -newheight : newheight;
   return Height();
  }

void CBoundingBox::Set(CRect r)
  {
   SetLeft(r.left);
   SetTop(r.top);
   SetRight(r.right);
   SetBottom(r.bottom);
  }

void CBoundingBox::Set(int left, int top, int right, int bottom)
  {
   SetLeft(left);
   SetTop(top);
   SetRight(right);
   SetBottom(bottom);
  }

void CBoundingBox::Set(int x, int y)
  {
   SetLeft(x);
   SetRight(x);
   SetTop(y);
   SetBottom(y);
  }

void CBoundingBox::LPtoDP(CDC * dc)
  {
   dc->LPtoDP((LPPOINT)this, 2);
   // If we had transformed coordinates, we may need to normalize...

   if(top > bottom)
      { /* swap TB */
       int t = top;
       top = bottom;
       bottom = t;
      } /* swap TB */

   if(left > right)
      { /* swap LR */
       int t = left;
       left = right;
       right = t;
      } /* swap LR */
  }
