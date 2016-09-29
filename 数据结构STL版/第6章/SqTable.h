//SqTable.h 静态查找表类（SqTable类）
#ifndef _SQTABLE_H_
#define _SQTABLE_H_
template<typename D>class SqTable
{
protected:
    vector<D> elem;
public:
    void Create_SeqFromFile(string FileName)
    {
        ifstream fin(FileName.c_str());
        D temp;

        while (!fin.eof()) {
            if (InputFromFile(fin, temp))
                elem.push_back(temp);
        }

        fin.close();
    }
    int Search_Seq(KeyType k, int j)const
    {
        int i;
        vector<D>::const_iterator cit;

        switch (j) {
        case 0:
            for (i = 0; i < elem.size(); i++)
                if EQ(k, elem[i].key)
                    return i;

            return -1;

        case 1:
            cit = find_if(elem.begin(), elem.end(), cmp);

            if (cit != elem.end())
                return cit - elem.begin();

            return -1;

        case 2:
            for (cit = elem.begin(); cit != elem.end(); cit++)
                if (cmp(*cit))
                    return cit - elem.begin();

            return -1;

        default:
            return -1;
        }
    }
    int Search_Bin(KeyType k)const
    {
        int mid, low = 0, high = elem.size() - 1;

        while (low <= high) {
            mid = (low + high) / 2;

            if EQ(k, elem[mid].key)
                return mid;
            else if LT(k, elem[mid].key)
                high = mid - 1;
            else
                low = mid + 1;
        }

        return -1;
    }
    int Search_Bin1(KeyType k)const
    {
        int mid, low = 0, high = elem.size();

        while (high - low > 1) {
            mid = (low + high) / 2;

            if LT(k, elem[mid].key)
                high = mid;
            else
                low = mid;
        }

        if EQ(k, elem[low].key)
            return low;
        else
            return -1;
    }
    bool GetElem(int i, D &e)const
    {
        if (i < 0 || i > elem.size() - 1)
            return false;

        e = elem[i];
        return true;
    }
    void Traverse(void(*visit)(D&))
    {
        for_each(elem.begin(), elem.end(), visit);
    }
    bool Input(ifstream &fin)
    {
        D temp;

        if (InputFromFile(fin, temp)) {
            elem.push_back(temp);
            return true;
        } else
            return false;
    }
    void OutputAllToFile(string FileName)const
    {
        ofstream fout(FileName.c_str());

        for (vector<D>::const_iterator itr = elem.begin(); itr != elem.end(); itr++)
            OutputToFile(fout, *itr);

        fout.close();
    }
};
#endif

