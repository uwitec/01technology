// GameKindInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "GameKindInfoDlg.h"
#include ".\gamekindinfodlg.h"


// CGameKindInfoDlg 对话框

IMPLEMENT_DYNAMIC(CGameKindInfoDlg, CDialog)
CGameKindInfoDlg::CGameKindInfoDlg(BOOL bNewGameKind,CWnd* pParent /*=NULL*/)
	: CDialog(CGameKindInfoDlg::IDD, pParent)
	, m_strKindName(_T(""))
	, m_dwSortID(100)
	, m_dwTableCount(200)
	, m_dwCellScore(100)
	, m_dwTaxRate(50)
	, m_dwGameProcessType(ID_B_HOE)
	, m_dwHighScore(10000)
	, m_dwLessScore(1000)
	, m_dwAIUserCount(0)
	, m_dwAILevel(80)
{
	m_bNewGameKind = bNewGameKind;
}

CGameKindInfoDlg::~CGameKindInfoDlg()
{
}

void CGameKindInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KINDNAME, m_strKindName);
	DDX_Text(pDX, IDC_SORT_ID, m_dwSortID);
	DDX_Text(pDX, IDC_TABLECOUNT, m_dwTableCount);
	DDX_Text(pDX, IDC_HIGHSCORE, m_dwHighScore);
	DDX_Text(pDX, IDC_CELLSCORE, m_dwCellScore);
	DDX_Text(pDX, IDC_LESSSCORE, m_dwLessScore);
	DDX_Text(pDX, IDC_TAXRATE, m_dwTaxRate);
	DDX_Text(pDX, IDC_AIUSERCOUNT, m_dwAIUserCount);
	DDX_Control(pDX, IDC_GAMEPROCESSTYPE, m_comboGameProcessType);
	DDX_Control(pDX, IDC_AILEVEL, m_sliderAILevel);
	DDX_Slider(pDX, IDC_AILEVEL, m_dwAILevel);
}


BEGIN_MESSAGE_MAP(CGameKindInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_GAMEPROCESSTYPE, OnCbnSelchangeGameprocesstype)
	ON_EN_CHANGE(IDC_CELLSCORE, OnEnChangeCellscore)
	ON_EN_CHANGE(IDC_TABLECOUNT, OnEnChangeTablecount)
END_MESSAGE_MAP()


// CGameKindInfoDlg 消息处理程序

BOOL CGameKindInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strKindName = m_strKindName;
	DWORD dwAIUserCount = m_dwAIUserCount;
	DWORD dwAILevel = m_dwAILevel;

	m_sliderAILevel.SetRange(0, 100);

	((CEdit*)GetDlgItem(IDC_KINDNAME))->LimitText(31);
	for(int i = 0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		int index = m_comboGameProcessType.AddString(g_GameProcessItem[i].szTypeName);
		m_comboGameProcessType.SetItemData(index, g_GameProcessItem[i].wProcessType);
		if(m_dwGameProcessType == g_GameProcessItem[i].wProcessType)
		{
			m_comboGameProcessType.SetCurSel(index);
			UpdateGameKindInfo(m_dwGameProcessType);
			UpdateGameBetLimit(m_dwGameProcessType,GetDlgItemInt(IDC_CELLSCORE));
		}
	}
	if(m_bNewGameKind == FALSE)
	{
		m_comboGameProcessType.EnableWindow(FALSE);
		m_strKindName = strKindName;
		m_dwAIUserCount = dwAIUserCount;
		m_dwAILevel = dwAILevel;

		//SetDlgItemText(IDC_KINDNAME, m_strKindName);
		UpdateData(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGameKindInfoDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_strKindName.IsEmpty())
	{
		AfxMessageBox("名字不能为空");
		return ;
	}
	if(m_dwTableCount <= 0)
	{
		AfxMessageBox("游戏桌数目必须大于 0");
		return ;
	}
	if(m_dwCellScore <= 0)
	{
		AfxMessageBox("Bet下限必须大于 0");
		return ;
	}
	if(m_dwHighScore < m_dwCellScore)
	{
		AfxMessageBox("Bet上限必须大于 Bet下限");
		return ;
	}
	m_dwGameProcessType = m_comboGameProcessType.GetItemData(m_comboGameProcessType.GetCurSel());
	OnOK();
}

