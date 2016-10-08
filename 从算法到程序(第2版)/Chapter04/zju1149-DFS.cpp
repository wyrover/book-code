#include<iostream>
#include<stdio.h>
using namespace std;

int a[7];
bool flag;
void DFS(int half, int fetch)
{
    if (half == fetch) {
        flag = true;
        return;
    }

    if (fetch < half) {
        for (int i = 6; i >= 1; i--) {
            if (a[i]) {
                fetch += i;
                a[i]--;

                if (fetch > half) {
                    a[i]++;
                    fetch -= i;
                    continue;
                }

                DFS(half, fetch);
            }
        }
    }
}

int main()
{
    int iCase = 1;

    while (1) {
        int sum = 0;

        for (int i = 1; i <= 6; i++) {
            cin >> a[i];
            sum += a[i] * i;
        }

        if (sum == 0) break;

        cout << "Collection #" << iCase++ << ':' << endl;

        if (sum % 2) {
            cout << "Can't be divided." << endl << endl;
            continue;
        }

        flag = false;
        DFS(sum / 2, 0);

        if (flag) cout << "Can be divided." << endl << endl;
        else cout << "Can't be divided." << endl << endl;
    }

    return 0;
}
