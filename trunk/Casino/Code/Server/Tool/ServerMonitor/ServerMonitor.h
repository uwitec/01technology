// ServerMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CServerMonitorApp:
// �йش����ʵ�֣������ ServerMonitor.cpp
//

class CServerMonitorApp : public CWinApp
{
public:
	CServerMonitorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerMonitorApp theApp;