void CGameKindInfoDlg::OnCbnSelchangeGameprocesstype()
{
	UpdateData();
	m_dwGameProcessType = m_comboGameProcessType.GetItemData(m_comboGameProcessType.GetCurSel());
	UpdateGameKindInfo(m_dwGameProcessType);
	UpdateGameBetLimit(m_dwGameProcessType,GetDlgItemInt(IDC_CELLSCORE));
}
void CGameKindInfoDlg::UpdateGameKindInfo(WORD wProcessType)
{
	UpdateGameKindName(wProcessType);

	if(IsVideoGameProcessType(wProcessType))
	{
		SetDlgItemInt(IDC_LESSSCORE, 0);
		SetDlgItemInt(IDC_TAXRATE, 0);
		SetDlgItemInt(IDC_TABLECOUNT, 2);
	}
	else
	{
		switch(wProcessType) 
		{
		case ID_B_HOE:
			{
				SetDlgItemInt(IDC_CELLSCORE, 4);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_LAND:
			{
				SetDlgItemInt(IDC_CELLSCORE, 20);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_UNCOVERPIG:
			{
				SetDlgItemInt(IDC_CELLSCORE, 10);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_BACK_JACK:
			{
				SetDlgItemInt(IDC_CELLSCORE, 100);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_UPGRADE:
			{
				SetDlgItemInt(IDC_CELLSCORE, 20);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_SPARROW:
			{
				SetDlgItemInt(IDC_CELLSCORE, 20);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_SHOWHAND:
			{
				SetDlgItemInt(IDC_CELLSCORE, 100);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		case ID_B_THREELEAF:
			{
				SetDlgItemInt(IDC_CELLSCORE, 100);
				SetDlgItemInt(IDC_TABLECOUNT, 100);
			}
			break;
		default:
			SetDlgItemInt(IDC_CELLSCORE, 100);
			SetDlgItemInt(IDC_TABLECOUNT, 100);
			break;
		}
		SetDlgItemInt(IDC_TAXRATE, 50);
	}
}
void CGameKindInfoDlg::UpdateGameBetLimit(WORD wProcessType,DWORD dwCellScore)
{
	if(IsVideoGameProcessType(wProcessType))
	{
		SetDlgItemInt(IDC_HIGHSCORE, 10000);
		SetDlgItemInt(IDC_LESSSCORE, 0);
	}
	else
	{
		switch(wProcessType) 
		{
		case ID_B_HOE:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 400);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 200);
			}
			break;
		case ID_B_LAND:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 80);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 40);
			}
			break;
		case ID_B_UNCOVERPIG:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 200);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 100);
			}
			break;
		case ID_B_BACK_JACK:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 100);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 12);
			}
			break;
		case ID_B_UPGRADE:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 20);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 10);
			}
			break;
		case ID_B_SPARROW:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 40);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 20);
			}
			break;
		case ID_B_SHOWHAND:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 100);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 12);
			}
			break;
		case ID_B_THREELEAF:
			{
				SetDlgItemInt(IDC_HIGHSCORE, dwCellScore * 100);
				SetDlgItemInt(IDC_LESSSCORE, dwCellScore * 12);
			}
			break;
		default:
			SetDlgItemInt(IDC_HIGHSCORE, 10000);
			SetDlgItemInt(IDC_LESSSCORE, 100);
			break;
		}
	}
}
void CGameKindInfoDlg::UpdateGameKindName(WORD wProcessType)
{
	CString strTemp;
	for(int i = 0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(wProcessType == g_GameProcessItem[i].wProcessType)
		{
			strTemp = g_GameProcessItem[i].szTypeName;
			int pos = strTemp.Find(TEXT("-"));
			if(pos != -1)
			{
				strTemp = strTemp.Right(strTemp.GetLength() - pos - 1);
				SetDlgItemText(IDC_KINDNAME, strTemp);
			}
			break;
		}
	}
}
void CGameKindInfoDlg::OnEnChangeCellscore()
{
	UpdateData();
	m_dwGameProcessType = m_comboGameProcessType.GetItemData(m_comboGameProcessType.GetCurSel());
	UpdateGameBetLimit(m_dwGameProcessType,GetDlgItemInt(IDC_CELLSCORE));
}

void CGameKindInfoDlg::OnEnChangeTablecount()
{
	//m_dwAIUserCount = GetDlgItemInt(IDC_TABLECOUNT)/ 3*2;
	//SetDlgItemInt(IDC_AIUSERCOUNT, m_dwAIUserCount);
}
