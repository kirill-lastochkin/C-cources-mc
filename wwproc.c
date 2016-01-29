#include "fmng.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


int StartFile(char *name)
{
    pid_t pid;
    int status;
    pid=fork();
    if(pid==0)
    {
        execl(name,name,NULL);
        perror("file error\n");
        exit(1);
    }
    else
    {
        wait(&status);
    }
    return 0;
}

int ExecuteCMD(char *str)
{
    int cnt;
    char *coms[20],ps,pch1[15];
    pid_t pid;
    int status,fd[2],fd1[2];
    char *pch=strtok(str," ");
    for(cnt=0;pch!=NULL;cnt++)
    {
        coms[cnt]=pch;
        pch=strtok(NULL," ");
    }
    //
    pipe(fd);
    pipe(fd1);
    pid=fork();
    if(pid==0)
    {
        close(fd[0]);
        dup2(fd[1],1);
        strncpy(pch1,"/bin/",6);
        strncat(pch1,coms[0],3);
        execl(pch1,pch1,coms[1],NULL);
        perror("execl error\n");
        exit(1);
    }
    else
    {
        pid=fork();
        if(pid==0)
        {
            close(fd[1]);
            close(fd1[0]);
            dup2(fd[0],0);
            dup2(fd1[1],1);
            strncpy(pch1,"/usr/bin/",10);
            strncat(pch1,coms[3],4);
            execl(pch1,pch1,coms[4],NULL);
            perror("\nerr\n");
            exit(2);
        }
        else
        {
            size_t size;
            int pos=1;
            close(fd1[1]);
            while((size=read(fd1[0],&ps,1))!=0)
            {
                PrintCMDResult(ps,pos);
                pos++;
                if(pos>500) break;
            }
                waitpid(pid,&status,0);
        }
    }
    //
    return 0;
}
