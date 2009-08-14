#ifndef COM_SERVICE_HEAD_FILE
#define COM_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//系统头文件
#include "AfxHtml.h"

//平台头文件
#include "..\..\Template\Template.h"

#include ".\ComClassDef.h"

//模块定义
#ifdef _DEBUG
	#define COM_SERVICE_DLL_NAME	TEXT("CommonModuleD.dll")			//组件 DLL 名字
#else
	#define COM_SERVICE_DLL_NAME	TEXT("CommonModule.dll")			//组件 DLL 名字
#endif

//////////////////////////////////////////////////////////////////////////

//导出类头文件
#include "Encrypt.h"
#include "WebBrowser.h"
#include "HtmlBrowser.h"
#include "WinFileInfo.h"
#include "DataStorage.h"
#include "ServiceThread.h"
#include "ExceptionHandle.h"
//////////////////////////////////////////////////////////////////////////

#endif