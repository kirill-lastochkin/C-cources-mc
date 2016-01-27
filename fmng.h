#ifndef _FMNG_H
#define _FMNG_H
#include <dirent.h>
//графические функции
void InitScreen(void);
void DeleteScreen(void);
int PrintDir(int mngnum,int pos,char* name);
void ClearPanel(int mngnum);
void SelectDir(int mngnum,int pos,int mode);
int LeftRightHandler(int x0,int mngnum,int pos);
int UpDownHandler(int y0,int mngnum,int pos);
int PrintInfo_g(char* info, long data, int line, int col);

//работа с директориями
DIR* StartDir(char *path, int mngnum);
int ChangeDir(int mngnum,int pos);
int PrintInfo(int mngnum,int pos);
void FinishDir(int mngnum);


#endif
