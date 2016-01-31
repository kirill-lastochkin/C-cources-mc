#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fmng.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

DIR *dp[2];     //2 ������ ���������� ��� 2 �������
struct dirent *current;
char *curname[2]; //������� ���������� ��� 2 �������

//������������� ������ � ������� ����������
//���� - ���� � ����� ������
//����� - ��������� �� �����
DIR* StartDir(char *path, int mngnum)
{
    int i;
    dp[mngnum]=opendir(path);
    if(!dp[mngnum])
    {
        perror("dir not opned");
        return NULL;
    }
    //��������� � ����� �����
    i=chdir(path);
    if(i==-1)
    {
        perror("dir not changed");
        closedir(dp[mngnum]);
        return NULL;
    }
    //������� ������
    ClearPanel(mngnum);
    i=1;
    //��������� ������ ������� ��� ����������
    while((current=readdir(dp[mngnum]))!=NULL)
    {
        if(current->d_type==4)
        {
            PrintDir(mngnum,i,'/',current->d_name);
        }
        else
        {
            PrintDir(mngnum,i,'*',current->d_name);
        }
        i++;
    }
    //��������� ��� �������� ��������
    curname[mngnum]=(char*)calloc(70,sizeof(char));
    if(getcwd(curname[mngnum],69)==NULL)
    {
        perror("error with getcwd\n");
        free(curname[mngnum]);
        return NULL;
    }
    return dp[mngnum];
}

//����� ��������
//���� - ����� ������, �������, �� ������� ��������� ����� ���
int ChangeDir(int mngnum,int pos)
{
    //��������� � ������� ���� ����� ������
    chdir(curname[mngnum]);
    //������� �������
    rewinddir(dp[mngnum]);
    int i;
    for(i=1;i<=pos;i++)
    {
        current=readdir(dp[mngnum]);
    }
    //��������� ����� � ������������� ��� ����� ������� �����
    closedir(dp[mngnum]);
    dp[mngnum]=opendir(current->d_name);
    if(!dp[mngnum])
    {
        perror("error of changing dir\n");
        //��� �������(�������� �������� ������� � ����) ������ ������ �������������� �����
        //� ������� ������, �.�. ������� �� ��������
        free(curname[mngnum]);
        dp[mngnum]=StartDir(".",mngnum);
        return pos;
    }
    //���� �� ���� ������, ������
    chdir(current->d_name);
    //��������� ����� ������� ����
    if(getcwd(curname[mngnum],69)==NULL)
    {
        perror("error with getcwd\n");
        return 0;
    }
    //��������� ������ ������
    rewinddir(dp[mngnum]);
    ClearPanel(mngnum);
    i=1;
    while((current=readdir(dp[mngnum]))!=NULL)
    {
        if(current->d_type==4)
        {
            PrintDir(mngnum,i,'/',current->d_name);
        }
        else
        {
            PrintDir(mngnum,i,'*',current->d_name);
        }
        i++;
    }
    SelectDir(mngnum,1,1);
    return 1;
}

//������ ���������� � �����
int PrintInfo(int mngnum,int pos)
{
    chdir(curname[mngnum]);
    rewinddir(dp[mngnum]);
    int i;
    long int data;
    struct stat *st;
    //������� ������ �������
    for(i=1;i<=pos;i++)
    {
        current=readdir(dp[mngnum]);
    }
    st=(struct stat*)malloc(sizeof(struct stat));
    if(st==NULL)
    {
        perror("memory error\n");
        exit(1);
    }
    i=stat(current->d_name,st);
    if(i==-1)
    {
        perror("stat error\n");
        free(st);
        return 0;
    }
    //�� ������� �������� ��� � ����.����
    data=(long int)current->d_ino;
    ClearPanel(2);
    PrintInfo_g(current->d_name,0,1,1);
    PrintInfo_g(NULL,data,3,1);
    data=(long int)st->st_size;
    PrintInfo_g(NULL,data,2,1);
    //
    if(S_ISREG(st->st_mode))
        PrintInfo_g("Regular file",0,1,2);
    if(S_ISDIR(st->st_mode))
        PrintInfo_g("Directory",0,1,2);
    if(S_ISLNK(st->st_mode))
        PrintInfo_g("Link file",0,1,2);
    //
    data=(long int)current->d_type;
    PrintInfo_g(NULL,data,2,2);
    free(st);
    //S_IFMT
    return 1;

}

//���������� ������ � ������������
void FinishDir(int mngnum)
{
    closedir(dp[mngnum]);
    free(curname[mngnum]);
}

//���������� ���������� ��� ����
//������������ ��������:
//0 - ����������, 1 - ����������� ����,2 - ���������, -1 - ���-�� ������
int DirExecAnalysis(int mngnum, int pos,char *name)
{
    chdir(curname[mngnum]);
    //������� �������
    rewinddir(dp[mngnum]);
    int i;
    for(i=1;i<=pos;i++)
    {
        current=readdir(dp[mngnum]);
    }
    if(current->d_type==4)
        return 0;
    if(current->d_type==8)
    {
        strncpy(name,current->d_name,70);
        if(access(name,X_OK))
            return 2;
        else
            return 1;
    }
    return -1;
}


