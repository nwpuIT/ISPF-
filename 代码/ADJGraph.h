#ifndef ADJGRAPH_H_INCLUDED
#define ADJGRAPH_H_INCLUDED

#include "LNode.h"

/*********�ڽ�����ṹ***********/
class ADJGraph
{
public:
    ADJGraph();
    ~ADJGraph();
    LNode *adjList;
    bool visited[VERT_AMOUNT];
    int vertAmount;     //�ڵ���
    int edgeAmount;     //����Ŀ
};

#endif // ADJGRAPH_H_INCLUDED

