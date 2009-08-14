// GameTypeInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "GameTypeInfoDlg.h"
#include ".\gametypeinfodlg.h"


// CGameTypeInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CGameTypeInfoDlg, CDialog)
CGameTypeInfoDlg::CGameTypeInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameTypeInfoDlg::IDD, pParent)
	, m_strTypeName(_T(""))
	, m_dwSortID(100)
	, m_nImageID(0)
	, m_strImageID(_T(""))
{
}

CGameTypeInfoDlg::~CGameTypeInfoDlg()
{
}

void CGameTypeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TYPENAME, m_strTypeName);
	DDX_Text(pDX, IDC_SORT_ID, m_dwSortID);

	DDX_CBString(pDX, IDC_IMAGEID, m_strImageID);
}


BEGIN_MESSAGE_MAP(CGameTypeInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CGameTypeInfoDlg ��Ϣ�������

BOOL CGameTypeInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_TYPENAME))->LimitText(31);
	const char* strImageName[4] = 
	{
		"������Ƶ",
		"��ս",
		"����",
		"����",
	};
	CString str;
	for(int i = 0; i < sizeof(strImageName) / sizeof(strImageName[0]); i ++)
	{
		str.Format("%d", i);
		((CComboBox*)GetDlgItem(IDC_IMAGEID))->AddString(strImageName[i]);
	}
	((CComboBox*)GetDlgItem(IDC_IMAGEID))->SetCurSel(m_nImageID);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGameTypeInfoDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_strTypeName.IsEmpty())
	{
		AfxMessageBox("���ֲ���Ϊ��");
		return ;
	}
	m_nImageID = ((CComboBox*)GetDlgItem(IDC_IMAGEID))->GetCurSel();
	OnOK();
}
