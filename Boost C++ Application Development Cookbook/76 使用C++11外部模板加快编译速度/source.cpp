#include "header.h"

#ifndef BOOST_NO_CXX11_EXTERN_TEMPLATE
    template class boost::variant <
    boost::blank,
    int,
    std::string,
    double
    >;
#endif