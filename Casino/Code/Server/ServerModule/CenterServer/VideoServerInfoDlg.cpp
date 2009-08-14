// VideoServerInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "VideoServerInfoDlg.h"
#include ".\InitParamter.h"
#include ".\VideoServerPathDlg.h"


// CVideoServerInfoDlg 对话框

IMPLEMENT_DYNAMIC(CVideoServerInfoDlg, CDialog)
CVideoServerInfoDlg::CVideoServerInfoDlg(WORD wTableID,
										 CWnd* pParent /*=NULL*/)
	: CDialog(CVideoServerInfoDlg::IDD, pParent)
{
	m_wTableID = wTableID;
}

CVideoServerInfoDlg::~CVideoServerInfoDlg()
{
}

void CVideoServerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoServerInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_ADDVIDEOSERVER, OnBnClickedAddvideoserver)
	ON_BN_CLICKED(IDC_DELVIDEOSERVER, OnBnClickedDelvideoserver)
	ON_BN_CLICKED(IDC_CLEARNOUESEDVIDEOSERVER, OnBnClickedClearnouesedvideoserver)
END_MESSAGE_MAP()

BOOL CVideoServerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect tmpRc;	
	int x;
	int y;
	CString strLoadString;
	int width = 0;
	GetDlgItem(IDC_STATIC_VIDEOSERVER)->GetWindowRect(&tmpRc);
	x = 15;
	y = 20;
	tmpRc.InflateRect(-10,-10);
	tmpRc.bottom -= 26;
	CRect rc(x, y, x + tmpRc.Width(), y + tmpRc.Height());
	m_VideoServerGrid.Create(rc, this, IDC_VIDEOSERVERGRID);
	m_VideoServerGrid.SetSingleRowSelection(TRUE);
	m_VideoServerGrid.SetFixedRowSelection(FALSE);
	m_VideoServerGrid.SetListMode(TRUE);
	m_VideoServerGrid.SetEditable(FALSE);
	m_VideoServerGrid.SetRowCount(1);
	m_VideoServerGrid.SetColumnCount(2);
	m_VideoServerGrid.SetFixedRowCount(1);
	m_VideoServerGrid.SetFixedColumnCount(0);
	for(int c = 0; c < 2; c ++)
	{
		switch(c)
		{
		case 0:
			width = 50;
			strLoadString = "ID";
			break;
		case 1:
			width = 250;
			strLoadString = "路径";
			break;
		default:
			break;
		}
		m_VideoServerGrid.SetItemText(0, c, strLoadString);
		m_VideoServerGrid.SetItemFormat(0, c, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
		m_VideoServerGrid.SetColumnWidth(c, width);
	}
	m_VideoServerGrid.SetRowHeight(0, 30);
	m_VideoServerGrid.SetDefCellHeight(25);

	UpdateVideoServerList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// CVideoServerInfoDlg 消息处理程序

void CVideoServerInfoDlg::OnBnClickedAddvideoserver()
{
	CVideoServerPathDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		InsertVideoServer(m_wTableID, dlg.m_strVideoServerPath);
		UpdateVideoServerList();	
	}
}

void CVideoServerInfoDlg::OnBnClickedDelvideoserver()
{
	CCellRange cr = m_VideoServerGrid.GetSelectedCellRange();
	if(cr.GetMinRow() > 0)
	{
		WORD wVideoServerID = m_VideoServerGrid.GetItemData(cr.GetMinRow(), 0);
		if(wVideoServerID)
		{
			if(AfxMessageBox(TEXT("确定删除该视频服务器吗?"), MB_YESNO) == IDYES)
			{
				DeleteVideoServer(wVideoServerID);
				UpdateVideoServerList();
			}
		}
	}
}

void CVideoServerInfoDlg::OnBnClickedClearnouesedvideoserver()
{

	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		return ;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(InitParamter.m_szServerDataBaseAddr,
			InitParamter.m_szServerDataBasePipeName,
			InitParamter.m_wServerDataBasePort,
			InitParamter.m_szServerDataBaseName,
			InitParamter.m_szServerDataBaseUser,
			InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			return ;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ClearNousedVideoServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->ExecuteCommand(true);

	
		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		return ;

	}
	catch (IADOError * pIADOError)
	{
	}
	return ;
}


