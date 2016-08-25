#include "screen.h"

inline int max(int a, int b) { return a<b ? b : a; }
inline int min(int a, int b) { return a<b ? a : b; }

struct shape {
  static shape* list;
  shape* next;
  shape() { next = list; list = this; }
  
  virtual point north() const = 0;
  virtual point south() const = 0;
  virtual point east() const = 0;
  virtual point west() const = 0;
  virtual point neast() const = 0;
  virtual point seast() const = 0;
  virtual point nwest() const = 0;
  virtual point swest() const = 0;
  virtual void draw() = 0;
  virtual void move(int, int) = 0;
};

class line : public shape {
/*
    The line from "w" to "e".
    north() is defined as ``above the center,
    as far north as the northernmost point.''
*/
  point w, e;
 public:
  point north() const
    { return point((w.x+e.x)/2, max(e.y,w.y)); }
  point south() const
    { return point((w.x+e.x)/2, min(e.y,w.y)); }
  point east() const
    { return point(max(e.x, w.x), (w.y+e.y)/2); }
  point west() const 
    { return point(min(e.x, w.x), (w.y+e.y)/2); }
  point neast() const 
    { return point(max(e.x, w.x), max(e.y, w.y)); }
  point seast() const 
    { return point(max(e.x, w.x), min(e.y, w.y)); }
  point nwest() const 
    { return point(min(e.x, w.x), max(e.y, w.y)); }
  point swest() const 
    { return point(min(e.x, w.x), min(e.y, w.y)); }
  void move(int a, int b)
    { w.x += a; w.y += b; e.x += a; e.y += b; }
  void draw() { put_line(w,e); }
  line(point a, point b) { w = a; e = b; }
  line(point a, int len) 
    { w = point(a.x + len - 1, a.y); e = a; }
};

class rectangle : public shape {
/*
   nw ---- n ---- ne
   |               |
   w       c       e
   |               |
   sw ---- s ---- se
*/
    point sw, ne;
 public:
  point north() const 
    { return point((sw.x+ne.x)/2, ne.y); }
  point south() const 
    { return point((sw.x+ne.x)/2, sw.y); }
  point east() const 
    { return point(ne.x, (ne.y+sw.y)/2); }
  point west() const 
    { return point(sw.x, (ne.y+sw.y)/2); }
  point neast() const { return ne; }
  point seast() const { return point(ne.x, sw.y); }
  point nwest() const { return point(sw.x, ne.y); }
  point swest() const { return sw; }
  void move(int a, int b)
    { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
  void draw();
  rectangle(point, point);
};

void shape_refresh();      // draw all shapes
void stack(shape* p, const shape* q); // put p on top of q
