//Algo8-2.cpp k路平衡归并类（替代败者树）
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "InsSort.h"
template<typename D>class LoserTree
{
private:
    bool Make;
    int k;
    void append(const string FileName)
    {
        FILE *fp;
        fp = fopen(FileName.c_str(), "at+");
        fseek(fp, -1L, SEEK_END);
        char s = fgetc(fp);

        if (s != '\n') {
            fseek(fp, 0L, SEEK_END);
            fwrite("\n", sizeof(char), 1, fp);
        }

        fclose(fp);
    }
public:
    LoserTree()
    {
        Make = false;
    }
    void MakeSortBigFile(ofstream &fout)
    {
        if (Make) {
            ifstream *finp = new ifstream[k];
            PriType c;
            int i, j = 0;
            string s = "F00.txt";

            for (i = 0; i < k; i++) {
                s[2] = i % 10 + '0';
                s[1] = i / 10 % 10 + '0';
                finp[i].open(s.c_str(), ios::in);
            }

            priority_queue<PriType, vector<PriType>, cmp> pri_queue;

            for (i = 0; i < k; i++) {
                if (InputFromFile(finp[i], c.data))
                    c.number = i;

                pri_queue.push(c);
            }

            while (!pri_queue.empty()) {
                c = pri_queue.top();
                Visit(c.data);
                OutputToFile(fout, c.data);
                pri_queue.pop();

                if (InputFromFile(finp[c.number], c.data))
                    pri_queue.push(c);
            }

            cout << endl;

            for (i = 0; i < k; i++)
                finp[i].close();

            delete[]finp;
        }
    }
    void MakeSmallFile(const string FileName, int N)
    {
        append(FileName);
        ifstream fin_big(FileName.c_str());
        string s = "F00.txt";
        int j;
        k = 0;

        while (!fin_big.eof()) {
            InsSort<D> is;

            for (j = 0; j < N; j++)
                if (!is.Input(fin_big))
                    break;

            if (j > 0) {
                is.InsertSort();
                s[2] = k % 10 + '0';
                s[1] = k / 10 % 10 + '0';
                is.OutputAllToFile(s);
                k++;
            }
        }

        fin_big.close();
        Make = true;
    }
};
#include "Func7-1.h"
#include "Func8-1.h"
struct PriType {
    int number;
    ID data;
};
class cmp
{
public:
    bool operator()(const PriType a, const PriType b)const
    {
        return a.data.key > b.data.key;
    }
};
void main()
{
    LoserTree<ID> lt;
    string FileName;
    int N;
    cout << "请输入待排序的无序大文件名：";
    cin >> FileName;
    cout << "请输入每个小文件最多容纳的数据个数N：";
    cin >> N;
    lt.MakeSmallFile(FileName, N);
    cout << "请输入已排序大文件名：";
    cin >> FileName;
    ofstream fout(FileName.c_str());
    cout << "有序大文件" << FileName << "的数据为" << endl;
    lt.MakeSortBigFile(fout);
    fout.close();
}

