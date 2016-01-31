#ifndef _FMNG_H
#define _FMNG_H
#include <dirent.h>
//графические функции
void InitScreen(void); //инициализация экрана
void DeleteScreen(void); //завершение работы с экраном
int PrintDir(int mngnum, int pos, char c, char* name); //распечатать строчку на панели
void ClearPanel(int mngnum); //очистить панель
void SelectDir(int mngnum,int pos,int mode); //выделить или убрать выделение с директории
int LeftRightHandler(int x0,int mngnum,int pos); //обработка стрелки лево-право
int UpDownHandler(int y0,int mngnum,int pos); //обработка стрели вверх-вниз
int PrintInfo_g(char* info, long data, int line, int col); //печать информации в окно
void RestoreState(void); //восстановление свойство окон
void ActivateCMD(void); //активировать "консоль"
void PrintCMDResult(char,int); //печать результат работы консольной команды
void ErrorMsg(char *str); //напечать инфо об ошибке

//работа с директориями
DIR* StartDir(char *path, int mngnum); //начать работу с директорией
int ChangeDir(int mngnum,int pos); //изменить текущую директорию
int PrintInfo(int mngnum,int pos); //печать инфо о файле/директории
void FinishDir(int mngnum); //закончить работу с потоком директорий
int DirExecAnalysis(int mngnum, int pos,char *name); //анализ типа файла

//работа с процессами
int StartFile(char *name); //запустить исполняемый файл
int ExecuteCMD(char *str); //выполнить команду консоли
int OpenFile(char *name); //открыть файл текстовым редактором

#endif
