#include <map>
#include <stack>
#include <iostream>
using namespace std;

struct Node {
    int row, col;
};

int main()
{
    int n;
    char name;
    map<char, Node> matrix;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> name;
        cin >> matrix[name].row >> matrix[name].col;
    }

    string exp;

    while (cin >> exp) {
        int i;
        int count = 0;
        stack<Node> array;

        for (i = 0; i < exp.size(); i++) {
            if (exp[i] == '(')  continue;

            if (exp[i] == ')') {
                Node b = array.top();
                array.pop();
                Node a = array.top();
                array.pop();

                if (a.col != b.row) {
                    cout << "error" << endl;
                    break;
                }

                count += a.row * b.row * b.col;
                Node tmp = {a.row, b.col};
                array.push(tmp);
            } else  array.push(matrix[exp[i]]);
        }

        if (i == exp.size())
            cout << count << endl;
    }

    return 0;
}
