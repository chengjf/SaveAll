#include "iniparser.h"
#include <time.h>
#include <io.h>

#ifndef _SAVEALL_H_
#define _SAVEALL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

DLLIMPORT void  HelloWorld();
DLLIMPORT void  RegisterCallback(int Index, void *Addr);
DLLIMPORT char* IdentifyPlugIn(int);
DLLIMPORT char* CreateMenuItem(int);
DLLIMPORT void  OnMenuClick(int);
DLLIMPORT char* About();
DLLIMPORT void  Configure();
DLLIMPORT void  OnActivate();

struct datetime{
	long date;
	long time;
} datetime;
#endif
