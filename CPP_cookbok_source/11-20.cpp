template<class In, class In2, class T, class BinOp, class BinOp2>
T inner_product(In first, In last, In2 first2, T init, BinOp op, Binop2 op2)
{
    while (first != last) {
        BinOp(init, BinOp2(*first++, *first2++));
    }

    return init;
}