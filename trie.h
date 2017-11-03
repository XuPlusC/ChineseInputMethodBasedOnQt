#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#define MAX 26

typedef struct TrieNode                    //Trie结点声明
{
    bool isStr;                            //标记该结点处是否构成单词
    struct TrieNode *next[26];            //儿子分支
}Trie;

class trie
{
public:
    trie();
    Trie *root;
    void insert(Trie *root, std::string s);  //将单词s插入到字典树中
    int search(Trie *root, std::string s);   //查找某个单词是否已经存在
    void del(Trie *root);               //释放整个字典树占的堆区空间
    int initialTrie(void);
};

#endif // TRIE_H
