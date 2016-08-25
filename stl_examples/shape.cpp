#include "shape.h"

rectangle::rectangle(point a, point b)
{
    if (a.x <= b.x) {
       if (a.y <= b.y) {
            sw = a;
            ne = b;
       }
       else {
            sw = point(a.x, b.y);
            ne = point(b.x, a.y);
       }
    }
    else {
       if (a.y <= b.y) {
            sw = point(b.x, a.y);
            ne = point(a.x, b.y);
       }
       else {
            sw = b;
            ne = a;
       }
    }
}

void rectangle::draw()
{
   point nw(sw.x, ne.y);
   point se(ne.x, sw.y);
   put_line(nw,ne);
   put_line(ne,se);
   put_line(se,sw);
   put_line(sw,nw);
}

void shape_refresh()
{
   screen_clear();
   for (shape* p = shape::list; p; p=p->next) p->draw();
   screen_refresh();
}

void stack(shape* p, const shape* q)  // put p on top of q
{ 
   point n = q->north();
   point s = p->south();
   p->move(n.x-s.x, n.y-s.y+1);
}

shape* shape::list = 0;

