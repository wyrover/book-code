// Combining STL components with inheritance and virtual
// functions.
#include "shape.h"
#include <vector.h> 
#include <algo.h>  

class myshape : public rectangle {
 /* Define a new shape, a simple "face," derived from
    rectangle.
 */
  line* l_eye;
  line* r_eye;
  line* mouth;
 public:
  myshape(point, point);
  void draw();
  void move(int, int);
};

myshape::myshape(point a, point b) : rectangle(a,b)
{ 
  int ll = neast().x - swest().x+1;
  int hh = neast().y - swest().y+1;
  l_eye = 
    new line(point(swest().x+2,swest().y+hh*3/4),2);
  r_eye = 
    new line(point(swest().x+ll-4,swest().y+hh*3/4),2);
  mouth = 
    new line(point(swest().x+2,swest().y+hh/4),ll-4);
}

void myshape::draw()
  // Draw it by drawing the rectangle and a point for the
  // "nose"; the eye and mouth objects are refreshed 
  // separately by the shape_refresh function.
{
  rectangle::draw();
  int a = (swest().x + neast().x)/2;
  int b = (swest().y + neast().y)/2;
  put_point(point(a,b));
}

void myshape::move(int a, int b)
  // Move it by moving the base rectangle and secondary
  // objects.
{
  rectangle::move(a,b);
  l_eye->move(a,b);
  r_eye->move(a,b);
  mouth->move(a,b);
}

// Beginning of definitions added in order to use STL 
// with shape classes.

struct CompWestX : binary_function<shape*, shape*, bool> {
   
  bool operator()(shape* p, shape* q) const
    // Compare shapes based on the x-coordinate of 
    // west point. The west function is virtual in the 
    // shape class, so the comparison is made based on
    // how it is defined for a more specific shape.
  {
    return p->west().x < q->west().x;
  }
} compWestX;

void outputWestX (const vector<shape*>& vs) 
   // Output the x-coordinate of the west point of each
   // shape in vs.
{
  vector<shape*>::const_iterator i;
  for (i = vs.begin(); i != vs.end(); ++i)
    cout << "The x-coordinate of the west point of shape " 
         << i - vs.begin() << " is " << (*i)->west().x 
	 << endl;
}

// End of definitions added in order to use STL.

int main()
{
// First part is same as in Stroustrup's book.
  screen_init();
  shape* p1 = new rectangle(point(0,0),point(10,10));
  shape* p2 = new line(point(0,15),17);
  shape* p3 = new myshape(point(15,10),point(27,18));
  shape_refresh();
  p3->move(-10,-10);
  stack(p2,p3);
  stack(p1,p2);
  shape_refresh();

// This part is added as a tiny example of use of STL 
// along with code that uses a class hierarchy and 
// virtual functions. First put points into an STL vector.
  vector<shape*> vs;
  vs.push_back(p1);
  vs.push_back(p2);
  vs.push_back(p3);

// Use STL-provided iterator to move all points 
// horizontally 20 units.
  vector<shape*>::iterator i;
  for (i = vs.begin(); i != vs.end(); ++i)
    (*(*i)).move(20, 0);
  shape_refresh();

// Demonstrate use of STL generic sorting algorithm with 
// objects in a class hierarchy.
  outputWestX(vs);
  cout << "Sorting the shapes according to the "
       << "x-coordinate of their west points." << endl;
  sort(vs.begin(), vs.end(), compWestX); 
  cout << "After sorting:" << endl;
  outputWestX(vs);
  screen_destroy();
}
