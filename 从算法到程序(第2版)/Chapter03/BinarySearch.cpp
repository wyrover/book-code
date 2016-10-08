template<class Type>
int BinarySearch(Type a[], const Type& x, int n)
{
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int middle = (left + right) / 2;

        if (x == a[middle]) return middle;

        if (x > a[middle]) left = middle + 1;
        else right = middle - 1;
    }

    return -1;
}
