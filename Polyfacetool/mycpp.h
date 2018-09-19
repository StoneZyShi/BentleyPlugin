#pragma once
#include <string>

#define LOGFUN_MSG
#define logfunmsg(path,file,line,fun) infoModel(path,funMsg(file,line,fun));

#ifdef LOGFUN_MSG
#define lmsg(path) logfunmsg(path,__FILE__, __LINE__, __FUNCTION__)
#else
#define lmsg(path) path;
#endif

//#define FUN_MSG
#define funmsg(file,line,fun) pris(funMsg(file,line,fun));
#ifdef FUN_MSG
#define fmsg funmsg(__FILE__, __LINE__, __FUNCTION__)
#else
#define fmsg
#endif


void strToTransform(std::string str, double myTransform[4][4]);
std::string transformToStr(double myTransform[4][4]);
void toTransform(double transformA[4][4], double transformB[4][4]);

std::string funMsg(char* file, int line, char* fun);
