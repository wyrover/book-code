//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused
#include "stdio.h"
#include "string.h"
#include "conio.h"
#include <iostream>
#include <StrUtils.hpp>
#include "search.h"
#define STAS_COUNT 10 //统计次数

DWORD T1, T2;

LARGE_INTEGER litmp;
long QPart1, QPart2;

float dfMinus, dfFreq, dfTim;
float time_coust[8][STAS_COUNT];

void write_test_file( char * filename )
{
    using namespace std;
    FILE * in;

    if ((in = fopen(filename, "a+")) == NULL)
    {
        cout << " 打开文件失败\n";
        getch();
        return;
    }

    char buf [] = "test...";

    for (int i = 0; i<5000000; i++)
    {
        fwrite(buf, strlen(buf), 1, in);
        fwrite("\r\n", 2, 1, in);
    }

    fwrite("hellotest", 9, 1, in);
    fclose(in);
    cout << "写入完成.\n";
}

void start_time()
{
    using namespace std;
    //cout<<"start time:";
    //cout<<FormatDateTime("hh:nn:ss zzz",Time()).c_str();
    //cout<<"\n";
    QueryPerformanceFrequency(& litmp); // 获得计数器的时钟频率
    dfFreq = (double)litmp.QuadPart;
    QueryPerformanceCounter(& litmp);
    QPart1 = litmp.QuadPart;
}

void stop_time( float * temp_time_cost )
{
    using namespace std;
    QueryPerformanceCounter(& litmp);
    QPart2 = litmp.QuadPart;
    dfMinus = (double)(QPart2 - QPart1);
    dfTim = dfMinus / dfFreq;
    //cout<<"stop time:";
    //cout<<FormatDateTime("hh:nn:ss zzz",Time()).c_str();
    cout << "\n";
    cout << "time cost:";
    cout << FormatFloat("#.##", dfTim * 10000).c_str();
    * temp_time_cost = dfTim * 10000;
    cout << " ms\n---------------------------------------------\n";
}

void main()
{
    using namespace std;

    TStringList * TempList = new TStringList();
    cout << "start...\n";

    try
    {
        TempList->LoadFromFile("./test/test.hex");

        AnsiString S = TempList->Text; //提前赋值

        char p [] = "hellotest";
        int Index = -1;
        //开始kmp
        Sleep(500);

        for (int count = 0; count<STAS_COUNT; count++)
        {
            //开始简单模式查找
            start_time();
            Index = simple_match(S.c_str(), p);
            cout << "simple search:找到位置为:(以0开头的序列)";
            cout << Index;
            cout << "\n";
            stop_time(& time_coust[0][count]);
            //结束简单模式查找
            start_time();
            char * ptr = strstr(S.c_str(), p);
            cout << "strstr.\n";
            stop_time(& time_coust[1][count]);

            //开始AnsiString.pos的查找
            start_time();
            Index = S.Pos("hellotest");
            cout << "AnsiString.pos:找到位置为:(以0开头的序列)";
            cout << Index - 1;
            cout << "\n";
            stop_time(& time_coust[2][count]);
            //结束 AnsiString.pos的查找

            //开始区分大小写的查找
            start_time();
            AnsiString SubS1, SubS2;
            SubS1 = "hellotest";
            SubS1 = SubS1.LowerCase();
            SubS2 = SubS1.UpperCase();
            Index = Search_NoCase(S.c_str(), SubS1.c_str(), SubS2.c_str(), SubS1.Length());
            cout << "Search_NoCase:找到位置为:(以0开头的序列)";
            cout << Index;
            cout << "\n";
            stop_time(& time_coust[3][count]);
            //结束区分大小写的查找

            start_time();
            int next[50];
            kmp_get_next(p, next);
            printf("kmp:找到位置为 %d (以0开头的序列)\n", kmp(S.c_str(), p, 0, next));
            stop_time(& time_coust[4][count]);
            //结束kmp

            //开始朴素匹配
            start_time();
            printf("pusu:找到位置为 %d (以0开头的序列)\n", pusu_search(S.c_str(), p, 0));
            stop_time(& time_coust[5][count]);

            //开始不区分大小写的查找

            start_time();
            Index = Search_HaveCase(S.c_str(), p, strlen(p));
            cout << "Search_HaveCase:找到位置为:(以0开头的序列)";
            cout << Index;
            cout << "\n";
            stop_time(& time_coust[6][count]);
            //结束不区分大小写的查找

            //另外一种Search算法
            start_time();
            Search patt(p);
            patt.find(S.c_str());
            cout << "B.M算法.\n";
            stop_time(& time_coust[7][count]);
        } //end for

        delete TempList;
        cout << "--------统计次数:";
        cout << STAS_COUNT;
        cout << ",求平均值后的统计结果如下:\n";
        int i, j;

        for (i = 0; i<8; i++)
        {
            float average = 0;

            for (j = 0; j<STAS_COUNT; j++)
            {
                //cout<<time_coust[i][j];
                average += time_coust[i][j];
            //cout<<" ";
            }
            //cout<<"\n";

            cout << "-----算法:";
            cout << i;
            cout << "平均查找时间:";
            cout << average / STAS_COUNT;
            cout << " ms----------------\n";
        }
    }
    catch (...)
    {
        delete TempList;
        cout << "创建文件\n";
        write_test_file("./test/test.hex");
    }

    getch();
}
