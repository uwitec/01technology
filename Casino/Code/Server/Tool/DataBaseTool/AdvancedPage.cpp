// AdvancedPage.cpp : 实现文件
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "AdvancedPage.h"
#include ".\advancedpage.h"


// CAdvancedPage 对话框

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
// CAdvancedPage 消息处理程序

void CAdvancedPage::OnBnClickedButtonReset()
{
	BOOL bResetGTI = ((CButton*)GetDlgItem(IDC_CHECK_RESETGAMETREEINFO))->GetCheck();
	BOOL bResetUI  = ((CButton*)GetDlgItem(IDC_CHECK_RESETUSERINFO))->GetCheck();

	if(bResetGTI || bResetUI )
	{
		if(AfxMessageBox(TEXT("注意此操作将丢失现有数据,请先备份数据,如已经备份请按'是(Y)'执行吗？"), MB_YESNO) != IDYES)
			return;
		if(Database_Reset(bResetGTI,bResetUI) == 0)
		{
			AfxMessageBox(TEXT("重置数据成功"));
		}
	}
	else
	{
		AfxMessageBox(TEXT("请选择您要重置的数据"));
	}
}
LONG	CAdvancedPage::Database_Reset(BOOL bResetGTI,
									   BOOL bResetUI)
{
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		AfxMessageBox(TEXT("数据库组件创建失败，无法获取数据库信息"));
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(m_pInitParamter->m_szServerDataBaseAddr,m_pInitParamter->m_szServerDataBasePipeName,m_pInitParamter->m_wServerDataBasePort,
			m_pInitParamter->m_szServerDataBaseName,m_pInitParamter->m_szServerDataBaseUser,m_pInitParamter->m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			AfxMessageBox(TEXT("信息数据库连接失败，无法获取数据库信息"));
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_DT_Reset"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@cbResetGameTreeInfo"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bResetGTI));
		ServerInfoDB->AddParamter(TEXT("@cbResetUserInfo"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bResetUI));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
		AfxMessageBox(pIADOError->GetErrorDescribe());
	}
	return -1;
}