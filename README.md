# MrubiPrint
Alternative solution to AirPrint for printing from iOS and android devices
to any printer connected to a windows PC

1. Ensure your printer is connected to your PC and switched on.
3. Create a folder named “MrubiPrint” under C:\
4. Copy MrubiPrint.exe to the START>Program Files > Startup folder
6. Install an FTP server application like FileZilla, setup the server so that 
c:\MrubiPrint is set as the root folder of the server. Give read, write and delete permissions to the folder in FTP server settings.
7. Restart windows
8. On your iPhone/iPad or android install an FTP client app. FTPManager is a good enough app for iOS. For android ESFileExplorer is suggested.
9. Connect to your windows FTP server from the app
10. That’s it, from now on you just need to copy a file (supported types for printing are .doc, .docx, .txt, .pdf, .xls, .xlsx, .ppt, .pptx, .jpg, .png) over FTP from your device to the PC  at c:\MrubiPrint and within 5 secs the document printing will start.
11. After printing the document is deleted from c:\MrubiPrint
