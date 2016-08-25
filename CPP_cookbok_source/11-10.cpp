template<class Iter_T, class Value_T, class BinOp_T>
Iter_T accumulate(Iter_T begin, Iter_T end, Value_T value, BinOp_T op)
{
    while (begin != end) {
        value = op(value, *begin++)
    }

    return value;
}