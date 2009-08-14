// BackupRestorePage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataBaseTool.h"
#include "BackupRestorePage.h"
#include ".\backuprestorepage.h"


// CBackupRestorePage �Ի���

IMPLEMENT_DYNAMIC(CBackupRestorePage, CDialog)
CBackupRestorePage::CBackupRestorePage(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupRestorePage::IDD, pParent)
{
	m_pInitParamter = NULL;
}

CBackupRestorePage::~CBackupRestorePage()
{
}

void CBackupRestorePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBackupRestorePage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEBKPATH, OnBnClickedButtonBrowsebkpath)
	ON_BN_CLICKED(IDC_BUTTON_BROWSERTPATH, OnBnClickedButtonBrowsertpath)
	ON_BN_CLICKED(IDC_BUTTON_BACKUP, OnBnClickedButtonBackup)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnBnClickedButtonRestore)
END_MESSAGE_MAP()


// CBackupRestorePage ��Ϣ�������
void	CBackupRestorePage::InitPage(CInitParamter	*pInitParamter)
{
	m_pInitParamter = pInitParamter;

	CString strBackupPath = CString(m_pInitParamter->m_szDatabaseBackupPath) + CTime::GetCurrentTime().Format(TEXT("%Y-%m-%d %H_%M_%S.bak"));
	SetDlgItemText(IDC_EDIT_BACKUPPATH, strBackupPath);
}
void CBackupRestorePage::OnBnClickedButtonBrowsebkpath()
{
	CString strFilePath;
	GetDlgItemText(IDC_EDIT_BACKUPPATH, strFilePath);
	int pos = strFilePath.ReverseFind('\\');
	if(pos != -1)
		strFilePath = strFilePath.Left(pos) + "\\" ;
	TCHAR szFileName[512];
	_tcscpy(szFileName, CTime::GetCurrentTime().Format(TEXT("%Y-%m-%d %H_%M_%S.bak")));

	CFileDialog fd(FALSE, TEXT("*.bak"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("backup files(*.bak)|*.bak|All files (*.*)|*.*||")); 
	fd.m_ofn.lpstrInitialDir=strFilePath; 
	fd.m_ofn.lpstrFile =szFileName;
	fd.m_ofn.nMaxFile = 512;
	if(fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_BACKUPPATH, fd.GetPathName());
	}
}

void CBackupRestorePage::OnBnClickedButtonBrowsertpath()
{
	CString strFilePath;
	GetDlgItemText(IDC_EDIT_BACKUPPATH, strFilePath);
	int pos = strFilePath.ReverseFind('\\');
	if(pos != -1)
		strFilePath = strFilePath.Left(pos) + "\\" ;

	CFileDialog fd(TRUE, TEXT("*.bak"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("backup files(*.bak)|*.bak|All files (*.*)|*.*||")); 
	fd.m_ofn.lpstrInitialDir=strFilePath; 
	if(fd.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_RESTOREPATH, fd.GetPathName());
	}
}

void CBackupRestorePage::OnBnClickedButtonBackup()
{
	CString strFilePath;
	GetDlgItemText(IDC_EDIT_BACKUPPATH, strFilePath);
	if(strFilePath.GetLength() == 0)
		return;

	if(Database_Backup((LPCSTR)strFilePath, strFilePath.GetLength()) == 0)
	{
		int pos = strFilePath.ReverseFind('\\');
		if(pos != -1)
			strFilePath = strFilePath.Left(pos) + "\\" ;
		_tcscpy(m_pInitParamter->m_szDatabaseBackupPath, (LPCSTR)strFilePath);

		AfxMessageBox("�������ݳɹ�");
	}

}

void CBackupRestorePage::OnBnClickedButtonRestore()
{
	if(AfxMessageBox(TEXT("��ԭ���ݿ⽫��ʧ��������,ȷ��ִ����"), MB_YESNO) != IDYES)
		return;

	CString strFilePath;
	GetDlgItemText(IDC_EDIT_RESTOREPATH, strFilePath);
	if(strFilePath.GetLength() == 0)
		return;

	if(Database_Restore((LPCSTR)strFilePath, strFilePath.GetLength()) == 0)
	{
		AfxMessageBox("��ԭ���ݳɹ�");
	}
}
LONG CBackupRestorePage::Database_Backup(LPCSTR lpszPath, WORD dwPathSize)
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

		::DeleteFile((LPTSTR)lpszPath);
	
		TCHAR szSQL[1024];
		_sntprintf(szSQL, sizeof(szSQL),"USE master; "
"EXEC sp_dropdevice \'GServerInfo_Bak\';"
"EXEC sp_addumpdevice \'disk\', \'GServerInfo_Bak\', '%s'; "
"BACKUP DATABASE %s TO GServerInfo_Bak; ",
			lpszPath,szServerInfoDB);

		//ִ�в�ѯ
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->Execute(szSQL);

		return 0;

	}
	catch (IADOError * pIADOError)
	{
		AfxMessageBox(pIADOError->GetErrorDescribe());
	}
	return -1;
}
LONG CBackupRestorePage::Database_Restore(LPCSTR lpszPath, WORD dwPathSize)
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
		ServerInfoDB->SetConnectionInfo(m_pInitParamter->m_szServerDataBaseAddr,m_pInitParamter->m_szServerDataBasePipeName,
			m_pInitParamter->m_wServerDataBasePort,
			"master",m_pInitParamter->m_szServerDataBaseUser,m_pInitParamter->m_szServerDataBasePass);
		if (ServerInfoDB->OpenConnection()==false)
		{
			AfxMessageBox(TEXT("��Ϣ���ݿ�����ʧ�ܣ��޷���ȡ���ݿ���Ϣ"));
			return -1;
		}


		TCHAR szSQL[1024];
		_sntprintf(szSQL, sizeof(szSQL),"USE master; "
			"EXEC sp_dropdevice \'GServerInfo_Bak\';"
			"EXEC sp_addumpdevice \'disk\', \'GServerInfo_Bak\', '%s'; "
			"RESTORE DATABASE %s FROM GServerInfo_Bak WITH REPLACE; ",
			lpszPath,szServerInfoDB,szServerInfoDB,szServerInfoDB);

		//ִ�в�ѯ
		ServerInfoDB->ClearAllParameters();
		ServerInfoDB->Execute(szSQL);

		return 0;

	}
	catch (IADOError * pIADOError)
	{
		AfxMessageBox(pIADOError->GetErrorDescribe());
	}
	return -1;
}