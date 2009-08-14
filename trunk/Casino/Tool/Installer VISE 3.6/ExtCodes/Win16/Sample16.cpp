
#include <windows.h>

extern "C" int CALLBACK _export LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
{ 
	if (cbHeapSize > 0)
		UnlockData(0);

	return 1;
} 

extern "C" int CALLBACK _export WEP(int nParam)
{

	return 1;
}

// A return value of -1 is reserved. It will cause the installer to exit immediately.
// The return value is interpreted as following when linking items to the success or failure of the external code call:
// A return value of 0 is considered a failure.
// A return value of 1 is considered a success.
// The return value may be stored in a runtime variable.
//
// The return value is ignored if the dll is called from the uninstaller.

extern "C" LONG CALLBACK _export ViseEntry(LONG lParam1, LPCSTR lpParam2, DWORD dwReserved)
{   
	int nRet = MessageBox(NULL, "Do you want to do this?", "Sample16 External Code", MB_YESNO); 
	if (nRet == IDYES)   
		return 1;  
	else
		return 0;
}
