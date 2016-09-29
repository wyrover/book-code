//Algo2-7.cpp 用递归函数和非递归函数求解马踏棋盘问题
#include "C.h"
#include "SqStack.h"
#include "PosDirect.h"
const PosType HTry[8] = {{2, -1}, {2, 1}, {1, 2}, { -1, 2}, { -2, 1}, { -2, -1},
    { -1, -2}, {1, -2}
};
class HORSE
{
private:
    int N;
    vector<vector<int> > Chessboard;
    ofstream fout;
    void Init(int n)
    {
        N = n;
        Chessboard.assign(N + 4);

        for (int i = 0; i < N + 4; i++) {
            Chessboard[i].assign(N + 4, 0);

            for (int j = 0; j < N + 4; j++)
                if (i < 2 || i > N + 1 || j < 2 || j > N + 1)
                    Chessboard[i][j] = -1;
        }
    }
    void HorseByRecursion(PosType Now, int count, int &number)
    {
        PosType Next;

        for (int m = 0; m < 8; m++) {
            Next = Now + HTry[m];

            if (Chessboard[Next.row][Next.col] == 0) {
                Chessboard[Next.row][Next.col] = ++count;

                if (count < N * N)
                    HorseByRecursion(Next, count, number);
                else
                    PrintToFile(++number);

                Chessboard[Next.row][Next.col] = 0;
                count--;
            }
        }
    }
    void PrintToFile(int number = 1)
    {
        fout << "解" << number << endl;

        for (int k = 0; k < N; k++) {
            for (int n = 0; n < N; n++)
                fout << setw(3) << Chessboard[k + 2][n + 2];

            fout << endl;
        }
    }
public:
    void HorseByStack(int n, PosType star, string FileName)
    {
        Init(n);
        PosType p = {1, 1};
        star = star + p;
        fout.open(FileName.c_str(), ios::out);
        PosDirect First;
        First.direct = 0;
        First.row = star.row;
        First.col = star.col;
        fout << "利用栈求马踏棋盘的一个解：" << endl;
        int count = 1;
        bool flag;
        PosDirect next, now;
        SqStack<PosDirect> S(8);
        S.Push(First);
        Chessboard[First.row][First.col] = 1;

        while (!S.StackEmpty()) {
            S.Pop(now);
            flag = false;

            while (now.direct < 8) {
                next.row = now.row + HTry[now.direct].row;
                next.col = now.col + HTry[now.direct].col;

                if (Chessboard[next.row][next.col] == 0) {
                    Chessboard[next.row][next.col] = ++count;
                    flag = true;
                    now.direct++;
                    S.Push(now);
                    next.direct = 0;
                    S.Push(next);

                    if (count == N * N) {
                        PrintToFile();
                        fout.close();
                        return;
                    }

                    break;
                } else
                    now.direct++;
            }

            if (!flag) {
                count--;
                Chessboard[now.row][now.col] = 0;
            }
        }

        fout.close();
    }
    void HorseByRecursion(int n, PosType star, string FileName)
    {
        Init(n);
        int count = 0, number = 0;
        PosType p = {1, 1};
        star = star + p;
        fout.open(FileName.c_str(), ios::out);
        clock_t tstart = clock();
        Chessboard[star.row][star.col] = 1;
        fout << "用递归方法求马踏棋盘的所有解：" << endl;
        HorseByRecursion(star, 1, number);
        double t = (clock() - tstart);
        fout.close();
        cout << "共有" << number << "个解，用时" << t << "毫秒。" << endl;
    }
};
void main()
{
    HORSE h;
    PosType Start;
    Start.row = 1;
    Start.col = 3;
    h.HorseByStack(5, Start, "马踏棋盘1.txt");
    h.HorseByRecursion(5, Start, "马踏棋盘2.txt");
}

