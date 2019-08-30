#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED
#include "ADJGraph.h"

void printDoubleVector(double& i);
void printIntVector(int& i);

void DFS(ADJGraph &g, int s);

/**********************************/
void PrintADJGraphInfo(ADJGraph &g);
void Traves(ADJGraph &g);

/**************ÔØÈëÍ¼**************/
void LoadGraph(const char* filename, ADJGraph &g);
bool ListInsert(LNode *&L,int len,Elem e);

/*********************************/
bool IsNeighbour(ADJGraph *g,int s,int num);
void Dijkstra_Best(ADJGraph &g,int s);

/************************************************************/
void Dijkstra_Back_SPF(ADJGraph &g,int s);  //某边权值改为0后,以S为根,构造最短路径树
void Dijkstra_Back_iSPF(ADJGraph &g,int s);

/************************************************************/
void AllPath_Best(ADJGraph &g,const char *filename);
void AllPath_Backup(ADJGraph &g,const char *filename);
void Path_Best(ADJGraph &g,const char *filename,int s,int d);
void Path_Back(ADJGraph &g,const char *filename,int s,int d);
void AllPath_Backup_ISPF(ADJGraph &g,const char *filename);
void Path_Back_ISPF(ADJGraph &g,const char *filename,int s,int d);
void GetChild(ADJGraph &g);
void SubTree(ADJGraph &g,int s,int child);


void Sub_Weight_Subtree(ADJGraph &g,int s,int neighbour,double tmpWeight);
void Add_Weight_Subtree(ADJGraph &g,int s,int neighbour,int tmpWeight);

void ChoiceEdge(int start,int finish,const char *infilename,const char *outfilename);
void SavePower(ADJGraph g,const char *infilename,const char *outfilename);
bool Indirect(ADJGraph g,const char *filename);


void PrintPriority_Dist_Back(ADJGraph &g,int s);
double ProtectionRatio(ADJGraph g);

/********************************/
void SPF_BackupPath(ADJGraph &g);
void ISPF_BackupPath(ADJGraph &g);

void SPF_TotalRunTime(ADJGraph &g);
void ISPF_TotalRunTime(ADJGraph &g);
#endif // FUNC_H_INCLUDED
