#include <iostream>
#include <vector>
using namespace std;

double greedy(vector<int> client, int s)
{
    vector<int> service(s + 1, 0);
    vector<int> sum(s + 1, 0);
    int n = client.size();
    sort(client.begin(), client.end());
    int i = 0, j = 0;

    while (i < n) {
        service[j] += client[i];
        sum[j] += service[j];
        ++i, ++j;

        if (j == s) j = 0;
    }

    double t = 0;

    for (i = 0; i < s; ++i) t += sum[i];

    t /= n;
    return t;
}

int main()
{
    vector<int> queue;
    int n, s;
    scanf("%d%d", &n, &s);
    int temp;

    for (int i = 0; i < n; i++) {
        scanf("%d", &temp);
        queue.push_back(temp);
    }

    printf("%.3lf\n", greedy(queue, s));
}
