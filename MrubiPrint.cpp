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

int main(int argc, char *argv[])
{
	HFILE hf = HFILE_ERROR, chf = HFILE_ERROR;
	OFSTRUCT fdata;
	DWORD size, outsize;
	unsigned char *buf = NULL;
	char str[256] = {0};
	char copystr[256] = {0};
	//char dbg[256] = {0};
	char *fname = NULL;
	DWORD cursize, prevsize;

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
		 	sprintf(copystr, ".\\%s", fname);
		 	CopyFile(str, copystr, FALSE);
                        DeleteFile(str);
			ShellExecute(NULL, "print", copystr, NULL, NULL, SW_HIDE);
			Sleep(30000);
			DeleteFile(copystr);
         }
   }
   return 0;  
}
