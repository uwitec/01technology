// GridCellRadio.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
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
// History:
// 23 Jul 2001 - Complete rewrite
// 13 Mar 2004 - GetCellExtent and GetRadioPlacement fixed by Yogurt
//             - Read-only now honoured - Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../GridCell.h"
#include "../GridCtrl.h"

#include "GridCellRadio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellRadio, CGridCell)

CGridCellRadio::CGridCellRadio() : CGridCell()
{
    m_bRadioed = FALSE;
    //m_Rect.IsRectNull();
}

CSize CGridCellRadio::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the Radiobox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellRadio::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;
    }
    return bResult;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellRadio::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    CRect RadioRect = GetRadioPlacement();
    rect.left = RadioRect.right;

    // enough room to draw?
    // if (RadioRect.Width() < rect.Width() && RadioRect.Height() < rect.Height()) {

    // Do the draw 
    pDC->DrawFrameControl(GetRadioPlacement(), DFC_BUTTON, 
	    (m_bRadioed)? DFCS_BUTTONRADIO | DFCS_CHECKED : DFCS_BUTTONRADIO);

    // }
#endif
    return bResult;
}

void CGridCellRadio::OnClick(CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	/*PointCellRelative += m_Rect.TopLeft();

    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);	
    if (!GetGrid()->IsCellEditable(cell))		
        return;

	// GetRadioPlacement returns the Radiobox dimensions in client coords. Only Radio/
	// unRadio if the user clicked in the box
	if (GetRadioPlacement().PtInRect(PointCellRelative))*/
	{
	//	m_bRadioed = !m_bRadioed;
		GetGrid()->InvalidateRect(m_Rect);
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellRadio::SetRadio(BOOL bRadioed /*=TRUE*/)
{
	BOOL bTemp = m_bRadioed;
	m_bRadioed = bRadioed;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bTemp;
}

BOOL CGridCellRadio::GetRadio()
{
	return m_bRadioed;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the Radiobox in client coords.
CRect CGridCellRadio::GetRadioPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect + CSize(GetMargin(), GetMargin());
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	/* for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    */

    if (m_Rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = m_Rect.top + (m_Rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}