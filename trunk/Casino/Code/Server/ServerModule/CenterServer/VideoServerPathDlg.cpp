// VideoServerPathDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "VideoServerPathDlg.h"


// CVideoServerPathDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoServerPathDlg, CDialog)
CVideoServerPathDlg::CVideoServerPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoServerPathDlg::IDD, pParent)
	, m_strVideoServerPath(_T("rtmp://127.0.0.1/video"))
{
}

CVideoServerPathDlg::~CVideoServerPathDlg()
{
}

void CVideoServerPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VIDEOSERVERPATH, m_strVideoServerPath);
}


BEGIN_MESSAGE_MAP(CVideoServerPathDlg, CDialog)
END_MESSAGE_MAP()


// CVideoServerPathDlg ��Ϣ�������
