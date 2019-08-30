#include "Func.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <queue>
#include <time.h>
#include <functional>

extern int countPath;
extern int countbackup;
extern int flag;
extern int mark;

struct cmp
{
    bool operator () (LNode *a ,LNode *b)
    {
        return a->priority_dist_tmp >b->priority_dist_tmp ;
    }
};

struct cmp_back
{
    bool operator () (LNode *a ,LNode *b)
    {
        return  a->priority_dist_back_tmp >b->priority_dist_back_tmp;
    }
};

void DFS(ADJGraph &g, int s)
{
    EdgeNode *p = g.adjList[s].enext;
    g.visited[s] = true;
    while(p)
    {
        if(g.visited[p->elem.num] != true)
        {
            DFS(g,p->elem.num);
        }
        p = p->next;
    }
}

/**********************打印邻接表的所有信息*************************/
void PrintADJGraphInfo(ADJGraph &g)
{
    printf("There are %d vertex,%d edge.\n",g.vertAmount,g.edgeAmount);
    for(int i=0; i<g.vertAmount; i++)
    {
        //printf("Nodenumber:%d\t",g.adjList[i].num);
        //printf("Nodename:%d\t",g.adjList[i].nodeName);
        //printf("Len:%d\n",g.adjList[i].len);
        FILE *fp;
        if(flag == 0)
        {
            fp = fopen("BackupSPF.txt","a+");
        }
        else if(flag == 1)
        {
            fp = fopen("BackupISPF.txt","a+");
        }

        for(int j=0; j<g.vertAmount; j++)
        {
            if(i!=j)
            {
                fprintf(fp,"%d到%d的备份下一跳: ",g.adjList[i].nodeName,g.adjList[j].nodeName);
                for(int k=0; k<g.adjList[i].backupNextHop[j].size(); k++)
                {
                    fprintf(fp,"%d ",g.adjList[i].backupNextHop[j][k]);
                }

                fprintf(fp,"\t%d\n",g.adjList[i].backupNextHop[j].size());
            }
        }
        fprintf(fp,"\n\n");
        fclose(fp);
    }
}

/************************打印每个节点的邻居****************************/
void Traves(ADJGraph &g)
{
    for(int i=0; i<g.vertAmount; i++)
    {
        printf("Node %d Neighbor is: \n",g.adjList[i].nodeName);
        EdgeNode *p = g.adjList[i].enext;
        while(p != NULL)
        {
            printf("%d—%f\t",p->elem.num,p->elem.weight);
            p = p->next;
        }
        putchar('\n');
    }
}
vector<int >edge;
void LoadGraph(const char* filename, ADJGraph &g)
{
    int i = 0;              //for 循环的控制变量
    int vertAmount;
    int edgeAmount;
    LNode *vert;            // 指向邻接表第一个节点
    LNode *vertex;           // 用来指向邻接表中任意顶点
    int start;
    int finish;
    double weight;
    Elem elem;
    FILE *fp = fopen(filename,"r");
    fscanf(fp,"%d",&vertAmount);
    fscanf(fp,"%d",&edgeAmount);
    g.vertAmount = vertAmount;
    g.edgeAmount = edgeAmount;
    g.adjList = vert = (LNode* )malloc(sizeof(LNode)*vertAmount);
    if(!g.adjList)
    {
        puts("there is no space !");
        exit(-1);
    }

    /***************初始化LNode指针*****************/
    for(i=0; i<vertAmount; i++)
    {
        int s;
        fscanf(fp,"%d",&s);
        new (&vert[i])LNode(s,vertAmount);
        vert[i].num = i;
    }
    //每次加载图时清空edge数组,防止多次加入
    edge.resize(0);
    /****************读图各边转化为表*********************/
    while(!feof(fp))
    {
        fscanf(fp,"%d",&start);
        edge.push_back(start);
        fscanf(fp,"%d",&finish);
        edge.push_back(finish);
        fscanf(fp,"%lf",&weight);
        elem.weight = weight;
        elem.num = finish;
        vertex = vert + start;
        ListInsert(vertex,vertex->len+1,elem);
        elem.num = start;
        vertex = vert + finish;
        ListInsert(vertex,vertex->len+1,elem);
    }
    fclose(fp);
}

