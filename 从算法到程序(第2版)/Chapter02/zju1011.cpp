#include<iostream>
#include<vector>
using namespace std;

struct signal {
    int left, right;
};
vector<signal> table[20][20];

int n, m, k;
int treeLevel;
char tree[1000];
int treeDeep;

void readTable()
{
    char ch;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < k; j++) {
            table[i][j].clear();

            while (true) {
                signal pair;
                scanf("%d%d", &pair.left, &pair.right);
                table[i][j].push_back(pair);
                ch = getchar();

                if (ch == '\n') break;
            }
        }
}

void readTree()
{
    char ch;
    treeDeep = 0;
    int i, j;

    for (i = 0; i <= treeLevel; i++)
        for (j = 0; j < (1 << i); j++) {
            cin >> ch;
            tree[treeDeep] = ch;
            treeDeep++;
        }
}
bool judge(int signal , int node)
{
    int signal1, signal2;

    if (tree[node] == '*' || node >= treeDeep)
        if (signal < n - m) return false;
        else return true;

    int k1 = tree[node] - 'a';
    int left = node * 2 + 1;
    int right = left + 1;

    for (int i = 0; i < table[signal][k1].size(); i++) {
        signal1 = table[signal][k1][i].left;
        signal2 = table[signal][k1][i].right;

        if (judge(signal1, left) && judge(signal2, right)) return true;
    }

    return false;
}

int main()
{
    int iCase = 0;

    while (scanf("%d%d%d", &n, &m, &k) && (n || m || k)) {
        if (iCase++) printf("\n");

        printf("NTA%d:\n", iCase);
        readTable();

        while (scanf("%d", &treeLevel) && treeLevel != -1) {
            readTree();

            if (judge(0, 0)) printf("Valid\n");
            else printf("Invalid\n");
        }
    }

    return 0;
}
