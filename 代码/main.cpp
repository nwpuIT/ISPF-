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
        cout<<" 1: ������Ҫ���ص��ļ�"<<endl;
        cout<<" 2: ��ӡ���·�� "<<endl;
        cout<<" 3: ��ӡ������һ�� "<<endl;
        cout<<" 4: �Ƚ�������ʱ�� "<<endl;
        cout<<" 5: ������ϱ����� "<<endl;
        cout<<" 6: ������һ�������� "<<endl;
        cout<<" 7: �ر���·���� "<<endl;
        cout<<"please input :"<<endl;
        scanf("%d",&select);
        getchar();
        switch(select)
        {
            case 1:
            {
                memset(buf,0,80);
                cout<<"  �������ļ���:"<<endl;
                gets(buf);
                cout<<"  �ļ�"<<buf<<"��ȡ��ϣ�\n"<<endl;
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
                cout<<"  ����·���Ѽ��ص� BestPath.txt ��"<<endl;

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
                cout<<"  ����SPF�����ı�����һ���Ѽ��ص� PathSPF.txt ��"<<endl;
                flag = 0;
                ADJGraph g2;
                LoadGraph(buf,g2);
                ISPF_BackupPath(g2);
                PrintADJGraphInfo(g2);
                //AllPath_Backup(g,"PathISPF.txt");
                cout<<"  ����ISPF�����ı�����һ���Ѽ��ص� PathISPF.txt ��"<<endl;
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
                cout <<g.vertAmount << "�ڵ����SPF��������������ʱ����"<< (double)(finish-start)<< "ms" << endl;
                ADJGraph g2;
                start = clock();
                LoadGraph(buf,g2);
                ISPF_TotalRunTime(g2);
                finish = clock();
                cout <<g.vertAmount << "�ڵ����ISPF��������������ʱ����"<< (double)(finish-start)<< "ms" << endl;
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
                cout <<"���ϱ�����:"<< ProtectionRatio(g) << endl;
            }
            break;
            case 6:
            {
                countbackup = 0;
                ADJGraph g;
                LoadGraph(buf,g);
                SPF_BackupPath(g);
                cout <<"SPF���������ı�����һ������:"<< countbackup <<endl;
                countbackup = 0;
                ADJGraph g2;
                LoadGraph(buf,g2);
                ISPF_BackupPath(g2);
                cout <<"ISPF���������ı�����һ������:"<< countbackup <<endl;
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
        cout<<"�Ƿ����: 0 �˳�\t 1 ����"<<endl;
        scanf("%d",&select);
        getchar();
        if(select==0)
        {
            cout<<"main end ���ڴ����ͷ�"<<endl;
            break;
        }else
        {
            system("cls");
        }
    }
    return 0;
}
