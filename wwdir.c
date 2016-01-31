#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fmng.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

DIR *dp[2];     //2 потока директорий для 2 панелей
struct dirent *current;
char *curname[2]; //текущая директория для 2 панелей

//инициализация работы с потоком директорий
//вход - путь и номер панели
//выход - указатель на поток
DIR* StartDir(char *path, int mngnum)
{
    int i;
    dp[mngnum]=opendir(path);
    if(!dp[mngnum])
    {
        perror("dir not opned");
        return NULL;
    }
    //переходим в нжную папку
    i=chdir(path);
    if(i==-1)
    {
        perror("dir not changed");
        closedir(dp[mngnum]);
        return NULL;
    }
    //очищаем панель
    ClearPanel(mngnum);
    i=1;
    //заполняем панель именами без сортировки
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
    //сохраняем имя текущего каталога
    curname[mngnum]=(char*)calloc(70,sizeof(char));
    if(getcwd(curname[mngnum],69)==NULL)
    {
        perror("error with getcwd\n");
        free(curname[mngnum]);
        return NULL;
    }
    return dp[mngnum];
}

//смена каталога
//вход - номер панели, позиция, на которой находится нжное имя
int ChangeDir(int mngnum,int pos)
{
    //переходим в текущую папк нжной панели
    chdir(curname[mngnum]);
    //находим позицию
    rewinddir(dp[mngnum]);
    int i;
    for(i=1;i<=pos;i++)
    {
        current=readdir(dp[mngnum]);
    }
    //закрываем поток и переоткрываем для новой текущей папки
    closedir(dp[mngnum]);
    dp[mngnum]=opendir(current->d_name);
    if(!dp[mngnum])
    {
        perror("error of changing dir\n");
        //при неудаче(например пытались перейти в файл) просто заново инициализируем поток
        //с текущим именем, т.е. каталог не меняется
        free(curname[mngnum]);
        dp[mngnum]=StartDir(".",mngnum);
        return pos;
    }
    //если не было ошибок, меняем
    chdir(current->d_name);
    //сохраняем новый текущий путь
    if(getcwd(curname[mngnum],69)==NULL)
    {
        perror("error with getcwd\n");
        return 0;
    }
    //заполняем нужную панель
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

//печать информации о файле
int PrintInfo(int mngnum,int pos)
{
    chdir(curname[mngnum]);
    rewinddir(dp[mngnum]);
    int i;
    long int data;
    struct stat *st;
    //находим нужную позицию
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
    //по очереди печатаем все в спец.окно
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

//завершение работы с директориями
void FinishDir(int mngnum)
{
    closedir(dp[mngnum]);
    free(curname[mngnum]);
}

//определние директория или файл
//возвращаемое значение:
//0 - директория, 1 - исполняемый файл,2 - текстовый, -1 - что-то другое
int DirExecAnalysis(int mngnum, int pos,char *name)
{
    chdir(curname[mngnum]);
    //находим позицию
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


