#include <stdio.h>
#include <memory.h>
#include <algorithm>
using namespace std;

struct stBoard {
    int number, length;
    stBoard()
    {
        number = 1000;   //木板数量的最优值，初值为∞
    }
} back[4096];   //用于回溯的数组
bool operator < (const stBoard &op1, const stBoard &op2)
{
    if (op1.number == op2.number)
        return op1.length > op2.length;

    return op1.number < op2.number;
}
int part[13];   //要切割的木料
int board, saw; //木板的长度，锯的宽度
int n;          //木料的数量

void backtrack(int a)
{
    //如果该木材已经安排过（记忆式搜索）
    if (back[a].number >= 0) return;

    stBoard best, now;

    //安排所有的木料
    for (int i = 0; i < n; i++)

        //第i个木料尚未安排
        if (a & 1 << i) {
            //（1）除掉第i个木料（^是异或运算）
            int index = a ^ 1 << i;
            //（2）递归回溯，安排其余木料
            backtrack(index);
            //（3）获得安排其余木料需要木板的数量
            now.number = back[index].number;

            // （4）如果剩余长度back[index].length能够安排第i个木料，
            if (back[index].length >= part[i])
                //调整剩余木料的长度
                now.length = back[index].length - part[i];
            else {  //不能安排第i个木料
                //增加1个木料
                now.number++;
                now.length = board - part[i];
            }

            //调整剩余木料的长度
            now.length = max(now.length - saw, 0);
            //（5）调整最优值
            best = min(best, now);
        }

    back[a] = best; //保存最优值
}

int main()
{
    printf("Problem 7 by team x\n");
    char c;

    while (scanf("%d", &board) != EOF) { //读取木板的长度
        memset(back, 255, sizeof(back));
        scanf("%d", &saw);              //读取木板的宽度
        n = 0;

        while ((c = getc(stdin)) != '\n') {
            ungetc(c, stdin);   //将字符c退回缓冲区
            scanf("%d", part + n++);
        }

        back[0].number = back[0].length = 0;
        backtrack((1 << n) - 1);
        printf("\n");
        printf("Board length            =%6d\n", board);
        printf("Saw width               =%6d\n", saw);
        printf("Number of boards needed =%6d\n", back[(1 << n) - 1].number);
    }

    printf("End of problem 7 by team x\n");
    return 0;
}
