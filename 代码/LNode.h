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
    int num;                        //�ڵ�ı��
    int len;                        //�ڵ���ھ���Ŀ
    double priority_dist_tmp,priority_dist_back_tmp;
    EdgeNode *enext;                //�ڵ�ĵ�һ���ھ�
    vector<double> priority_dist;   //�˽ڵ㵽ÿ���ڵ����̾���
    vector<double> priority_dist_back;
    vector<int> parent;             //�˽ڵ�����·������i�ڵ�ĸ��׽ڵ�
    vector<int> bestNextHop;
    vector<int> children[VERT_AMOUNT];
    vector<int> backupNextHop[VERT_AMOUNT];
    vector <int> subtree[VERT_AMOUNT];

};

typedef struct Elem
{
    int num;                    //LNode �ڵ���ھӵı��
    double weight;              //LNode �ڵ�����ھӱ�Ȩ
}Elem;
typedef struct EdgeNode
{
    Elem  elem;                 //LNode ���ھ���Ϣ
    struct EdgeNode *next;      //LNode ����һ�ھ�
}EdgeNode;

#endif // LNODE_H_INCLUDED
