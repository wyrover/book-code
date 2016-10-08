#include <iostream>
using namespace std;

string names[400];
int numName;
int numReceps;
int numAdapts;
int matchPlug[100];
int matchRecep[100];
bool canUse[100];

struct plug {
    int recep;
    bool list[400];
} plugs[100];

struct adapter {
    int recep, plug;
} adapts[100];

int findName(string name)
{
    for (int i = 0; i < numName; i++)
        if (name == names[i]) return i;

    names[numName++] = name;
    return numName - 1;
}

void useAdapter(int i)
{
    plugs[i].list[plugs[i].recep] = true;
    bool flag;

    do {
        flag = false;

        for (int j = 0; j < numAdapts; j++) {
            if (plugs[i].list[adapts[j].recep] &&
                !plugs[i].list[adapts[j].plug]) {
                flag = true;
                plugs[i].list[adapts[j].plug] = true;
            }
        }
    } while (flag);
}

bool backtrack(int node)
{
    for (int j = 0; j < numReceps; j++) {
        if (plugs[node].list[j] && canUse[j]) {
            if (matchRecep[j] == -1) {
                matchPlug[node] = j;
                matchRecep[j] = node;
                return true;
            } else {
                int save = matchRecep[j];
                matchRecep[j] = node;
                matchPlug[save] = -1;
                matchPlug[node] = j;
                canUse[j] = false;

                if (backtrack(save))
                    return true;

                canUse[j] = true;
                matchPlug[node] = -1;
                matchPlug[save] = j;
                matchRecep[j] = save;
            }
        }
    }

    return false;
}

int main()
{
    int numPlugs;
    string name;
    int N;
    scanf("%d", &N);

    while (N--) {
        cin >> numReceps;

        for (int i = 0; i < numReceps; i++)
            cin >> names[i];

        numName = numReceps;
        cin >> numPlugs;

        for (int i = 0; i < numPlugs; i++) {
            cin >> name >> name;
            plugs[i].recep = findName(name);
            memset(plugs[i].list, false, sizeof(plugs[i].list));
        }

        cin >> numAdapts;

        for (int i = 0; i < numAdapts; i++) {
            cin >> name;
            adapts[i].recep = findName(name);
            cin >> name;
            adapts[i].plug = findName(name);
        }

        for (int i = 0; i < numPlugs; i++)
            useAdapter(i);

        memset(matchPlug, 0xff, sizeof(matchPlug));
        memset(matchRecep, 0xff, sizeof(matchRecep));
        int i = 0;

        while (i < numPlugs) {
            for (int j = 0; j < numReceps; j++)
                canUse[j] = true;

            if (matchPlug[i] == -1 && backtrack(i)) i = 0;
            else  i++;
        }

        int min = 0;

        for (int i = 0; i < numPlugs; i++)
            if (matchPlug[i] == -1) min++;

        cout << min << endl;

        if (N) printf("\n");
    }

    return 0;
}
