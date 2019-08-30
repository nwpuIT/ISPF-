#include "LNode.h"
#include <string.h>
#include "Func.h"
#include <vector>
#include<stdlib.h>
LNode::LNode(int n,int vertAmount)
{
    nodeName=n;
    num = -1;
    len = 0;
    enext = NULL;
    priority_dist_tmp=100000000;
    priority_dist_back_tmp=1000000;
    for(int i=0; i<vertAmount; i++)
    {
        priority_dist.push_back(1000000000);
        priority_dist_back.push_back(1000000000);
        parent.push_back(-1);
        bestNextHop.push_back(-1);
    }
}

LNode::~LNode()
{
    if(enext)
        free(enext);
    enext=NULL;
}
