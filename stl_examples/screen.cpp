#include "screen.h"
#include <iostream.h>

enum color { black='*', white=' ' };

char screen[XMAX][YMAX];

void screen_init()
{ 
    for (int y=0; y<YMAX; y++)
        for (int x=0; x<XMAX; x++)
            screen[x][y] = white;
}

void screen_destroy() {}

inline int on_screen(int a, int b)   // clipping
{
    return 0<=a && a<XMAX && 0<=b && b<YMAX;
}

void put_point(int a, int b)
{
    if (on_screen(a, b)) screen[a][b] = black;
}

void put_line(int x0, int y0, int x1, int y1)
/*
   Plot the line (x0, y0) to (x1, y1).
   The line being plotted is b(x-x0) +a(y-y0) = 0.
   Minimize abs(eps) where eps = 2*(b(x-x0) + a(y-y0).
   See Newman and Sproull:
   ``Principles of Interactive Computer Graphics''
   McGraw-Hill, New York, 1979.  pp. 33-44.
*/
{
   register int dx = 1;
   int a = x1 - x0;
   if (a < 0) dx = -1, a = -a;

   register int dy = 1;
   int b = y1 - y0;
   if (b < 0) dy = -1, b = -b;

   int two_a = 2*a;
   int two_b = 2*b;
   int xcrit = -b + two_a;
   register int eps = 0;

   for(;;) {
      put_point(x0,y0);
      if (x0==x1 && y0 == y1) break;
      if (eps <= xcrit) x0 += dx, eps += two_b;
      if (eps>=a || a<=b) y0 += dy, eps -= two_a;
   }
}

void screen_clear() { screen_init(); }

void screen_refresh()
{
    for (int y=YMAX-1; 0<=y; y--) {    // top to bottom
        for (int x=0; x<XMAX; x++)      // left to right
            cout << screen[x][y];
        cout << '\n';
    }
}
