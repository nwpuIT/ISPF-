#include <iostream>
#include "ADJGraph.h"
#include<stdlib.h>
ADJGraph::ADJGraph()
{
    this->adjList = NULL;
    this->vertAmount = 0;
    this->edgeAmount = 0;
}
ADJGraph:: ~ADJGraph()
 {
     if(adjList)
        free(adjList);
     adjList=NULL;
 }
