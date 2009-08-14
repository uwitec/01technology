// ClearDataPage.cpp : 实现文件
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "ClearDataPage.h"
#include ".\cleardatapage.h"


// CClearDataPage 对话框

IMPLEMENT_DYNAMIC(CClearDataPage, CDialog)
CClearDataPage::CClearDataPage(CWnd* pParent /*=NULL*/)
	: CDialog(CClearDataPage::IDD, pParent)
{
	m_pInitParamter = NULL;
}

CClearDataPage::~CClearDataPage()
{
}

void CClearDataPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GRC, m_dpGRC);
	DDX_Control(pDX, IDC_DATETIMEPICKER_OP, m_dpOP);
}


BEGIN_MESSAGE_MAP(CClearDataPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLEARDATA, OnBnClickedButtonCleardata)
END_MESSAGE_MAP()


// CClearDataPage 消息处理程序
void	CClearDataPage::InitPage(CInitParamter	*pInitParamter)
{
	m_pInitParamter = pInitParamter;

	CTime tmNow = CTime::GetCurrentTime();
	CTime tmGRC(tmNow.GetYear(),tmNow.GetMonth(), tmNow.GetDay(), 11,59,59);
	CTimeSpan psGRC(7,0,0,0);
	tmGRC -= psGRC;
	m_dpGRC.SetTime(&tmGRC);

	CTime tmOP(tmNow.GetYear(),tmNow.GetMonth(), tmNow.GetDay(), 11,59,59);
	CTimeSpan psOP(14,0,0,0);
	tmOP -= psOP;
	m_dpOP.SetTime(&tmOP);
}
void CClearDataPage::OnBnClickedButtonCleardata()
{
	BOOL bClearGRC = ((CButton*)GetDlgItem(IDC_CHECK_GRC))->GetCheck();
	BOOL bClearOP  = ((CButton*)GetDlgItem(IDC_CHECK_OP))->GetCheck();
	BOOL bClearBC  = ((CButton*)GetDlgItem(IDC_CHECK_BC))->GetCheck();
	BOOL bClearLL  = ((CButton*)GetDlgItem(IDC_CHECK_LL))->GetCheck();

	if(bClearGRC || bClearOP || bClearBC || bClearLL)
	{
		if(AfxMessageBox(TEXT("注意此操作将丢失现有数据,请先备份数据,如已经备份请按'是(Y)'执行吗？"), MB_YESNO) != IDYES)
			return;
		CTime tmGRC(0);
		if(bClearGRC)
			m_dpGRC.GetTime(tmGRC);
		CTime tmOP(0);
		if(bClearOP)
			m_dpOP.GetTime(tmOP);
		if(Database_Clear(tmGRC.GetTime(), tmOP.GetTime(), bClearBC, bClearLL) == 0)
		{
			AfxMessageBox(TEXT("清除数据成功"));
		}
	}
	else
	{
		AfxMessageBox(TEXT("请选择您要清除的数据"));
	}
}
LONG	CClearDataPage::Database_Clear(LONGLONG llTimeGRC,
					   LONGLONG llTimeOP,
					   BOOL bClearBC,
					   BOOL bClearLL)
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
		ServerInfoDB->SetSPName(TEXT("GSP_DT_ClearData"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,4,_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@llTimeGameRoundAndChart"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t(llTimeGRC));
		ServerInfoDB->AddParamter(TEXT("@llTimeOperation"),adParamInput,adBigInt,sizeof(LONGLONG),_variant_t(llTimeOP));
		ServerInfoDB->AddParamter(TEXT("@cbClearBetScore"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bClearBC));
		ServerInfoDB->AddParamter(TEXT("@cbClearLockLogon"),adParamInput,adTinyInt,sizeof(BYTE),_variant_t((BYTE)bClearLL));
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