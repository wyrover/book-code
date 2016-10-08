#include <cstdio>
#include <iostream>
#include <queue>
using namespace std;

struct point {
    int x, y;
    int c;
} from, to;
int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};

void bfs(char src[], char dist[])
{
    queue<point> q;
    from.x = src[0] - 'a';
    from.y = src[1] - '1';
    from.c = 0;
    to.x = dist[0] - 'a';
    to.y = dist[1] - '1';
    q.push(from);
    point temp;

    while (true) {
        from = q.front();
        q.pop();

        if (from.x == to.x && from.y == to.y) break;

        for (int i = 0; i < 8; i++) {
            temp.x = from.x + dx[i];
            temp.y = from.y + dy[i];
            temp.c = from.c + 1;

            if (temp.x < 0 || temp.x > 7 ||
                temp.y < 0 || temp.y > 7) continue;

            q.push(temp);
        }
    }
}

int main()
{
    char src[3];
    char dist[3];

    while (scanf("%s%s", src, dist) != EOF) {
        bfs(src, dist);
        printf("To get from %s to %s ", src, dist);
        printf("takes %d knight moves.\n", from.c);
    }
}
