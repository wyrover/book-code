#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;

void parse(vector<set<int> > &adj, unsigned int p = 0)
{
    unsigned int x;
    cin >> x;

    if (p) {
        adj[p].insert(x);
        adj[x].insert(p);
    }

    while (true) {
        char ch;
        cin >> ch;

        if (ch == ')') break;

        parse(adj, x);
    }

    return;
}

int main()
{
    char ch;

    while (cin >> ch) {
        vector<set<int> > adj(1024, set<int>());
        parse(adj);
        priority_queue< int, vector<int>, greater<int> > leafs;
        int n = 0;

        for (unsigned int i = 0; i < adj.size(); i++)
            if (adj[i].size()) {
                n++;

                if (adj[i].size() == 1)
                    leafs.push(i);
            }

        for (int k = 1; k < n; k++) {
            unsigned int x = leafs.top();
            leafs.pop();
            unsigned int p = *(adj[x].begin());

            if (k > 1)
                cout << " ";

            cout << p;
            adj[p].erase(x);

            if (adj[p].size() == 1)
                leafs.push(p);
        }

        cout << endl;
    }

    return 0;
}
