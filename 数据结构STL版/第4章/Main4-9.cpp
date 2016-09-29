//Main4-9.cpp 验证赫夫曼树HuffmanTree<T>类的成员函数
#include "C.h"
#include "HuffmanTree.h"
void main()
{
    HuffmanTree<int> h;
    h.CreateHT("F4-10.txt");
    h.HuffmanCodingLeaf();
    h.HuffmanCodingRoot();
}

