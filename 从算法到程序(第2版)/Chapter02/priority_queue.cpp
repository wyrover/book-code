#include <iostream>
#include <queue>
using namespace std;

struct info {
    string name;
    float score;
    bool operator < (const info &a) const
    {
        return a.score < score;
    }
};

int main()
{
    priority_queue <info> pq;
    info in;
    in.name = "Jack";
    in.score = 68.5;
    pq.push(in);
    in.name = "Bomi";
    in.score = 18.5;
    pq.push(in);
    in.name = "Peti";
    in.score = 90;
    pq.push(in);

    while (!pq.empty()) {
        cout << pq.top().name << ": " << pq.top().score << endl;
        pq.pop();
    }

    return 0;
}
