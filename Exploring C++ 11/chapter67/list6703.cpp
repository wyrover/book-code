// Listing 67-3. Specializing iterator_advancer for Random Access Iterators
// Partial specialization for random access iterators
template<>
class iterator_advancer<std::random_access_iterator_tag>
{
public:
   template<class RandomIter, class Distance>
   void operator()(RandomIter& iter, Distance distance)
   {
      trace("iterator_advancer<random_access_iterator_tag>");
      iter += distance;
   }
};
