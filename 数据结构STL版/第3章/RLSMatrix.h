//RLSMatrix.h 三元组行逻辑链接顺序表的类（RLSMatrix类）
#ifndef _RLSMATRIX_H_
#define _RLSMATRIX_H_
template<typename T>struct Triple {
    int i, j;
    T e;
};
template<typename T>class RLSMatrix
{
private:
    vector<Triple<T> >data;
    vector<int> rpos;
    int col;
public:
    RLSMatrix()
    {
        col = 0;
    }
    void ClearSMatrix()
    {
        data.clear();
        rpos.clear();
        col = 0;
    }
    void MakeMatrixFromFile(string FileName)
    {
        int i, j, k;
        ifstream fin(FileName.c_str());
        fin >> k >> col >> j;
        rpos.assign(k, 0);
        data.resize(j);

        for (i = 0; i < j; i++) {
            fin >> data[i].i >> data[i].j >> data[i].e;

            if (data[i].i < 0 || data[i].i >= rpos.size() || data[i].j < 0 || data[i].j >= col) {
                cout << "矩阵的行或列超出范围。" << endl;
                exit(0);
            }

            if (i > 0)
                if (data[i].i < data[i - 1].i || data[i].i == data[i - 1].i && data[i].j <= data[i - 1].j) {
                    cout << "元素没有按顺序输入。" << endl;
                    exit(0);
                }
        }

        fin.close();

        for (i = 0; i < data.size(); i++)
            for (j = data[i].i + 1; j < rpos.size(); j++)
                rpos[j]++;
    }
    void CopySMatrix(const RLSMatrix &M)
    {
        col = M.col;
        data = M.data;
        rpos = M.rpos;
    }
    void TransposeSMatrix(const RLSMatrix &M)
    {
        int i, j, k;
        vector<int> colm;
        col = M.rpos.size();

        if (M.data.size()) {
            data.resize(M.data.size());
            rpos.assign(M.col, 0);

            for (i = 0; i < data.size(); i++)
                for (j = M.data[i].j + 1; j < rpos.size(); j++)
                    rpos[j]++;

            colm = rpos;

            for (i = 0; i < data.size(); i++) {
                j = M.data[i].j;
                k = colm[j]++;
                data[k].i = j;
                data[k].j = M.data[i].i;
                data[k].e = M.data[i].e;
            }
        }
    }
    bool AddSMatrix(const RLSMatrix &M, const RLSMatrix &N)
    {
        int p, q, up, uq;

        if (M.rpos.size() != N.rpos.size() || M.col != N.col)
            return false;

        col = M.col;
        rpos.resize(M.rpos.size());

        for (int k = 0; k < rpos.size(); k++) {
            rpos[k] = data.size();
            p = M.rpos[k];
            q = N.rpos[k];

            if (k < rpos.size() - 1) {
                up = M.rpos[k + 1];
                uq = N.rpos[k + 1];
            } else {
                up = M.data.size();
                uq = N.data.size();
            }

            while (p < up && q < uq)
                if (M.data[p].j < N.data[q].j)
                    data.push_back(M.data[p++]);
                else if (M.data[p].j > N.data[q].j)
                    data.push_back(N.data[q++]);
                else {
                    if (M.data[p].e+N.data[q].e != 0) {
                        data.push_back(M.data[p]);
                        data[data.size() - 1].e += N.data[q].e;
                    }

                    p++, q++;
                }

            while (p < up)
                data.push_back(M.data[p++]);

            while (q < uq)
                data.push_back(N.data[q++]);
        }

        return true;
    }
    bool SubtSMatrix(const RLSMatrix &M, const RLSMatrix &N)
    {
        int i;
        RLSMatrix N1;
        N1.CopySMatrix(N);

        for (i = 0; i < N1.data.size(); i++)
            N1.data[i].e *= -1;

        return AddSMatrix(M, N1);
    }
    bool MultSMatrix(const RLSMatrix &M, const RLSMatrix &N)
    {
        int i, j, q, p, up, uq;
        Triple<T> t;
        RLSMatrix S;

        if (M.col != N.rpos.size())
            return false;

        rpos.resize(M.rpos.size());
        col = N.col;
        S.TransposeSMatrix(N);

        for (i = 0; i < rpos.size(); i++)
            for (j = 0; j < col; j++) {
                t.e = 0;
                p = M.rpos[i];
                q = S.rpos[j];

                if (i < M.rpos.size() - 1)
                    up = M.rpos[i + 1];
                else
                    up = M.data.size();

                if (j < S.rpos.size() - 1)
                    uq = S.rpos[j + 1];
                else
                    uq = S.data.size();

                while (p < up && q < uq)
                    if (M.data[p].j < S.data[q].j)
                        p++;
                    else if (M.data[p].j > S.data[q].j)
                        q++;
                    else
                        t.e += M.data[p++].e * S.data[q++].e;

                if (t.e != 0) {
                    t.i = i, t.j = j;
                    data.push_back(t);
                }
            }

        return true;
    }
    void PrintSMatrix()const
    {
        int k = 0;
        vector<Triple<T> >::const_iterator p = data.begin();

        for (int i = 0; i < rpos.size(); i++) {
            for (int j = 0; j < col; j++)
                if (k < data.size() && p->i == i && p->j == j) {
                    cout << setw(3) << (p++)->e;
                    k++;
                } else
                    cout << setw(3) << 0;

            cout << endl;
        }
    }
};
#endif

