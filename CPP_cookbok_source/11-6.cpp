template<class Iter_T>
double computeMean(Iter_T first, Iter_T last)
{
    return static_cast<double>(accumulate(first, last, 0.0))
           / distance(first, last);
}