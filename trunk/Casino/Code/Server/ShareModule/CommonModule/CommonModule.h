#ifndef COM_SERVICE_HEAD_FILE
#define COM_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ϵͳͷ�ļ�
#include "AfxHtml.h"

//ƽ̨ͷ�ļ�
#include "..\..\Template\Template.h"

#include ".\ComClassDef.h"

//ģ�鶨��
#ifdef _DEBUG
	#define COM_SERVICE_DLL_NAME	TEXT("CommonModuleD.dll")			//��� DLL ����
#else
	#define COM_SERVICE_DLL_NAME	TEXT("CommonModule.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////

//������ͷ�ļ�
#include "Encrypt.h"
#include "WebBrowser.h"
#include "HtmlBrowser.h"
#include "WinFileInfo.h"
#include "DataStorage.h"
#include "ServiceThread.h"
#include "ExceptionHandle.h"
//////////////////////////////////////////////////////////////////////////

#endif