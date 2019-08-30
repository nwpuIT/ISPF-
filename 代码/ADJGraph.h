#ifndef ADJGRAPH_H_INCLUDED
#define ADJGRAPH_H_INCLUDED

#include "LNode.h"

/*********邻接链表结构***********/
class ADJGraph
{
public:
    ADJGraph();
    ~ADJGraph();
    LNode *adjList;
    bool visited[VERT_AMOUNT];
    int vertAmount;     //节点数
    int edgeAmount;     //边数目
};

#endif // ADJGRAPH_H_INCLUDED

