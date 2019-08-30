#include <iostream>
#include<stdio.h>
#include "LNode.h"
#include "Func.h"
#include <time.h>
#include <algorithm>
#include <string.h>
#define CLOCKS_PER_SEC ((clock_t)1000)
using namespace std;

int countPath = 0;
int countbackup = 0;
int mark;
int flag;
int main()
{
    int select;
    char buf[80];
    while(1)
    {
        cout<<" 1: 请输入要加载的文件"<<endl;
        cout<<" 2: 打印最短路径 "<<endl;
        cout<<" 3: 打印备份下一跳 "<<endl;
        cout<<" 4: 比较两方案时间 "<<endl;
        cout<<" 5: 计算故障保护率 "<<endl;
        cout<<" 6: 备份下一跳总数量 "<<endl;
        cout<<" 7: 关闭链路节能 "<<endl;
        cout<<"please input :"<<endl;
        scanf("%d",&select);
        getchar();
        switch(select)
        {
            case 1:
            {
                memset(buf,0,80);
                cout<<"  请输入文件名:"<<endl;
                gets(buf);
                cout<<"  文件"<<buf<<"读取完毕！\n"<<endl;
            }
            break;
            case 2:
            {
                ADJGraph g;
                LoadGraph(buf,g);
                for(int s=0; s<g.vertAmount; s++)
                {
                    Dijkstra_Best(g,s);
                }
                AllPath_Best(g,"BestPath.txt");
                cout<<"  最优路径已加载到 BestPath.txt 中"<<endl;

            }
            break;
            case 3:
            {
                flag = 1;
                ADJGraph g;
                LoadGraph(buf,g);
                SPF_BackupPath(g);
                PrintADJGraphInfo(g);
                //AllPath_Backup(g,"PathSPF.txt");
                cout<<"  基于SPF方案的备份下一跳已加载到 PathSPF.txt 中"<<endl;
                flag = 0;
                ADJGraph g2;
                LoadGraph(buf,g2);
                ISPF_BackupPath(g2);
                PrintADJGraphInfo(g2);
                //AllPath_Backup(g,"PathISPF.txt");
                cout<<"  基于ISPF方案的备份下一跳已加载到 PathISPF.txt 中"<<endl;
            }
            break;
            case 4:
            {
                clock_t start,finish;
                ADJGraph g;
                start = clock();
                LoadGraph(buf,g);
                SPF_TotalRunTime(g);
                finish = clock();
                cout <<g.vertAmount << "节点基于SPF保护方案的运行时长："<< (double)(finish-start)<< "ms" << endl;
                ADJGraph g2;
                start = clock();
                LoadGraph(buf,g2);
                ISPF_TotalRunTime(g2);
                finish = clock();
                cout <<g.vertAmount << "节点基于ISPF保护方案的运行时长："<< (double)(finish-start)<< "ms" << endl;
            }
            break;
            case 5:
            {
                ADJGraph g;
                LoadGraph(buf,g);
                mark = 0;
                for(int s=0; s<g.vertAmount; s++)
                {
                    Dijkstra_Best(g,s);
                }
                for(int s=0;s<g.vertAmount;s++)
                {
                    Dijkstra_Back_SPF(g,s);
                }
                cout <<"故障保护率:"<< ProtectionRatio(g) << endl;
            }
            break;
            case 6:
            {
                countbackup = 0;
                ADJGraph g;
                LoadGraph(buf,g);
                SPF_BackupPath(g);
                cout <<"SPF保护方案的备份下一跳总数:"<< countbackup <<endl;
                countbackup = 0;
                ADJGraph g2;
                LoadGraph(buf,g2);
                ISPF_BackupPath(g2);
                cout <<"ISPF保护方案的备份下一跳总数:"<< countbackup <<endl;
            }
            break;
            case 7:
            {
                ADJGraph g;
                if(g.adjList!=NULL)
                    g.adjList=0;
                LoadGraph(buf,g);
                SPF_BackupPath(g);
                AllPath_Backup(g,"PathSPF.txt");
                ADJGraph g2;
                LoadGraph(buf,g2);
                ISPF_BackupPath(g2);
                PrintADJGraphInfo(g2);
                //AllPath_Backup(g,"PathISPF.txt");
                SavePower(g,"PathSPF.txt","outspf.txt");
            }
            break;
        }
        cout<<"是否继续: 0 退出\t 1 继续"<<endl;
        scanf("%d",&select);
        getchar();
        if(select==0)
        {
            cout<<"main end ，内存已释放"<<endl;
            break;
        }else
        {
            system("cls");
        }
    }
    return 0;
}
