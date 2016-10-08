#include <queue>
#include <algorithm>
#include <iostream>
using namespace std;

int n;
int m;
int a[20];
struct node {
    int num;
    string digit;
};

void bfs()
{
    queue <node> q;
    node cur;
    cur.num = 0;
    cur.digit = "";
    q.push(cur);
    bool r[5000] = {false};
    bool find = false;

    while (!q.empty()) {
        cur = q.front();
        q.pop();

        for (int i = 0; i < m; i++) {
            int x = cur.num * 10 + a[i];

            if (x == 0) continue;

            node E;

            if (!r[x % n]) {
                r[x % n] = true;
                E.num = x % n;
                E.digit = cur.digit + char(a[i] + '0');
                q.push(E);
            }

            if (r[0]) {
                cout << E.digit << endl;
                find = true;
                break;
            }
        }

        if (find) break;
    }

    if (!find) cout << 0 << endl;

    return;
}

int main()
{
    while (cin >> n >> m) {
        for (int i = 0; i < m; i++)
            cin >> a[i];

        sort(a, a + m);

        if (n == 0) cout << 0 << endl;
        else bfs();
    }

    return 0;
}
