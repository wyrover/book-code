#ifndef ADVANCE_HPP_
#define ADVANCE_HPP_

// Listing 67-1. One Possible Implementation of std::advance
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

void trace(std::string const& msg)
{
   std::cout << msg << '\n';
}

// Default implementation: advance the slow way
template<class Kind>
class iterator_advancer
{
public:
   template<class InIter, class Distance>
   void operator()(InIter& iter, Distance distance)
   {
      trace("iterator_advancer<>");
      for ( ; distance != 0; --distance)
         ++iter;
   }
};

// Partial specialization for bi-directional iterators
template<>
class iterator_advancer<std::bidirectional_iterator_tag>
{
public:
   template<class BiDiIter, class Distance>
   void operator()(BiDiIter& iter, Distance distance)
   {
      trace("iterator_advancer<bidirectional_iterator_tag>");
      if (distance < 0)
         for ( ; distance != 0; ++distance)
            --iter;
      else
         for ( ; distance != 0; --distance)
            ++iter;
   }
};

template<class InIter, class Distance>
void my_advance(InIter& iter, Distance distance)
{
    iterator_advancer<typename std::iterator_traits<InIter>::iterator_category>{}

        (iter, distance);
}

#endif

