#include <stdio.h>
#include <ncurses.h>

#include "fmng.h"

extern WINDOW *menu[2],*mng[2];

int main(void)
{
    int ch,mngnum,pos,check;
    char str[70];
    DIR *dp[2];
    mngnum=0;
    pos=1;
    InitScreen();
    dp[0]=StartDir(".",0);
    if(dp[0]==NULL)
    {
        DeleteScreen();
        return 0;
    }
    dp[1]=StartDir("/",1);
    if(dp[1]==NULL)
    {
        DeleteScreen();
        FinishDir(0);
        return 0;
    }
    SelectDir(0,1,1);

    while((ch=wgetch(menu[0]))!=KEY_F(3))
    {
        if(ch==KEY_UP)
        {
            pos=UpDownHandler(-1,mngnum,pos);
        }
        if(ch==KEY_DOWN)
        {
            pos=UpDownHandler(1,mngnum,pos);
        }
        if(ch==KEY_LEFT)
        {
            mngnum=LeftRightHandler(-1,mngnum,pos);
            pos=1;
        }
        if(ch==KEY_RIGHT)
        {
            mngnum=LeftRightHandler(1,mngnum,pos);
            pos=1;
        }
        if(ch==KEY_F(1))
        {
            check=PrintInfo(mngnum,pos);
        }
        if(ch==KEY_F(2))
        {
            ActivateCMD();
        }
        //отлавливаем enter по коду
        if(ch==10)
        {
            check=DirExecAnalysis(mngnum,pos,str);
            if(check==-1)
                continue;
            if(check==0)
            {
                pos=ChangeDir(mngnum,pos);
            }
            if(check==1)
            {
                StartFile(str);
                RestoreState();
            }
            if(check==2)
            {
                OpenFile(str);
                RestoreState();
            }
        }

    }
    FinishDir(0);
    FinishDir(1);
    DeleteScreen();
    return 0;
}

