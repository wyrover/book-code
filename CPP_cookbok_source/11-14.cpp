template<class Iter_T, class Fxn_T>
void generate(Iter_T first, Iter_T last, Fxn_T f)
{
    while (first != last) *first++ = f();
}

template<class Iter_T, class Fxn_T>
void generate_n(Iter_T first, int n, Fxn_T f)
{
    for (int i = 0; i < n; ++i) *first++ = f();
}