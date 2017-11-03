#include "trie.h"
#include <QDebug>
#include <QString>
trie::trie()
{
}

void trie::insert(Trie *root, std::string s)
{
    //std::cout <<"inserting :"<<s<<std::endl;
    int k = 0;
    if(root == NULL || s[k] == '\0')
        return;
    int i;
    Trie *p = root;
    while(s[k] != '\0')
    {
        if(p->next[s[k]-'a']==NULL)        //如果不存在，则建立结点
        {
            Trie *temp = (Trie *)std::malloc(sizeof(Trie));
            for(i = 0; i < MAX; i++)
            {
                temp->next[i] = NULL;
            }
            temp->isStr = true;
            p->next[s[k]-'a'] = temp;
            p = p->next[s[k]-'a'];
        }
        else
        {
            p = p->next[s[k]-'a'];
        }
        k++;
    }
    p->isStr = true;                       //单词结束的地方标记此处可以构成一个单词
}

int trie::search(Trie *root, std::string s)  //查找某个单词是否已经存在
{
    int k = 0;
    Trie *p = root;
    while(p != NULL && s[k] != '\0')
    {
        p = p->next[s[k] - 'a'];
        k++;
    }
    return (p != NULL && p->isStr == true);      //在单词结束处的标记为true时，单词才存在
}

void trie::del(Trie *root)                      //释放整个字典树占的堆区空间
{
    int i;
    for(i = 0; i < MAX; i++)
    {
        if(root->next[i] != NULL)
        {
            trie::del(root->next[i]);
        }
    }
    std::free(root);
}

int trie::initialTrie(void)
{
    int i;
    std::string s;
    std::ifstream icin("D:\\workspace\\Qt\\fk\\pinyin.txt");
    root = (Trie *)std::malloc(sizeof(Trie));
    if(!root)
        return 0;
    for(i = 0; i < MAX; i++)
    {
        root->next[i] = NULL;
    }
    root->isStr = false;
    while(icin)
    {
        std::getline(icin, s);
        insert(root, s);
    }

    return 1;
}
