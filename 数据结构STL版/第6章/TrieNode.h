//TrieNode.h Trie树结点的存储结构
#ifndef _TrieNODE_H_
#define _TrieNODE_H_
struct TrieNode {
    int index;
    vector<TrieNode*> ptr;
};
#endif

