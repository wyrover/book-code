#include <boost/scoped_ptr.hpp>
#include <cassert>
#include "Widget.h" // Widget definition
// …

void useTwoWidgets()
{
    // default constructed scoped_ptr
    boost::scoped_ptr<Widget> wgt;
    assert(!wgt);          // null test - Boolean context
    wgt.reset(new Widget); // create first widget
    assert(wgt);          // non-null test – Boolean context
    wgt->display();        // display first widget
    wgt.reset(new Widget); // destroy first, create second widget
    wgt->display();        // display second widget
    Widget *w1 = wgt.get();  // get the raw pointer
    Widget& rw1 = *wgt;      // 'dereference' the smart pointer
    assert(w1 == &rw1);      // same object, so same address
    boost::scoped_ptr<Widget> wgt2(new Widget);
    Widget *w2 = wgt2.get();
    wgt.swap(wgt2);
    assert(wgt.get() == w2);  // effect of swap
    assert(wgt2.get() == w1); // effect of swap
}

int main()
{
    useTwoWidgets();
}
