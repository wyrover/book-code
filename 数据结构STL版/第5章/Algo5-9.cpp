//Algo5-9.cpp 调用弗洛伊德类求每两点之间的最短路径
#include "C5-1.h"
#include "Floyd.h"
#include "Func5-1.h"
#include "Func5-2.h"
void main()
{
    Floyd<V, A> f;
    f.CreateFromFile("F5-13.txt");
    f.Display();
    f.ShortestPath_FLOYD();
    f.DisplayD();

    for (int i = 0; i < f.GetVexNumOrArcNum(true); i++)
        for (int j = 0; j < f.GetVexNumOrArcNum(true); j++) {
            f.DisplayP(i, j);
            f.Path(i, j);
        }
}

