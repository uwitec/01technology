// DataBaseTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDataBaseToolApp:
// �йش����ʵ�֣������ DataBaseTool.cpp
//

class CDataBaseToolApp : public CWinApp
{
public:
	CDataBaseToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDataBaseToolApp theApp;
