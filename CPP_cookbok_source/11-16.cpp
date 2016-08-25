template<class T>
T* valarray_begin(valarray<T>& x)
{
    return &x[0];
}

template<class T>
T* valarray_end(valarray<T>& x)
{
    return valarray_begin(x) + x.size();
}