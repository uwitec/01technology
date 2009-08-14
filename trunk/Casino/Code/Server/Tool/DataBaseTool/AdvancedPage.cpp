// AdvancedPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "AdvancedPage.h"
#include ".\advancedpage.h"


// CAdvancedPage �Ի���

IMPLEMENT_DYNAMIC(CAdvancedPage, CDialog)
CAdvancedPage::CAdvancedPage(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedPage::IDD, pParent)
{
}

CAdvancedPage::~CAdvancedPage()
{
}

void CAdvancedPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdvancedPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
END_MESSAGE_MAP()

void	CAdvancedPage::InitPage(CInitParamter	*pInitParamter)
{
	m_pInitParamter = pInitParamter;

}
// CAdvancedPage ��Ϣ�������

void CAdvancedPage::OnBnClickedButtonReset()
{
	BOOL bResetGTI = ((CButton*)GetDlgItem(IDC_CHECK_RESETGAMETREEINFO))->GetCheck();
	BOOL bResetUI  = ((CButton*)GetDlgItem(IDC_CHECK_RESETUSERINFO))->GetCheck();

	if(bResetGTI || bResetUI )
	{
		if(AfxMessageBox(TEXT("ע��˲�������ʧ��������,���ȱ�������,���Ѿ������밴'��(Y)'ִ����"), MB_YESNO) != IDYES)
			return;
		if(Database_Reset(bResetGTI,bResetUI) == 0)
		{
			AfxMessageBox(TEXT("�������ݳɹ�"));
		}
	}
	else
	{
		AfxMessageBox(TEXT("��ѡ����Ҫ���õ�����"));
	}
}
LONG	CAdvancedPage::Database_Reset(BOOL bResetGTI,
									   BOOL bResetUI)
{
	//�������ݿ�
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		AfxMessageBox(TEXT("���ݿ��������ʧ�ܣ��޷���ȡ���ݿ���Ϣ"));
		return -1;
	}
	try
	{
		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(m_pInitParamter->m_szServerDataBaseAddr,m_pInitParamter->m_szServerDataBasePipeName,m_pInitParamter->m_wServerDataBasePort,
			m_pInitParamter->m_szServerDataBaseName,m_pInitParamter->m_szServerDataBaseUser,m_pInitParamter->m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			AfxMessageBox(TEXT("��Ϣ���ݿ�����ʧ�ܣ��޷���ȡ���ݿ���Ϣ"));
			return -1;
		}

		//ִ�в�ѯ
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_DT_Reset"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@cbResetGameTreeInfo"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bResetGTI));
		ServerInfoDB->AddParamter(TEXT("@cbResetUserInfo"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bResetUI));
		ServerInfoDB->ExecuteCommand(true);

		//����ж�
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		AfxMessageBox(pIADOError->GetErrorDescribe());
	}
	return -1;
}