bool ListInsert(LNode *&L,int len,Elem e)
{
    if(len<1||len>L->len+1)
    {
        return false;
    }

    EdgeNode *p = L->enext;
    EdgeNode *temp = (EdgeNode *)malloc(sizeof(EdgeNode));
    int count = 1;
    if(!temp)
    {
        puts("malloc EdegeNode faliue!");
        exit(-1);
    }
    temp->elem = e;

    if(!p && count==1)
    {
        temp->next = p;
        L->enext = temp;
    }
    else
    {
        while(p!=NULL && count<len-1)
        {
            p = p->next;
            count++;
        }
        temp->next = p->next;
        p->next = temp;
    }
    L->len++;

    return true;
}

/*************************************************/
bool IsNeighbour(ADJGraph *g,int s,int num)
{
    EdgeNode *p = g->adjList[s].enext;
    while(p)
    {
        if(p->elem.num == num)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

void Dijkstra_Best(ADJGraph &g,int s)
{
    priority_queue <LNode *, vector<LNode *>, cmp > que;
    g.adjList[s].priority_dist[s] = 0;
    g.adjList[s].priority_dist_back[s] = 0;
    g.adjList[s].parent[s] = s;
    que.push(g.adjList+s);
    int i=0;
    for(i=0; i<g.vertAmount; i++)
        g.visited[i]=0;
    while(!que.empty())
    {
        LNode *mi = que.top();
        que.pop();
        if(g.visited[mi->num] == false)
        {
            g.visited[mi->num] = true;
            g.adjList[mi->num].priority_dist_tmp=g.adjList[s].priority_dist[mi->num];
            if(mi->num == s)
            {
                g.adjList[s].bestNextHop[mi->num] = mi->num;
            }
            else
            {
                if(IsNeighbour(&g,s,mi->num))
                {
                    g.adjList[s].bestNextHop[mi->num] = mi->num;//只能放一个数
                    if (mark == 1)//表示ISPF
                    {
                        g.adjList[s].backupNextHop[mi->num].push_back(mi->num);
                        countbackup++;
                    }
                    g.adjList[g.adjList[s].parent[s]].children[s].push_back(mi->num);//***
                }
                else
                {
                    g.adjList[s].children[g.adjList[s].parent[mi->num]].push_back(mi->num);
                    g.adjList[s].bestNextHop[mi->num]=g.adjList[s].bestNextHop[g.adjList[s].parent[mi->num]];
                    if(mark == 1)
                    {
                        g.adjList[s].backupNextHop[mi->num].push_back(g.adjList[s].bestNextHop[g.adjList[s].parent[mi->num]]);
                        countbackup++;
                    }
                }
            }
            EdgeNode *p = g.adjList[mi->num].enext;
            while(p)
            {

                if(g.visited[p->elem.num]==0)
                {
                    if((g.adjList[s].priority_dist[p->elem.num]) >
                            (g.adjList[s].priority_dist[mi->num]+p->elem.weight))
                    {
                        g.adjList[s].priority_dist[p->elem.num] =
                            g.adjList[s].priority_dist[mi->num]+p->elem.weight;
                        g.adjList[p->elem.num].priority_dist_tmp=g.adjList[s].priority_dist[p->elem.num];

                        g.adjList[s].priority_dist_back[p->elem.num] =
                            g.adjList[s].priority_dist_back[mi->num]+p->elem.weight;

                        g.adjList[s].parent[p->elem.num] = mi->num;
                        que.push(g.adjList+p->elem.num);
                    }
                }

                p = p->next;
            }
        }
    }

}
/***********************打印最优路径*****************************/
void AllPath_Best(ADJGraph &g,const char *filename)
{
    //为了每次从新覆盖文件
    FILE *fp = fopen(filename,"w");
    fclose(fp);
    for(int s=0; s<g.vertAmount; s++)
    {
        for(int d=0; d<g.vertAmount; d++)
        {
            Path_Best(g,filename,s,d);
        }
    }
}
void Path_Best(ADJGraph &g,const char *filename,int s,int d)
{
    FILE *fp = fopen(filename,"a+");
    int i = s,num=0;
    if(d!=s)
    {

        fprintf(fp,"%d",g.adjList[i].num);
        fputc(' ',fp);
        while(d != s)//打印一条BestPath路径
        {
            fprintf(fp,"%d",g.adjList[g.adjList[s].bestNextHop[d]].num);
            s = g.adjList[s].bestNextHop[d];
            if(d!=s)
                fputc(' ',fp);
        }
        fputc('\n',fp);

    }




    fclose(fp);
}

/***********************打印备份路径*****************************/
int source;
int m = 0;//表示ss数组的每条备份大小
int ss[100] = {0};//占存backup的下一跳
void AllPath_Backup(ADJGraph &g,const char *filename)
{
    for(int s=0; s<g.vertAmount; s++)
    {
        for(int d=0; d<g.vertAmount; d++)
        {
            if(d != s)
            {
                source = s;
                Path_Back(g,filename,s,d);
            }
            else
            {
                continue;
            }
        }
    }
}
void Path_Back(ADJGraph &g,const char *filename,int s,int d)
{
    int tmp;
    FILE *fp = fopen(filename,"a+");
    for(int i=0; i<g.adjList[s].backupNextHop[d].size(); i++)
    {
        tmp = g.adjList[s].backupNextHop[d].at(i);
        if(tmp == d)
        {
            fprintf(fp,"%d",source);
            fputc(' ',fp);
            for(int j=0; j<m; j++)
            {
                fprintf(fp,"%d",ss[j]);
                fputc(' ',fp);
            }
            fprintf(fp,"%d",g.adjList[d].num);
            fputc('\n',fp);
            countPath ++;
        }
        else
        {
            ss[m] = tmp;
            m++;
            Path_Back(g,filename,tmp,d);
            m--;
        }
    }

    fclose(fp);
    return ;
}
//SPF 的back路径方法,满足公式cost(s,d)<cost(x,d)，x表示s的邻居
void Dijkstra_Back_SPF(ADJGraph &g,int s)
{
    EdgeNode *p = g.adjList[s].enext;
    while(p)
    {
        for(int i=0; i<g.vertAmount; i++)
        {
            if(i!=s)
            {
                if(g.adjList[s].priority_dist[i] >
                        g.adjList[p->elem.num].priority_dist[i])
                {
                    g.adjList[s].backupNextHop[i].push_back(p->elem.num);
                    countbackup++;
                }
            }
        }
        p = p->next;
    }
}

void Dijkstra_Back_iSPF(ADJGraph &g,int s)
{
    priority_queue <LNode *, vector<LNode *>, cmp_back > que;
    EdgeNode *p = g.adjList[s].enext;
    for(int k=0; k<g.adjList[s].len; k++)
    {

        for(int i=0; i<g.vertAmount; i++)
        {
            g.visited[i] = false;
            g.adjList[i].priority_dist_back_tmp = 100000000;
        }

        double tmpWeight = p->elem.weight;
        Sub_Weight_Subtree(g,s,p->elem.num,tmpWeight);
        for(int i=0; i<g.adjList[s].subtree[p->elem.num].size(); i++)
        {
            int subtreeNodeNum = g.adjList[s].subtree[p->elem.num].at(i);
            EdgeNode *subNei = g.adjList[subtreeNodeNum].enext;
            //利用subnei遍历subtreeNodeNum 所有邻居
            for(int j=0; j<g.adjList[subtreeNodeNum].len; j++)
            {
                if(g.visited[subNei->elem.num]==false)
                {
                    if(g.adjList[s].priority_dist_back[subNei->elem.num] >
                            g.adjList[s].priority_dist_back[subtreeNodeNum] + subNei->elem.weight)
                    {

                        g.adjList[s].priority_dist_back[subNei->elem.num] =
                            g.adjList[s].priority_dist_back[subtreeNodeNum] + subNei->elem.weight;

                        g.adjList[subNei->elem.num].priority_dist_back_tmp =
                            g.adjList[s].priority_dist_back[subNei->elem.num];
                        que.push(g.adjList+subNei->elem.num);
                    }
                }
                subNei = subNei->next;
            }
        }
        while(!que.empty())
        {

            LNode *mi = que.top();
            que.pop();
            if(g.visited[mi->num] == false)
            {
                EdgeNode *q = g.adjList[mi->num].enext;
                //找出队列的下一跳，备份
                g.adjList[s].backupNextHop[mi->num].push_back(p->elem.num);
                countbackup++;
                g.visited[mi->num]=true;
                while(q)
                {
                    if(g.visited[q->elem.num]==0)
                    {
                        if((g.adjList[s].priority_dist_back[q->elem.num]) >
                                (g.adjList[s].priority_dist_back[mi->num]+q->elem.weight))
                        {
                            g.adjList[s].priority_dist_back[q->elem.num] =
                                g.adjList[s].priority_dist_back[mi->num]+q->elem.weight;
                            g.adjList[q->elem.num].priority_dist_back_tmp =g.adjList[s].priority_dist_back[q->elem.num];
                            //入队列他的父亲就改变了
                            g.adjList[s].parent[q->elem.num]=mi->num;
                            que.push(g.adjList+q->elem.num);
                        }
                    }
                    q = q->next;
                }
            }

        }
        //把边加回去,子树复原
        for(int i=0; i<g.vertAmount; i++)
        {
            g.adjList[s].priority_dist_back[i] = g.adjList[s].priority_dist[i];
        }
        p = p->next;
    }

}

void Sub_Weight_Subtree(ADJGraph &g,int s,int neighbour,double tmpWeight)
{
    for(int i=0; i<g.adjList[s].subtree[neighbour].size(); i++)
    {
        g.adjList[s].priority_dist_back[g.adjList[s].subtree[neighbour].at(i)] =
            g.adjList[s].priority_dist[g.adjList[s].subtree[neighbour].at(i)] - tmpWeight;

        g.visited[g.adjList[s].subtree[neighbour].at(i)] = true;
    }
}

void GetChild(ADJGraph &g)
{
    for(int i=0; i<g.vertAmount; i++)
    {
        for(int j=0; j<g.vertAmount; j++)
        {
            if(g.adjList[i].parent[j] != i)
            {
                g.adjList[g.adjList[i].parent[j]].children[i].push_back(j);
            }
        }
    }
}
//遍历s为根的子树
void SubTree(ADJGraph &g,int s,int child)
{
    queue <int> que;
    que.push(child);
    while(!que.empty())
    {
        g.adjList[s].subtree[child].push_back(que.front());
        int tmp = que.front();
        que.pop();
        //遍历每个节点的孩子
        for(int i=0; i<g.adjList[s].children[tmp].size(); i++)
        {
            if(!g.adjList[s].children[tmp].empty())
            {
                int k;
                k = g.adjList[s].children[tmp].at(i);
                que.push(k);
            }
        }
    }
}

/*************************************************/
void SavePower(ADJGraph g,const char *infilename,const char *outfilename)
{
    int i=0,num=0,edgecount=0;
    char ch;
    for(i=0; i<edge.size(); i+=2)
    {
        ChoiceEdge(edge.at(i),edge.at(i+1),infilename,outfilename);
        if(Indirect(g,outfilename))
        {
            cout<<"可关的边\t"<<edge.at(i)<<"----"<<edge.at(i+1)<<endl;;
            edgecount++;
            FILE *in=fopen(infilename,"w");
            if(!in)perror(0);
            FILE *out=fopen(outfilename,"r");
            if(!out)perror(0);
            while(!feof(out))
            {
                if(fscanf(out,"%d",&num)<0) continue;//表示下面的不执行
                ch=fgetc(out);
                fprintf(in,"%d",num);
                fputc(ch,in);
            }
            fclose(in);
            fclose(out);
        }
    }
    cout<<"边数\t"<< edgecount<<endl;;
}
void  ChoiceEdge(int start,int finish,const char *infilename,const char *outfilename)
{
    int i=0,j=0,tmp1,tmp2,tmp3,flag=1;
    char ch;
    vector<int >a;
    FILE *infp=fopen(infilename,"r");
    if(!infp)perror(0);
    FILE *outfp=fopen(outfilename,"w");
    if(!outfp)perror(0);
    while(!feof(infp))
    {
        //读入一行到a容器里
        if(fscanf(infp,"%d",&tmp1)<0)continue;
        fscanf(infp,"%d",&tmp2);
        ch=fgetc(infp);
        a.push_back(tmp1);
        a.push_back(tmp2);
        while(ch!='\n'&&!feof(infp))
        {
            fscanf(infp,"%d",&tmp1);
            ch=fgetc(infp);
            a.push_back(tmp1);
        }
        flag=0;
        for(j=0; j<a.size()-1; j++)
        {
            if((start==a.at(j)&&finish==a.at(j+1))||(finish==a.at(j)&&start==a.at(j+1)))
            {
                flag=1;
                break;
            }
        }
        if(flag!=1)
        {
            for(j=0; j<a.size(); j++)
            {
                tmp3=a.at(j);
                fprintf(outfp,"%d",tmp3);
                //行末尾不能打印空格
                if(j!=a.size()-1)
                    fputc(' ',outfp);
            }
            fputc('\n',outfp);
        }
        a.clear();
    }
    fclose(infp);
    fclose(outfp);
}
bool Indirect(ADJGraph g,const char *filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp)perror(0);
    int i=0,j=0,tmp,sum=0,flag=0,e[2],flag1=0;
    char ch;
    bool mark[g.vertAmount];
    fscanf(fp,"%d",&e[0]);
    ch=fgetc(fp);
    while(!feof(fp))
    {
        mark[e[0]]=true;
        for(i=e[0]+1; i<g.vertAmount; i++)
            mark[i]=false;
        if(flag1!=0)
        {
            for(i=0; i<=e[0]; i++)
                mark[i]=true;
            flag1=1;
        }
        tmp=e[0];
        while(tmp==e[0]&&!feof(fp))
        {
            //读一行
            while(ch!='\n'&&!feof(fp))
            {
                fscanf(fp,"%d",&e[1]);
                ch=fgetc(fp);
            }
            mark[e[1]]=true;
            if( fscanf(fp,"%d",&e[0])<0)break;
            ch=fgetc(fp);
        }
        flag=0;
        for(i=0; i<g.vertAmount; i++)
            if(!mark[i])
            {
                flag=1;
                break;
            }
        if(flag==1)
        {
            return false;
        }
    }
    fclose(fp);
    return true;
}
void Jianshu(const char *filename)
{
    int i=0,j=0,tmp1,tmp2,tmp3,flag=1,k=0;
    char ch;
    vector<int >a;
    flag=0;
    int *sum=(int *)calloc(sizeof(int ),edge.size()/2);
    FILE *infp=fopen(filename,"r");
    while(!feof(infp))
    {
        //读入一行到a容器里
        fscanf(infp,"%d",&tmp1);
        fscanf(infp,"%d",&tmp2);
        ch=fgetc(infp);
        a.push_back(tmp1);
        a.push_back(tmp2);
        while(ch!='\n'&&!feof(infp))
        {
            fscanf(infp,"%d",&tmp1);
            ch=fgetc(infp);
            a.push_back(tmp1);
        }
        for(j=0; j<a.size(); j++)
        {
            cout<<a.at(j)<<"\t";
        }
        cout<<endl;
        for(k=0,j=0; j<a.size()-1; j++)
        {
            for(k=0,i=0; i<edge.size(); i+=2)
            {
                if((edge.at(i)==a.at(j)&&edge.at(i+1)==a.at(j+1))||(edge.at(i+1)==a.at(j)&&edge.at(i)==a.at(j+1)))
                {
                    sum[k]++;
                    break;
                }
                k++;
            }

        }
        a.clear();
    }
    for(j=0,k=0; j<edge.size()/2; j++,k+=2)
    {
        cout<<edge.at(k)<<"--"<<edge.at(k+1)<<"边介数："<<sum[j]<<endl;
    }
    fclose(infp);
}

double ProtectionRatio(ADJGraph g)
{
    int protect = 0;
    for(int i=0; i<g.vertAmount; i++)
    {
        for(int j=0; j<g.vertAmount; j++)
        {
            if(i!=j)
            {
                //此处backup有betspath
                if(g.adjList[i].backupNextHop[j].size()>=2)
                {
                    protect++;
                }
            }
        }
    }
    return (double) protect/((g.vertAmount-1)*g.vertAmount);
}

void SPF_BackupPath(ADJGraph &g)
{
    mark = 0;
    for(int s=0; s<g.vertAmount; s++)
    {
        Dijkstra_Best(g,s);
    }
    for(int s=0; s<g.vertAmount; s++)
    {
        Dijkstra_Back_SPF(g,s);
    }

}
void ISPF_BackupPath(ADJGraph &g)
{
    mark = 1;
    for(int s=0; s<g.vertAmount; s++)
    {
        Dijkstra_Best(g,s);
    }
    for(int s=0; s<g.vertAmount; s++)
    {
        for(int c=0; c<g.vertAmount; c++)
        {
            SubTree(g,s,c);
        }
    }
    for(int s=0; s<g.vertAmount; s++)
    {
        Dijkstra_Back_iSPF(g,s);
    }
}

void SPF_TotalRunTime(ADJGraph &g)
{
    double time = 0;
    clock_t start,finish;
    mark = 0;
    for(int s=0; s<g.vertAmount; s++)
    {
        start = clock();
        Dijkstra_Best(g,s);
        for(int c=0; c<g.vertAmount; c++)
        {
            SubTree(g,s,c);
        }
        EdgeNode *p = g.adjList[s].enext;
        for(int i=0; i<g.adjList[s].len; i++)
        {
            Dijkstra_Best(g,p->elem.num);
            for(int c=0; c<g.vertAmount; c++)
            {
                SubTree(g,s,c);
            }
            p = p->next;
        }
        Dijkstra_Back_SPF(g,s);
        finish = clock();
        time += (double)(finish-start);

    }
}
void ISPF_TotalRunTime(ADJGraph &g)
{
    double time = 0;
    clock_t start,finish;
    mark = 1;
    for(int s=0; s<g.vertAmount; s++)
    {
        start = clock();
        Dijkstra_Best(g,s);
        for(int c=0; c<g.vertAmount; c++)
        {
            SubTree(g,s,c);
        }
        Dijkstra_Back_iSPF(g,s);
        finish = clock();
        time += (double)(finish-start);
    }
}
//void Temina()
//{
//    ADJGraph g2;
//    LoadGraph("11.txt",g2);
//    int originCommonEdge =findCommonEdge(g2,g2);
//    int T=100;
//    Edgeinfo* compedge=NULL;
//    while(findCommonEdge(g2,g2)>0||T>0)
//    {
//        compedg=MinCommonEdge();
//        if(originCommonEdge > compedge->commonedge||T>rand()%100)
//        {
//            //文件操作
//            for(int s=0; s<g2.vertAmount; s++)
//            {
//                Dijkstra_Best(g2,s);
//            }
//            AllPath_Best(g2,"1.txt");
//        }
//        T--;
//    }
//}
//
//Edgeinfo* MinCommonEdge(ADJGraph &g2, )
//{
//
//
//    for(int i = 0; i < edge.size(); i++)
//    {
//        LoadGraph(file, g2);
//        ReNewBest(g2,edge.at(i));
//        edge.at(i).commonedge = findCommonEdge(g,g2);
//    }
//    sort(0,edge,size(),less<Edgeinfo>);
//    return &edge.at(0);
//}
//
//void ChangeWeight(ADJGraph &g, EdgeInfo *e)
//{
//    e->weight += 2*(degree(e->start)+degree(e->finish));
//
//    EdgeNode *p = g.adjList[e->start].enext;
//    while(p->elem.num != e->finish)
//    {
//        p = p->next;
//    }
//    p->elem.weight = e->weight;
//
//    p = g.adjList[e->finish].enext;
//    while(p->elem.num != e->start)
//    {
//        p = p->next;
//    }
//    p->elem.weight = e->weight;
//}
