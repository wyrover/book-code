#include <stdio.h>
#include <memory.h>

#define maxNode 14000

int tree[maxNode];
char ch;
int complete;

int binaryTree()
{
    int i;
    int data;
    int p;

    while (scanf("%c", &ch) != EOF) {
        if (ch == '(') {
            data = 0;

            while (1) {
                scanf("%c", &ch);

                if (ch == ')' && data == 0) {
                    for (i = 2; i < maxNode && complete; i++)
                        if (tree[i] > 0 && tree[i >> 1] == 0)
                            complete = 0;

                    if (complete == 0) printf("not complete\n");
                    else {
                        printf("%d", tree[1]);

                        for (i = 2; i < maxNode; i++)
                            if (tree[i] > 0) printf(" %d", tree[i]);

                        printf("\n");
                    }

                    return 1;
                }

                if (ch == ',' && data == 0) complete = 0;

                if (ch == ',') break;

                data = data * 10 + ch - '0';
            }

            p = 1;

            while (1) {
                scanf("%c", &ch);

                if (ch == ')') break;

                if (ch == 'L') p *= 2;
                else if (ch == 'R') p = p * 2 + 1;
            }

            if (tree[p] > 0) complete = 0;

            tree[p] = data;
        }
    }

    return 0;
}

int main()
{
    while (1) {
        memset(tree, 0, sizeof(tree));
        complete = 1;

        if (!binaryTree()) break;
    }

    return 0;
}
