//MuArray.h 用vector实现多维数组的类（MuArray类）
#ifndef _MUARRAY_H_
#define _MUARRAY_H_
template<typename T>class MuArray
{
private:
    vector<T> base;
    int dim;
    vector<int> bounds;
    vector<int>constants;
    bool Locate(va_list ap, int &off)const
    {
        int i, ind;
        off = 0;

        for (i = 0; i < dim; i++) {
            ind = va_arg(ap, int);

            if (ind < 0 || ind >= bounds[i])
                return false;

            off += constants[i] * ind;
        }

        return true;
    }
public:
    void Array(int Dim, ...)
    {
        int elemtotal = 1, i;
        va_list ap;
        assert(Dim > 0);
        dim = Dim;
        va_start(ap, Dim);

        for (i = 0; i < dim; i++) {
            bounds.push_back(va_arg(ap, int));
            assert(bounds[i] > 0);
            elemtotal *= bounds[i];
        }

        va_end(ap);
        base.assign(elemtotal, 0);
        constants.assign(dim, 1);

        for (i = dim - 2; i >= 0; i--)
            constants[i] = bounds[i + 1] * constants[i + 1];
    }
    bool Value(T &e, int n, ...)const
    {
        va_list ap;
        int off;
        va_start(ap, n);

        if (Locate(ap, off) == false)
            return false;

        e = base[off];
        return true;
    }
    bool Assign(T e, ...)
    {
        va_list ap;
        int off;
        va_start(ap, e);

        if (Locate(ap, off) == false)
            return false;

        base[off] = e;
        return true;
    }
};
#endif

