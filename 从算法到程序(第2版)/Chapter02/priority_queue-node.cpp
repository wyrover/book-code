#include<iostream>
#include<vector>
#include<queue>
using namespace std;

struct node {
    int x;
    int y;
    int w;
    friend bool operator> (const node& a, const node& b)
    {
        return (a.w > b.w);
    }
    friend bool operator< (const node& a, const node& b)
    {
        return (a.w < b.w);
    }
};

int main()
{
    int i;
    priority_queue < node, vector<node>, greater<node> > que1;
    priority_queue < node, vector<node>, less<node> > que2;
    node da[5];

    for (i = 0; i < 5; i++) {
        da[i].x = i;
        da[i].y = i;
        da[i].w = i;
        que1.push(da[i]);
        que2.push(da[i]);
    }

    while (!que1.empty()) {
        cout << que1.top().x << " " << que1.top().y << " " << que1.top().w << endl;
        que1.pop();
    }

    cout << endl;

    while (!que2.empty()) {
        cout << que2.top().x << " " << que2.top().y << " " << que2.top().w << endl;
        que2.pop();
    }

    cout << endl;
    return 0;
}
