#ifndef LNODE_H_INCLUDED
#define LNODE_H_INCLUDED

#include <vector>

#define VERT_AMOUNT 500
#define CHILD_AMOUNT 10
#define BACKUP_NEXT_HOP 10

using namespace std;

struct EdgeNode;

class LNode
{
public:
    LNode(int n,int vertAmount);
    ~LNode();
    int  nodeName;
    int num;                        //节点的编号
    int len;                        //节点的邻居数目
    double priority_dist_tmp,priority_dist_back_tmp;
    EdgeNode *enext;                //节点的第一个邻居
    vector<double> priority_dist;   //此节点到每个节点的最短距离
    vector<double> priority_dist_back;
    vector<int> parent;             //此节点的最短路径树中i节点的父亲节点
    vector<int> bestNextHop;
    vector<int> children[VERT_AMOUNT];
    vector<int> backupNextHop[VERT_AMOUNT];
    vector <int> subtree[VERT_AMOUNT];

};

typedef struct Elem
{
    int num;                    //LNode 节点的邻居的编号
    double weight;              //LNode 节点与此邻居边权
}Elem;
typedef struct EdgeNode
{
    Elem  elem;                 //LNode 的邻居信息
    struct EdgeNode *next;      //LNode 的下一邻居
}EdgeNode;

#endif // LNODE_H_INCLUDED
