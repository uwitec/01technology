// BJServer.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>

//////////////////////////////////////////////////////////////////////////

static AFX_EXTENSION_MODULE BlackJackServerDLL = { NULL, NULL };
//DLL ����������
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(BlackJackServerDLL, hInstance)) return 0;
		new CDynLinkLibrary(BlackJackServerDLL);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(BlackJackServerDLL);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
