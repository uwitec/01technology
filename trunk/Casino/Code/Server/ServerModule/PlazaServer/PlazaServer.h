#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
class CPlazaServerApp : public CWinApp
{
	//��������
public:
	//���캯�� 
	CPlazaServerApp();

	//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
