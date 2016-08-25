#include <iostream.h>
void ExchangeSort(int* pData, int Count)
{
    int iTemp;

    for (int i = 0; i < Count - 1; i++) {
        for (int j = i + 1; j < Count; j++) {
            if (pData[j] < pData[i]) {
                iTemp = pData[i];
                pData[i] = pData[j];
                pData[j] = iTemp;
            }
        }
    }
}

void main()
{
    int data[] = {10, 9, 8, 7, 6, 5, 4};
    ExchangeSort(data, 7);

    for (int i = 0; i < 7; i++)
        cout << data[i] << " ";

    cout << "\n";
}

