#include <iostream>
#include <deque>
#include <map>
using namespace std;

typedef struct Eight {
    int a[3][3];
    int x, y;
    string way;
} Eight;
int dir[4][2] = { -1, 0, 0, 1, 1, 0, 0, -1};
char cir[4] = {'d', 'l', 'u', 'r'};
map<int, bool> flag;
map<int, string> step;

istream & operator>>(istream & ifs, Eight &eight)
{
    int i, j;
    char c;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++) {
            eight.way = "";
            cin >> c;

            if (c == 'x') {
                eight.x = i;
                eight.y = j;
                eight.a[i][j] = 0;
            } else eight.a[i][j] = c - '0';
        }

    return ifs;
}

int hash(Eight e)
{
    int i, j;
    int sum = 0;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            sum = sum * 10 + e.a[i][j];

    return sum;
}

void bfs(Eight eight)
{
    int index;
    int i, j;
    int count = 1;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            eight.a[i][j] = count++;

    eight.a[2][2] = 0;
    eight.x = 2;
    eight.y = 2;
    flag[1234567890] = true;
    deque<Eight> Q;
    Q.push_back(eight);

    while (!Q.empty()) {
        Eight now = Q.front();
        Q.pop_front();
        int nx, ny;

        for (i = 0; i < 4; i++) {
            nx = now.x + dir[i][0];
            ny = now.y + dir[i][1];
            eight = now;

            if (nx >= 0 && ny >= 0 && nx < 3 && ny < 3) {
                eight.a[now.x][now.y] = eight.a[nx][ny];
                eight.a[nx][ny] = 0;
                index = hash(eight);

                if (flag[index] == 0) {
                    flag[index] = true;
                    eight.x = nx;
                    eight.y = ny;
                    eight.way += cir[i];
                    step[index] = eight.way;
                    Q.push_back(eight);
                }
            }
        }
    }
}

int main()
{
    int i;
    Eight eight;
    bfs(eight);

    while (cin >> eight) {
        int index = hash(eight);

        if (flag[index] == 1) {
            string s = step[index];

            for (i = s.length() - 1; i >= 0; i--) cout << s[i];

            cout << endl;
        } else cout << "unsolvable" << endl;
    }

    return 0;
}
