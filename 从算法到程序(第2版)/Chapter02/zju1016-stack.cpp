#include <iostream>
#include <stack>
using namespace std;

stack<int> S;

int main()
{
    int iCase;
    cin >> iCase;

    while (iCase--) {
        int n;
        cin >> n;
        int p;
        int t = 0;

        for (int i = 1; i <= n; i++) {
            cin >> p;

            for (int j = 1; j <= p - t; j++)
                S.push(-1);

            if (S.top() == -1) {
                i == 1 ? cout << "1" : cout << " 1";
                S.pop();
                S.push(1);
            } else {
                int iTop = S.top();
                S.pop();

                while (S.top() != -1) {
                    iTop += S.top();
                    S.pop();
                }

                cout << " " << iTop + 1;
                S.pop();
                S.push(iTop + 1);
            }

            t = p;
        }

        cout << endl;

        while (!S.empty()) S.pop();
    }

    return 0;
}
