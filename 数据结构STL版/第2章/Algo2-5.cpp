//Algo2-5.cpp 用递归函数和非递归函数求解迷宫问题
#include "C.h"
#include "SqStack.h"
#include "PosDirect.h"
const PosType direc[4] = {{1, 0}, {0, 1}, {0, -1}, { -1, 0}};
//移动方向数组。{行增量, 列增量}，移动方向依次为南东西北
class MAZE
{
private:
    bool flag;
    vector<vector<int> > maze, m;

    int row, col;
    int count;
    PosType begin, end;
    void Try(PosType curpos, int curstep)
    {
        PosType nextpos;

        for (int i = 0; i <= 3; i++) {
            nextpos = curpos + direc[i];

            if (maze[nextpos.row][nextpos.col] == -1) {
                maze[nextpos.row][nextpos.col] = ++curstep;

                if (nextpos == end) {
                    cout << "解" << ++count << endl;
                    Print(maze);
                } else
                    Try(nextpos, curstep);

                maze[nextpos.row][nextpos.col] = -1;
                curstep--;
            }
        }
    }
    void Print(vector<vector<int> > mm)const
    {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++)
                cout << setw(3) << mm[i][j];

            cout << endl;
        }
    }
public:
    MAZE()
    {
        flag = false;
    }
    void CreatMazeFromFile(string FileName)
    {
        ifstream fin(FileName.c_str());
        fin >> row >> col;
        m.assign(row);

        for (int i = 0; i < row; i++)
            m[i].assign(col);

        for (i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
                fin >> m[i][j];

        cout << "迷宫布局如下：" << endl;
        Print(m);
        fin >> begin.row >> begin.col;
        cout << "迷宫的起点：(" << begin.row << ", " << begin.col << ")，";
        fin >> end.row >> end.col;
        cout << " 终点：(" << end.row << ", " << end.col << ')' << endl;
        fin.close();
        flag = true;
    }
    void MazePathByStack()
    {
        if (flag) {
            maze = m;
            SqStack<PosDirect> S(8);
            PosDirect e;
            PosType curpos = begin;
            int curstep = 1;

            do {
                if (maze[curpos.row][curpos.col] == -1) {
                    maze[curpos.row][curpos.col] = curstep;
                    e.row = curpos.row;
                    e.col = curpos.col;
                    e.direct = 0;
                    S.Push(e);
                    curstep++;

                    if (curpos == end) {
                        cout << "此迷宫从入口到出口的一条路径如下：" << endl;
                        Print(maze);
                        return;
                    } else
                        curpos = curpos + direc[e.direct];
                } else if (!S.StackEmpty()) {
                    S.Pop(e);
                    curstep--;

                    while (e.direct == 3 && !S.StackEmpty()) {
                        maze[e.row][e.col] = -2;
                        S.Pop(e);
                        curstep--;
                    }

                    if (e.direct < 3) {
                        e.direct++;
                        S.Push(e);
                        curstep++;
                        curpos.row = e.row + direc[e.direct].row;
                        curpos.col = e.col + direc[e.direct].col;
                    }
                }
            } while (!S.StackEmpty());

            cout << "此迷宫没有从入口到出口的路径" << endl;
        }
    }
    void MazePathByRecursion()
    {
        if (flag) {
            maze = m;
            maze[begin.row][begin.col] = 1;
            count = 0;
            Try(begin, 1);
        }
    }
};
void main()
{
    MAZE M;
    M.CreatMazeFromFile("F2-3.txt");
    cout << "利用栈求迷宫的一个解：" << endl;
    M.MazePathByStack();
    cout << "递归求迷宫的所有解：" << endl;
    M.MazePathByRecursion();
}