int CVideoServerInfoDlg::UpdateVideoServerList()
{
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	m_VideoServerGrid.SetRowCount(1);
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(InitParamter.m_szServerDataBaseAddr,
			InitParamter.m_szServerDataBasePipeName,
			InitParamter.m_wServerDataBasePort,
			InitParamter.m_szServerDataBaseName,
			InitParamter.m_szServerDataBaseUser,
			InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_ReadVideServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nTableID"),adParamInput,adInteger,sizeof(long),_variant_t(m_wTableID));
		ServerInfoDB->ExecuteCommand(true);

		//读取信息
		while (ServerInfoDB->IsEndRecordset()==false)
		{
			//读取信息

			WORD VideoServerID = 0;
			TCHAR szVideoServerPath[128] = "";
			ServerInfoDB->GetFieldValue(TEXT("VideoServerID"),VideoServerID);
			ServerInfoDB->GetFieldValue(TEXT("VideoServerPath"),szVideoServerPath, 128);

			int nRowIndex = m_VideoServerGrid.GetRowCount();
			m_VideoServerGrid.SetRowCount(nRowIndex + 1);
			CString strTmp;
			strTmp.Format("%d", VideoServerID);
			m_VideoServerGrid.SetItemText(nRowIndex, 0, strTmp);

			strTmp.Format("%s", szVideoServerPath);
			m_VideoServerGrid.SetItemText(nRowIndex, 1, strTmp);

			m_VideoServerGrid.SetItemData(nRowIndex, 0, VideoServerID);
		
			ServerInfoDB->MoveToNext();		
		}
		ServerInfoDB->CloseRecordset();

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		m_VideoServerGrid.Refresh();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
	}
	return -1;
}
int CVideoServerInfoDlg::DeleteVideoServer(WORD wVideoServerID)
{
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	m_VideoServerGrid.SetRowCount(1);
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(InitParamter.m_szServerDataBaseAddr,
			InitParamter.m_szServerDataBasePipeName,
			InitParamter.m_wServerDataBasePort,
			InitParamter.m_szServerDataBaseName,
			InitParamter.m_szServerDataBaseUser,
			InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_DeleteVideoServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nVideoServerID"),adParamInput,adInteger,sizeof(long),_variant_t(wVideoServerID));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		m_VideoServerGrid.Refresh();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
	}
	return -1;
}
int CVideoServerInfoDlg::InsertVideoServer(WORD wTableID, LPCSTR strVideoServerPath)
{
	CInitParamter InitParamter;
	InitParamter.LoadInitParamter();

	m_VideoServerGrid.SetRowCount(1);
	//创建数据库
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		return -1;
	}
	try
	{
		//连接数据库
		ServerInfoDB->SetConnectionInfo(InitParamter.m_szServerDataBaseAddr,
			InitParamter.m_szServerDataBasePipeName,
			InitParamter.m_wServerDataBasePort,
			InitParamter.m_szServerDataBaseName,
			InitParamter.m_szServerDataBaseUser,
			InitParamter.m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			return -1;
		}

		//执行查询
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->SetSPName(TEXT("GSP_GP_InsertVideoServer"));
		ServerInfoDB->AddParamter(TEXT("RETURN_VALUE"),adParamReturnValue,adInteger,sizeof(long),_variant_t((long)0));
		ServerInfoDB->AddParamter(TEXT("@nGameTableID"),adParamInput,adInteger,sizeof(long),_variant_t(wTableID));
		ServerInfoDB->AddParamter(TEXT("@strVideoServerPath"),adParamInput,adChar,CountString(strVideoServerPath),_variant_t(strVideoServerPath));
		ServerInfoDB->ExecuteCommand(true);

		//结果判断
		LONG lReturnCode=ServerInfoDB->GetReturnValue();

		m_VideoServerGrid.Refresh();

		return lReturnCode;

	}
	catch (IADOError * pIADOError)
	{
	}
	return -1;
}