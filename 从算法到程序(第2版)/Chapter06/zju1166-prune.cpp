#include <iostream>
#include <memory.h>
#include <algorithm>
using namespace std;

string lexicon1[2000];  // 剪枝后的字典
int n1;                 // 剪枝后字典的条目数
string word[20];        // 短语中的单词
string phrase;          // 短语（原始的）
string oldphrase;       // 单词排序后的短语
int letters[26];        // 短语中字母出现的频数

//对字典进行剪枝优化，如果被测试字符串不包含字典的某个条目，则字典中该条目就可以被剪枝
bool inclusive(string &str, int *alpha)
{
    memset(alpha, 0, sizeof(alpha[0]) * 26);

    for (int j = 0; j < str.length(); j++) {
        int a = str[j] - 'A';

        //一个条目中字母的频数超过了短语中的字母频数，包含关系无效
        if (++alpha[a] > letters[a]) return false;
    }

    return true;
}

//形参index是字典中条目的序号，anagram是变位词字符串
void findAnagram(int index, string anagram)
{
    int i;

    for (i = 0; i < 26 && letters[i] == 0; i++);

    //字母已经用完了，找到了一组变位词
    if (i == 26) {
        //如果变位词字符串与原始短语（排序过）不同，就是一组有效的变位词
        if (anagram != phrase)
            cout << oldphrase << " = " << anagram << endl;

        return;
    }

    //字母还没有用完，继续回溯搜索
    //从字典的第index个条目起搜索
    int alpha[26];

    for (i = index; i < n1; i++)
        if (inclusive(lexicon1[i], alpha)) {
            // 存在包含关系，则回溯搜索
            int j;

            //将该条目的单词数减去
            for (j = 0; j < 26; j++)
                letters[j] -= alpha[j];

            //将该条目加入到变位词字符串中
            string anagram1 = anagram;

            if (anagram != "")
                anagram1 += ' ';

            anagram1 += lexicon1[i];
            findAnagram(i, anagram1); // 递归搜索

            // 恢复现场
            for (j = 0; j < 26; j++)
                letters[j] += alpha[j];
        }
}

void concatenate(int m, string &str)
{
    str = "";

    for (int i = 0; i < m; i++)
        str += word[i] + " ";

    str.erase(str.length() - 1, str.length());
}

int main()
{
    string lexicon[2000];
    int n = 0, m = 0;

    while (cin >> lexicon[n] && lexicon[n] != "#")
        n++;

    while (cin >> word[m] && word[m++] != "#")
        if (cin.get() == '\n') {
            int i;
            concatenate(m, oldphrase);
            sort(word, word + m);
            concatenate(m, phrase);
            memset(letters, 0, sizeof(letters));

            for (i = 0; i < phrase.length(); i++)
                if (phrase[i] != ' ')
                    letters[phrase[i] - 'A']++;

            n1 = 0;
            int alpha[26];

            for (i = 0; i < n; i++)
                if (inclusive(lexicon[i], alpha))
                    lexicon1[n1++] = lexicon[i];

            findAnagram(0, "");
            m = 0;
        }

    return 0;
}

