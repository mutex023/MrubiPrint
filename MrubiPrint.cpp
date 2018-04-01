/* Alternative to AirPrint, keeps monitoring for a file in the 'c:\MrubiPrint' dir.
* Supported files are .txt, .pdf and .doc/.docx
* Once the file is detected and opened, it will send this file to the printer and
* delete it. So the use case is to ftp the file to print from ipad/iphone to your
* Windows PC's dir at c:\MrubiPrint. Needless to say your PC
* should have an FTP server like FileZilla running
Author : MrubiRishi
License : You have the right to copy !
Written on : 31 March 2018
note : you might have to link the libwinmm.o(devcpp) file or the winmm.lib (vc++) to compile
       it successfully
*/

#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <mmsystem.h>

SERVICE_STATUS          ServiceStatus; 
SERVICE_STATUS_HANDLE   hStatus; 


void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
int InitService();

char *GetFileFromDir(char *dirname)
{
   WIN32_FIND_DATA ffd;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   static char filename[256];
   char szDir[256];

   strcpy(szDir, dirname);
   strcat(szDir, "\\*");
   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
      return NULL;
   } 

   do {
		  if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		  {
		   	 ;
		  }
		  else
		  {
		     strcpy(filename, ffd.cFileName);
		     return filename;
		  }
	}
   while (FindNextFile(hFind, &ffd) != 0);
   return NULL;
}

int main() 
{
	HFILE hf = HFILE_ERROR, chf = HFILE_ERROR;
	OFSTRUCT fdata;
	DWORD size, outsize;
	unsigned char *buf = NULL;
	char str[256] = {0};
	//char dbg[256] = {0};
	char *fname = NULL;
	DWORD cursize, prevsize;

   /*SERVICE_TABLE_ENTRY ServiceTable[2];
   ServiceTable[0].lpServiceName = "MrubiPrint";
   ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

   ServiceTable[1].lpServiceName = NULL;
   ServiceTable[1].lpServiceProc = NULL;
   // Start the control dispatcher thread for our service
   StartServiceCtrlDispatcher(ServiceTable); */

   while (1) {
         Sleep(3000);
         fname = GetFileFromDir("c:\\MrubiPrint");
         if (fname) {
            hf = HFILE_ERROR;
            cursize = prevsize = 0;
            sprintf(str, "c:\\MrubiPrint\\%s", fname);
            //MessageBox(NULL, str, "Mrubi, found file", MB_OK);
            while (hf == HFILE_ERROR) {
				Sleep(1000);
				hf = OpenFile(str, &fdata, OF_READ);
				if (hf != HFILE_ERROR) {
                    //MessageBox(NULL, str, "Mrubi, opened file", MB_OK);
					chf = hf;
					cursize = GetFileSize((void *)hf, NULL);
					//sprintf(dbg, "file size is %d bytes", cursize);
					//MessageBox(NULL, dbg, "Mrubi", MB_OK);
					if (prevsize != cursize) {
						prevsize = cursize;
     					hf = HFILE_ERROR;
					}
					CloseHandle((void *)chf);
				}
			}
			
			//sprintf(str, "File: 'c:\\MrubiPrint\\%s'; Size: %d bytes", fname, cursize);
		 	//MessageBox(NULL, str, "Mrubi, Printing...", MB_OK);
		 	sprintf(str, "c:\\MrubiPrint\\%s", fname);
			ShellExecute(NULL, "print", str, NULL, NULL, SW_HIDE);
			Sleep(5000);
            DeleteFile(str);
         }
   }
   return 0;  
}

/*
void ServiceMain(int argc, char** argv) 
{ 
   HFILE hf = HFILE_ERROR;
   OFSTRUCT fdata;
   DWORD size;
   unsigned char *buf = NULL;
   
   MessageBox(
        NULL,
        "MrubiPrint entry",
        "Mrubi",
        MB_OK
    );
   ServiceStatus.dwServiceType = 
      SERVICE_WIN32; 
   ServiceStatus.dwCurrentState = 
      SERVICE_START_PENDING; 
   ServiceStatus.dwControlsAccepted   =  
      SERVICE_ACCEPT_STOP | 
      SERVICE_ACCEPT_SHUTDOWN;
   ServiceStatus.dwWin32ExitCode = 0; 
   ServiceStatus.dwServiceSpecificExitCode = 0; 
   ServiceStatus.dwCheckPoint = 0; 
   ServiceStatus.dwWaitHint = 0; 
   
   hStatus = RegisterServiceCtrlHandler(
      "", 
      (LPHANDLER_FUNCTION)ControlHandler); 
   if (hStatus == (SERVICE_STATUS_HANDLE)0) {
       MessageBox(
            NULL,
            "Failed to register service",
            "Mrubi",
            MB_OK
        );
        return; 
   }
   ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
   SetServiceStatus (hStatus, &ServiceStatus);
   int n = 0;
   while (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
         Sleep(2000); 
         hf = OpenFile("c:\\MrubiPrint\\vayavya", &fdata, OF_READ);
         if (hf != HFILE_ERROR) {
            size = GetFileSize((void *)hf, NULL);
            buf = (unsigned char *)malloc(size);
            if (buf) {
                 if (ReadFile((void *)hf, buf, size, NULL, NULL)) {
                    RawDataToPrinter("Canon MF3010", buf, size);
                    CloseHandle((void *)hf);
                    DeleteFile("c:\\MrubiPrint\\vayavya");
                }
            }
         }
   }
}

void ControlHandler(DWORD request) 
{ 
   switch(request) 
   { 
      case SERVICE_CONTROL_STOP: 
        
         ServiceStatus.dwWin32ExitCode = 0; 
         ServiceStatus.dwCurrentState = SERVICE_STOPPED; 
         SetServiceStatus (hStatus, &ServiceStatus);
         return; 
 
      case SERVICE_CONTROL_SHUTDOWN: 
         
         ServiceStatus.dwWin32ExitCode = 0; 
         ServiceStatus.dwCurrentState = SERVICE_STOPPED; 
         SetServiceStatus (hStatus, &ServiceStatus);
         return; 
        
      default:
         break;
    } 
 
    
    SetServiceStatus (hStatus, &ServiceStatus);
 
    return; 
}
*/
