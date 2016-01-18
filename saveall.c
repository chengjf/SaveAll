#include "saveall.h"
#include <windows.h>

// Description of this Plug-In (as displayed in Plug-In configuration dialog)
char *const Desc = "Save All Plugin v0.1";
int PlugInID;
char basedir[100];

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}


void load_config();
void GetDate(struct datetime* datetime);
void SaveFile(int number);
void SaveAll();

void ShowMessage(const char* Data)
{
	MessageBox(NULL,Data, "Save All Plugin",0);
}

void HelloWorld()
{
	ShowMessage("Hi");
}

// Declaration of PL/SQL Developer callback functions

char* (*SYS_Registry)();
char* (*SYS_OracleHome)();
char* (*SYS_RootDir)();
void  (*IDE_MenuState)(int ID, int Index, BOOL Enabled);
void  (*IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName);
int	  (*SQL_Execute)(char *SQL);
BOOL  (*SQL_Eof)();
int	  (*SQL_Next)();
char* (*SQL_Field)(int Field);
char* (*IDE_Filename)();
int	  (*IDE_GetWindowCount)();
BOOL  (*IDE_SelectWindow)(int Index);
BOOL  (*IDE_CanSaveWindow)();
BOOL  (*IDE_SaveFile)();
char* (*IDE_Filename)();
void  (*IDE_SetFilename)(char *Filename);

// Registration of PL/SQL Developer callback functions
void RegisterCallback(int Index, void *Addr)
{
  switch (Index)
  {
	case 2 :
	  memcpy(&SYS_Registry, &Addr, sizeof(Addr));
	  break;
	case 3 :
	  memcpy(&SYS_RootDir, &Addr, sizeof(Addr));
	  break;
	case 4 :
	  memcpy(&SYS_OracleHome, &Addr, sizeof(Addr));
	  break;
	case 10 :
		memcpy(&IDE_MenuState, &Addr, sizeof(Addr));
	  break;
	case 13 :
		memcpy(&IDE_Filename, &Addr, sizeof(Addr));
	  break;
	case 22 :
		memcpy(&IDE_SaveFile, &Addr, sizeof(Addr));
	  break;
	case 23 :
		memcpy(&IDE_Filename, &Addr, sizeof(Addr));
	  break;
	case 29 :
		memcpy(&IDE_SetFilename, &Addr, sizeof(Addr));
	  break;
	case 80 :
		memcpy(&IDE_GetWindowCount, &Addr, sizeof(Addr));
	  break;
	case 81 :
		memcpy(&IDE_SelectWindow, &Addr, sizeof(Addr));
	  break;
	case 101 :
		memcpy(&IDE_CanSaveWindow, &Addr, sizeof(Addr));
	  break;
  }
}

//---------------------------------------------------------------------------
// Plug-In identification, a unique identifier is received and
// the description is returned
char* IdentifyPlugIn(int ID)
{
  PlugInID = ID;
  return Desc;
}
//---------------------------------------------------------------------------
// Creating a menu item
char* CreateMenuItem(int Index)
{
  switch (Index)
  {
	case 1 : return "Plugins / &Save All";
  }
  return "";
}
//---------------------------------------------------------------------------
// The menu item got selected
void OnMenuClick(int Index)
{
  switch (Index)
  {
	case 1 :{
	  SaveAll();
	  break;
	  }
  }
}
//---------------------------------------------------------------------------

char* About(){
	return "SaveAllPlugin v0.1\n2016:chengjianfeng\nEmail:sharpkingdom@gmail.com\n\nBest wishes to you!";
}

void  Configure()
{
	char* root = (*SYS_RootDir)();
	char msg[200];
	msg[0] = 0;
	strcat(msg, "Please modify config file:");
	strcat(msg, root);
	strcat(msg, "\\PlugIns\\");
	strcat(msg, "saveall.ini\n");
	strcat(msg, "current basedir is ");
	strcat(msg, basedir);
	ShowMessage(msg);
}

void  OnActivate(){
	load_config();
}

void load_config(){
	char *value;
	char msg[100];
	msg[0] = 0;

	/* ini_r is a structure that holds state to make this reader
	 * reentrant */
	struct read_ini *ini_r = NULL;

	/* "test.ini" will be parsed into "ini" */

	char* root = (*SYS_RootDir)();
	char filename[100];
	filename[0] = 0;
	strcat(filename, root);
	strcat(filename, "\\PlugIns\\");
	strcat(filename, "saveall.ini");
	struct ini *ini = read_ini(&ini_r, filename);

	/* pretty printing of ini structure */
	ini_pp(ini);

	/* retrieve a value */
	value = ini_get_value(ini, "section", "basedir");
	if(value != NULL){
		strcat(msg, "value is\n");
		strcat(msg, value);
		strcpy(basedir, value);
//		ShowMessage(msg);
	}else{
//		ShowMessage("cannot get key");
	}

	/* free memory */
	destroy_ini(ini);
	cleanup_readini(ini_r);
}

void SaveAll(){
	char* fileName = IDE_Filename();
	int cnt = IDE_GetWindowCount();

	for(;cnt>0;cnt--){
		if(IDE_SelectWindow(cnt-1)){
			if(strcmp(fileName,"")==0){
				SaveFile(cnt);
			}else{
				if(IDE_CanSaveWindow()){
					if(IDE_SaveFile()){
						//IDE_CloseFile();
					}
				}else{
					ShowMessage("Can not save");
				}
			}
		}else{
			ShowMessage("selectwindow error");
		}
	}
}

void SaveFile(int number){
	struct datetime p ={0};
	GetDate(&p);
	char date[16];
	int i = 0;
	char time[16];
	int j = 0;
	char num[4];
	sprintf(date, "%u", p.date);
	sprintf(time, "%u", p.time);
	sprintf(num, "%d", number);

	char filename[100];
	filename[0] = 0;
	strcat(filename, basedir);
	strcat(filename, "\\");
	strcat(filename, date);
	if(!access(filename, F_OK)==0){
		int rs = mkdir(filename);
		if(0 != rs){
			ShowMessage("cannot mkdir");
		}
	}
	strcat(filename, "\\");
	strcat(filename, time);
	strcat(filename, "_");
	strcat(filename, num);
	strcat(filename, ".sql");


	IDE_SetFilename(filename);
	if(IDE_CanSaveWindow()){
		if(IDE_SaveFile()){
			//IDE_CloseFile();
		}
	}else{
		ShowMessage("Can not save");
	}
}

void GetDate(struct datetime* datetime){

	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	int year = 1900+p->tm_year;
	int month = 1+p->tm_mon;
	int mday = p->tm_mday;

	int hour = (p->tm_hour + 8)%24;
	int minute = p->tm_min;
	int second = p->tm_sec;

	long date = year*10000+month*100+mday;
	long time = hour*10000+minute*100+second;
	datetime->date = date;
	datetime->time = time;
}
