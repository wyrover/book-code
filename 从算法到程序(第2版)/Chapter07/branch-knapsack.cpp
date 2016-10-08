#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

#define NUM 100
class object
{
    friend int knapsack(int *, int *, int , int);
public:
    int operator < (object a)const
    {
        return (ratio > a.ratio);
    }
private:
    int ID;
    double ratio;
};

class node
{
    friend class knap;
    friend int knapsack(int *, int *, int , int);
private:
    node *parent;
    bool lchild;
};

class queueNode
{
    friend class knap;
public:
    friend bool operator < (queueNode a, queueNode b)
    {
        if (a.uprofit < b.uprofit) return true;
        else return false;
    }
private:
    int uprofit;
    int profit;
    int weight;
    int level;
    node *ptr;
};

class knap
{
    friend int knapsack(int *, int *, int , int);
public:
    int maxKnapsack();
private:
    priority_queue <queueNode> H;
    int bound(int i);
    void addLiveNode(int up, int cp, int cw, bool ch, int level);
    node *E;
    int c;
    int n;
    int w[NUM];
    int p[NUM];
    int cw;
    int cp;
    int bestx[NUM];
};

int knap::bound(int i)
{
    int cleft = c - cw;
    int b = cp;

    while (i <= n && w[i] <= cleft) {
        cleft -= w[i];
        b += p[i];
        i++;
    }

    if (i <= n) b += 1.0 * cleft * p[i] / w[i];

    return b;
}

void knap::addLiveNode(int up, int cp, int cw, bool ch, int lev)
{
    node *b = new node;
    b->parent = E;
    b->lchild = ch;
    queueNode N;
    N.uprofit = up;
    N.profit = cp;
    N.weight = cw;
    N.level = lev;
    N.ptr = b;
    H.push(N);
}

int knap::maxKnapsack()
{
    int i = 1;
    E = 0;
    cw = cp = 0;
    int bestp = 0;
    int up = bound(1);

    while (i != n + 1) {
        int wt = cw + w[i];

        if (wt <= c) {
            if (cp + p[i] > bestp) bestp = cp + p[i];

            addLiveNode(up, cp + p[i], cw + w[i], true, i + 1);
        }

        up = bound(i + 1);

        if (up >= bestp)
            addLiveNode(up, cp, cw, false, i + 1);

        queueNode N;
        N = H.top();
        H.pop();
        E = N.ptr;
        cw = N.weight;
        cp = N.profit;
        up = N.uprofit;
        i = N.level;
    }

    for (int j = n; j > 0; j--) {
        bestx[j] = E->lchild;
        E = E->parent;
    }

    return cp;
}

int knapsack(int p[], int w[], int c, int n)
{
    int i;
    int bestx[NUM];
    object *Q = new object[n];

    for (i = 0; i < n; i++) {
        Q[i].ID = i + 1;
        Q[i].ratio = 1.0 * p[i] / w[i];
    }

    stable_sort(Q, Q + n);
    knap K;

    for (i = 1; i <= n; i++) {
        K.p[i] = p[Q[i - 1].ID - 1];
        K.w[i] = w[Q[i - 1].ID - 1];
    }

    K.cp = 0;
    K.cw = 0;
    K.c = c;
    K.n = n;
    int bestp = K.maxKnapsack();

    for (i = 1; i <= n; i++)
        bestx[Q[i - 1].ID] = K.bestx[i];

    for (i = 1; i <= n; i++)
        if (bestx[i] == 1) printf("%d ", i);

    printf("\n");
    delete [] Q;
    return bestp;
}

int main()
{
    int w[NUM];
    int p[NUM];
    int c;
    int n;

    while (scanf("%d%d", &c, &n) != EOF) {
        //printf("%d %d\n",c,n);
        for (int i = 0; i < n; i++)
            scanf("%d%d", &w[i], &p[i]);

        printf("%d\n", knapsack(p, w, c, n));
    }

    return 0;
}
