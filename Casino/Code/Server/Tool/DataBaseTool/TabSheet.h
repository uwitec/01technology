#pragma once


// CTabSheet
#define MAXPAGE 16

class CTabSheet : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabSheet)

public:
	CTabSheet();
	virtual ~CTabSheet();

	int				GetCurSel();
	int				SetCurSel(int nItem);
	void			Show();
	void			SetRect();
	BOOL			AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);

protected:
	LPCTSTR			m_Title[MAXPAGE];
	UINT			m_IDD[MAXPAGE];
	CDialog*		m_pPages[MAXPAGE];
	int				m_nNumOfPages;
	int				m_nCurrentPage;
		
protected:
	//{{AFX_MSG(CTabSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


