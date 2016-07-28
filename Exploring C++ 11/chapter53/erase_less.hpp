#ifndef ERASE_LESS_HPP_
#define ERASE_LESS_HPP_

// Listing 53-3. Erasing Elements from a Sequence Container
template<class Container>
void erase_less(Container& cont)
{
  typename Container::iterator prev{cont.end()};
  typename Container::iterator iter{cont.begin()};
  while (iter != cont.end())
  {
    if (prev != cont.end() and not (*prev < *iter))
      iter = cont.erase(iter);
    else
    {
      prev = iter;
      ++iter;
    }
  }
}

#endif
