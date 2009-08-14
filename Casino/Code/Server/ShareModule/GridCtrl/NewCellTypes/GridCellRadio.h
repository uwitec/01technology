#if !defined(AFX_GRIDCELLRadio_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_GRIDCELLRadio_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GridCellRadio.h : header file
//
// MFC Grid Control - Grid combo cell class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////


#include "../GridCell.h"


class CGridCellRadio : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellRadio)

public:
    CGridCellRadio();

public:
	BOOL SetRadio(BOOL bRadioed = TRUE);
	BOOL GetRadio();

// Operations
	virtual CSize GetCellExtent(CDC* pDC);
    virtual void OnClick( CPoint PointCellRelative);
    virtual BOOL GetTextRect( LPRECT pRect);

protected:
	CRect GetRadioPlacement();

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

protected:
    BOOL  m_bRadioed;
    CRect m_Rect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLRadio_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
