//Tree Recovery
#include <iostream>
#include <string>
using namespace std;

string strPost;

//已知前序中序求后序
void PostOrder(string pre, string in)
{
    int iPos;

    if (pre == "" || in == "") return;

    iPos = in.find(pre[0]);
    PostOrder(pre.substr(1, iPos), in.substr(0, iPos));
    PostOrder(pre.substr(iPos + 1), in.substr(iPos + 1));
    strPost += pre[0];
}

int main()
{
    string pre, in;

    while (cin >> pre >> in) {
        strPost = "";
        PostOrder(pre, in);
        cout << strPost << endl;
    }

    return 0;
}
