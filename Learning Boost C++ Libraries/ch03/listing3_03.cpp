#include "Widget.h"
#include <boost/scoped_ptr.hpp>

void makeNewWidget(boost::scoped_ptr<Widget>& result)
{
    result.reset(new Widget);
    result->setProperties("height", 10);
    result->setProperties("width", 10);
}

void makeAndUseWidget()
{
    boost::scoped_ptr<Widget> wgt; // null wgt
    makeNewWidget(wgt);         // wgt set to some Widget object.
    wgt->display();              // widget #1 displayed
    makeNewWidget(wgt);        // wgt reset to some other Widget.
    // Older wgt released.
    wgt->display();            // widget #2 displayed
}

int main()
{
    makeAndUseWidget();
}
