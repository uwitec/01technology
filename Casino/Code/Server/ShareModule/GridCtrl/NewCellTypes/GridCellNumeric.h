// GridCellNumeric.h: interface for the CGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
// Updated by PJ Arends [pja@telus.net] September 2004
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDINTEGERCELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
#define AFX_GRIDINTEGERCELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GridCell.h"

class CGridCellNumeric : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellNumeric)

public:
    // bit flags for the m_dwFlags member and the SetFlags and GetFlags functions
    // The default value of zero sets the cell to handle unsigned integers with
    // an audible error warning.
    enum {
        Integer  = 0x00,  // Integer numbers       \.
        Real     = 0x01,  // Floating point numbers ]- mutually exclusive values
        Currency = 0x02,  // Currency              /
        TypeMask = 0x03,  // Integer | Real | Currency
        Negative = 0x04,  // Allow negative (signed) numbers
        Silent   = 0x08   // Disable audible warning in edit control
    };

    CGridCellNumeric(); // default constructor
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
    virtual void SetText(LPCTSTR szText);

    DWORD SetFlags(DWORD dwFlags);
    DWORD GetFlags() { return m_dwFlags; }

    // the number is stored internally as a double
    BOOL SetNumber(double);
    double GetNumber() { return m_nNumber; }

    void SetNumberFmt(LPNUMBERFMT pFmt) { m_pNumberFmt = pFmt; }
    void SetCurrencyFmt(LPCURRENCYFMT pFmt) { m_pCurrencyFmt = pFmt; }

protected:
    BOOL Format(CString &);

private:
    DWORD m_dwFlags;
    double m_nNumber;
    LPNUMBERFMT m_pNumberFmt;
    LPCURRENCYFMT m_pCurrencyFmt;

};

#endif // !defined(AFX_GRIDINTEGERCELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
