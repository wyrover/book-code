#include <string>
#include <iostream>

const static int MAGIC_NUMBER = 4;

template<typename T>
void join(T* arr[], size_t n, T c, std::basic_string<T>& s)
{
    s.clear();

    for (int i = 0; i < n; ++i) {
        if (arr[i] != NULL)
            s += arr[i];

        if (i < n - 1)
            s += c;
    }
}

int main()
{
    std::wstring ws;
    wchar_t* arr[MAGIC_NUMBER];
    arr[0] = L"you";
    arr[1] = L"ate";
    arr[2] = L"my";
    arr[3] = L"breakfast";
    join(arr, MAGIC_NUMBER, L'/', ws);
}
