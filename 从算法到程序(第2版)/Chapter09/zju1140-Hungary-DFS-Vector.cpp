#include <iostream>
#include <cstring>
#include <vector>
#include <cstdio>
using namespace std;

vector<int> map[105];
int p, n;
bool use[305];
int match[305];

bool dfs(int x)
{
    for (int i = 0; i < map[x].size(); ++i) {
        if (!use[map[x][i]]) {
            use[map[x][i]] = true;

            if (!match[map[x][i]] || dfs(match[map[x][i]])) {
                match[map[x][i]] = x;
                return true;
            }
        }
    }

    return false;
}

int hungary()
{
    int course = 0;
    memset(match, 0, sizeof(match));

    for (int i = 1; i <= p; ++i) {
        memset(use, 0, sizeof(use));

        if (dfs(i)) course++;
    }

    return course;
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        memset(map, 0, sizeof(map));
        scanf("%d%d", &p, &n);
        int num, student;

        for (int i = 1; i <= p; ++i) {
            scanf("%d", &num);

            while (num--) {
                scanf("%d", &student);
                map[i].push_back(student);
            }
        }

        printf("%s\n", hungary() == p ? "YES" : "NO");
    }

    return 0;
}
