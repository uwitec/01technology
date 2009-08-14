// GridCellNumeric.cpp: implementation of the CGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
// Updated by PJ Arends [pja@telus.net] September 2004
//  - Added the CInPlaceNumEdit class to allow for negative
//    numbers and a decimal point.
//  - Added everything but the Edit and EndEdit functions to the 
//    CGridCellNumeric class
//  - Modified the Edit function so it creates a CInPlaceNumEdit
//    control instead of a CInPlaceEdit control
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellNumeric.h"
#include "../GridCtrl.h"
#include "../inplaceedit.h"

IMPLEMENT_DYNCREATE(CGridCellNumeric, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////
// class CInPlaceNumEdit

class CInPlaceNumEdit : public CInPlaceEdit
{
public:
    CInPlaceNumEdit(CWnd*, CRect&, DWORD, UINT, int, int, CString, UINT, double, DWORD);
protected:
    CString LastValid;  // The last string to pass validation
    BOOL Updating;      // flag to stop an infinite recursive OnUpdate loop
    DWORD Sel;          // The users current text selection
    DWORD Flags;        // Flags

    TCHAR cDecimal;     // current locale's decimal seperator
    CString sDigits;    // current locale's digits
    TCHAR cNegative;    // current locale's negative sign
    int iNegative;      // current locale's negative format

    int GetPositionAndCount(LPCTSTR String, TCHAR ch, int &Count);

    afx_msg void OnUpdate();
    afx_msg void OnTimer(UINT);
    afx_msg void OnKillFocus(CWnd *);
    afx_msg void OnChar(UINT, UINT, UINT);
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit constructor  (public member function)
//    Initializes member variables
//
//  Parameters :
//    pParent      [in] - Not used here. Passed on to base class
//    rect         [in] - Not used here. Passed on to base class
//    dwStyle      [in] - Not used here. Passed on to base class
//    nID          [in] - Not used here. Passed on to base class
//    nRow         [in] - Not used here. Passed on to base class
//    nColumn      [in] - Not used here. Passed on to base class
//    InitialText  [in] - Not used here. Passed on to base class
//    nFirstChar   [in] - Not used here. Passed on to base class
//    InitialValue [in] - The number to display in the edit control
//    dwFlags      [in] - Flags
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

CInPlaceNumEdit::CInPlaceNumEdit(CWnd* pParent,
                                 CRect& rect,
                                 DWORD dwStyle,
                                 UINT nID,
                                 int nRow,
                                 int nColumn,
                                 CString InitialText,
                                 UINT nFirstChar,
                                 double InitialValue,
                                 DWORD dwFlags) :
CInPlaceEdit(pParent, rect, dwStyle, nID, nRow, nColumn, InitialText, nFirstChar),
LastValid(_T("0")),
Updating(0),
Sel(0),
cDecimal(0),
cNegative(0),
iNegative(-1),
Flags(dwFlags)
{
    int count = 0;
    CString buffer;
    // Get the locale's native digits
    count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                          LOCALE_SNATIVEDIGITS,
                          NULL,
                          0);
    count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                          LOCALE_SNATIVEDIGITS,
                          sDigits.GetBuffer(count),
                          count);
    sDigits.ReleaseBuffer();
    if (count == 0)
        sDigits = _T("0123456789");

    // Get the locale's decimal point
    if (Flags & CGridCellNumeric::Currency)
    {
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SMONDECIMALSEP,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SMONDECIMALSEP,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cDecimal = count ? buffer[0] : _T('.');
        sDigits += cDecimal;
    }
    else if (Flags & CGridCellNumeric::Real)
    {
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SDECIMAL,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SDECIMAL,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cDecimal = count ? buffer[0] : _T('.');
        sDigits += cDecimal;
    }

    if (Flags & CGridCellNumeric::Negative)
    {
        // Get the locale's negative sign
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SNEGATIVESIGN,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SNEGATIVESIGN,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cNegative = count ? buffer[0] : _T('-');
        sDigits += cNegative;

        // Get the locale's negative format
        if (Flags & CGridCellNumeric::Currency)
            count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                                  LOCALE_INEGCURR | LOCALE_RETURN_NUMBER,
                                  (LPTSTR)&iNegative,
                                  sizeof(int) / sizeof(TCHAR));
        else
            count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                                  LOCALE_INEGNUMBER | LOCALE_RETURN_NUMBER,
                                  (LPTSTR)&iNegative,
                                  sizeof(int) / sizeof(TCHAR));
        if (!count)
            iNegative = 1;

        // Add valid symbols to sDigits
        if (Flags & CGridCellNumeric::Currency)
        {
            if (iNegative == 0 || iNegative == 4 || iNegative == 14 || iNegative == 15)
                sDigits += _T("()");
        }
        else if (iNegative == 0)
            sDigits += _T("()");
    }

    int LeadingZero;
    count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                          LOCALE_ILZERO | LOCALE_RETURN_NUMBER,
                          (LPTSTR)&LeadingZero,
                          sizeof(int) / sizeof(TCHAR));
    if (!count)
        LeadingZero = 1;

    // build our initial editable string
    CString sInitText;
    sInitText.Format(_T("%.9f"), InitialValue);

    // remove trailing zeroes
    int pos = sInitText.GetLength() - 1;
    while (sInitText[pos] == _T('0'))
        sInitText.Delete(pos--);

    // remove trailing decimal point
    if (sInitText[pos] == _T('.'))
        sInitText.Delete(pos);

    // remove leading minus sign
    sInitText.Remove(_T('-'));

    // remove leading zero, unless the string is only a zero
    if (!LeadingZero && sInitText.GetLength() > 1 && sInitText[0] == _T('0'))
        sInitText.Delete(0);

    // replace the decimal point with the locale one
    sInitText.Replace(_T('.'), cDecimal);

    // format negative numbers for the local
    if (InitialValue < 0.0)
    {
        if (iNegative == 0)
        {
            // use surrounding brackets
            sInitText.Insert(0, _T('('));
            sInitText += _T(')');
        }
        else if (iNegative < 3)
        {
            // use leading locale negative sign
            sInitText.Insert(0, cNegative);
        }
        else
        {
            // use trailing locale negative sign
            sInitText += cNegative;
        }
    }

    // Replace the digits with the locale specific ones
    LPSTR Iptr = sInitText.GetBuffer(sInitText.GetLength() + 1);
    LPCTSTR Dptr = sDigits;
    while (*Iptr)
    {
        if (*Iptr >= 0x30 && *Iptr <= 0x39)
            *Iptr = Dptr[*Iptr - 0x30];
        Iptr = CharNext(Iptr);
    }
    sInitText.ReleaseBuffer();
    
    // resize the window so that the string fits.
    // first, get the required size
    CWindowDC DC(this);
    CFont *pFont = (CFont *)DC.SelectObject(GetFont());
    CSize size = DC.GetTextExtent(sInitText + _T("      "));
    DC.SelectObject(pFont);

    // Get the grid's client rect
    CRect ParentRect;
    GetParent()->GetClientRect( &ParentRect );

    //////////////////////////////////////////////////////////////////
    //                                                              //
    //                     Note: C2248 error                        //
    //                     =================                        //
    //                                                              //
    //   If you get compiler error C2248 in this piece of code,     //
    // it is because you have to make the m_Rect member of the      //
    // CInPlaceEdit base class a protected member. It is currently  //
    // an inaccessible private member.                              //
    //                                                              //
    //   A possibly better option would be to add Get/Set functions //
    // for m_Rect, but this was easier.                             //
    //                                                              //
    //////////////////////////////////////////////////////////////////

    // Check whether control needs to be resized
    // and whether there is space to grow
    if (size.cx > m_Rect.Width())
    {
        if( size.cx + m_Rect.left < ParentRect.right )
            m_Rect.right = m_Rect.left + size.cx;
        else
            m_Rect.right = ParentRect.right;
        MoveWindow( &m_Rect );
    }

    // Update the edit box text with the valid initial string
    SetWindowText(sInitText);
    SetSel(0, -1);

    // Set the timer to keep track of user's selection
    SetTimer(1, 50, NULL);
}

BEGIN_MESSAGE_MAP(CInPlaceNumEdit, CInPlaceEdit)
    ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
    ON_WM_TIMER()
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit::OnTimer  (protected member function)
//    Handles the WM_TIMER message
//    Get current text selection. We need this so we
//    can reset it if an invalid text insertion is made
//
//  Parameters :
//    None
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

void CInPlaceNumEdit::OnTimer(UINT)
{
    Sel = GetSel();
}

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit::OnChar  (protected member function)
//    Handles the WM_CHAR message
//    Catch the VK_ESCAPE character. Set Updating to TRUE
//    so we do not try to validate the text when the user
//    aborts the editing
//
//  Parameters :
//    nChar   [in] - The character being entered
//    nRepCnt [in] - Not used here. Passed on to base class
//    nFlags  [in] - Not used here. Passed on to base class
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

void CInPlaceNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_ESCAPE)
        Updating = TRUE;

    CInPlaceEdit::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit::GetPositionAndCount  (protected member function)
//    Get the first position, and number of occurances, of a character
//    in a string
//
//  Parameters :
//    String [in]  - The string to search
//    ch     [in]  - The character to search for
//    Count  [out] - The number of times the character was found
//
//  Returns :
//    The zero based position of the first occurance of the character
//    -1 if not found
//
/////////////////////////////////////////////////////////////////////////////

int CInPlaceNumEdit::GetPositionAndCount(LPCTSTR String, TCHAR ch, int &Count)
{
    int Position = -1;
    Count = 0;

    while (*String)
    {
        if (!Count)
            ++Position;
        if (*String == ch)
            ++Count;
        String = CharNext(String);
    }

    return Count ? Position : -1;
}

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit::OnUpdate  (protected member function)
//    Handles the reflected EN_UPDATE message
//    This is where we do all our validating. If an invalid
//    entry is made, we reset the last valid text and selection
//
//  Parameters :
//    None
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

void CInPlaceNumEdit::OnUpdate()
{
    if (!Updating)
    {
        BOOL Ok = FALSE;
        CString str;
        GetWindowText(str);
        CString Temp = str.SpanIncluding(sDigits);
        if (Temp == str)
        {
            // No invalid characters
            Ok = TRUE;
            int Pos = -1;
            int Count = 0;
            if (Flags & CGridCellNumeric::Negative)
            {
                // Negative numbers allowed
                if (iNegative == 0)
                {
                    // Number can use brackets for negative
                    Pos = GetPositionAndCount(str, _T('('), Count);

                    // Bracket, if present, must be first character
                    // Can only have one bracket
                    if (Pos > 0 || Count > 1)
                        Ok = FALSE;

                    // Can not have bracket and negative sign together
                    if (Count && str.Find(cNegative, 0) > -1)
                        Ok = FALSE;

                    if (Ok)
                    {
                        Pos = GetPositionAndCount(str, _T(')'), Count);

                        // Can only have one closing bracket, and it must be last
                        if (Count > 1 && Pos != str.GetLength() - 1)
                            Ok = FALSE;

                        // Can not have bracket and negative sign together
                        if (Count && str.Find(cNegative, 0) > -1)
                            Ok = FALSE;
                    }
                }

                if (Ok && iNegative < 3)
                {
                    // Leading negative sign must be first
                    // And yes, we do allow the user to use a leading
                    // negative sign even when the locale specifies that
                    // negatives are shown with brackets.
                    Pos = GetPositionAndCount(str, cNegative, Count);
                    if (Pos > 0 || Count > 1)
                        Ok = FALSE;
                }

                if (Ok && iNegative > 2)
                {
                    // Trailing negative sign must be last
                    Pos = GetPositionAndCount(str, cNegative, Count);
                    if (Count && Pos != str.GetLength() - 1)
                        Ok = FALSE;
                }
            }

            if (Ok && Flags & (CGridCellNumeric::Real | CGridCellNumeric::Currency))
            {
                // Can only have one decimal seperator sign
                GetPositionAndCount(str, cDecimal, Count);
                if (Count > 1)
                    Ok = FALSE;
            }
        }

        if (Ok)
            // save the valid string
            LastValid = str;
        else
        {
            Updating = TRUE;

            // audible warning
            if (!(Flags & CGridCellNumeric::Silent))
                MessageBeep(MB_OK);

            // reset the last valid string
            SetWindowText(LastValid);

            // reset the text selection. This has only one small problem:
            // If the original selection had the caret on the left hand
            // side, it will now be placed on the right hand side.
            SetSel(Sel);

            Updating = FALSE;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
//  CInPlaceNumEdit::OnKillFocus  (protected member function)
//    Handles the WM_KILLFOCUS message
//    If the edit control is empty, inserts a zero
//
//  Parameters :
//    pNewWnd [in] - Not used here. Passed on to the base class
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

void CInPlaceNumEdit::OnKillFocus(CWnd *pNewWnd)
{
    CString str;
    GetWindowText(str);
    
    if (str.IsEmpty())
    {
        str = sDigits[0];
        Updating = TRUE;
        SetWindowText(str);
    }

    CInPlaceEdit::OnKillFocus(pNewWnd);
}

/////////////////////////////////////////////////////////////////////////////
// class CGridCellNumeric

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  CGridCellNumeric constructor  (public member function)
//    Initializes member variables
//
//  Parameters :
//    None
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

CGridCellNumeric::CGridCellNumeric() :
m_pNumberFmt(0),
m_pCurrencyFmt(0),
m_dwFlags(0),
m_nNumber(0.0)
{
}

// Create a control to do the editing
BOOL CGridCellNumeric::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = TRUE;
    
    // CInPlaceEdit auto-deletes itself
//	m_pEditWnd = new CInPlaceEdit(GetGrid(), rect, /*GetStyle() |*/ ES_NUMBER, nID, nRow, nCol,
//		GetText(), nChar);

    // pja - Sept 12, 2004
    m_pEditWnd = new CInPlaceNumEdit(GetGrid(),
                                     rect,
                                     0,
                                     nID,
                                     nRow,
                                     nCol,
                                     GetText(),
                                     nChar,
                                     m_nNumber,
                                     m_dwFlags);

    return TRUE;
}

// Cancel the editing.
void CGridCellNumeric::EndEdit()
{
    if (m_pEditWnd)
        ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}

/////////////////////////////////////////////////////////////////////////////
//
//  CGridCellNumeric::SetText  (public virtual member function)
//    Called whenever the cell's text has to be set
//
//  Parameters :
//    szText [in] - The text to be set
//
//  Returns :
//    Nothing
//
/////////////////////////////////////////////////////////////////////////////

void CGridCellNumeric::SetText(LPCTSTR szText)
{
    CString str = szText;
    if (Format(str))
        CGridCell::SetText(str);
}

/////////////////////////////////////////////////////////////////////////////
//
//  CGridCellNumeric::SetFlags  (public member function)
//    Sets the bit flags that control the type of number and edit control
//    Valid bit flags are:
//             CGridCellNumeric::Integer
//             CGridCellNumeric::Real
//             CGridCellNumeric::Currency
//             CGridCellNumeric::Negative
//             CGridCellNumeric::Silent
//
//  Parameters :
//    dwFlags [in] - The flags to set
//
//  Returns :
//    The previous flags, -1 if an error occured
//
/////////////////////////////////////////////////////////////////////////////

DWORD CGridCellNumeric::SetFlags(DWORD dwFlags)
{
    // Integer, Real, and Currency are mutually exclusive flags,
    // But one of them has to set
    if ((dwFlags & TypeMask) != Integer &&
        (dwFlags & TypeMask) != Real &&
        (dwFlags & TypeMask) != Currency)
    {
        ASSERT(0);
        return (DWORD)-1;
    }
    DWORD old = m_dwFlags;
    m_dwFlags = dwFlags;
    return old;
}

/////////////////////////////////////////////////////////////////////////////
//
//  CGridCellNumeric::SetNumber  (public member function)
//    Call this function to set the number in the cell
//
//  Parameters :
//    Number [in] - the new number value
//
//  Returns :
//    TRUE on success, FALSE on failure
//
//  Note:
//    The m_nNumber variable is updated via the SetText call
//    so it does not get out of sync (hopefully) if an error occurs.
//
/////////////////////////////////////////////////////////////////////////////

BOOL CGridCellNumeric::SetNumber(double Number)
{
    CString str;
    str.Format(_T("%.9f"), Number);
    SetText(str);

    m_nState |= GVIS_MODIFIED;

    // check if the number passed in and the number saved are within 9
    // decimal points accuracy of each other. If they are, consider the
    // function successful.
    return __max(Number, m_nNumber) - __min(Number, m_nNumber) < 1e-9;
}

/////////////////////////////////////////////////////////////////////////////
//
//  CGridCellNumeric::Format  (protected member function)
//    Converts the given number string to a format specified by the
//    computer's locale settings. To override the locale settings, use the
//    SetNumberFmt or SetCurrencyFmt functions to change the defaults.
//
//    See GetCurrencyFormat and GetNumberFormat in MSDN for more info.
//
//  Parameters :
//    TheString [in/out] - The string to be converted to locale specific format
//
//  Returns :
//    Nothing
//
//  Note :
//    Has a side effect of setting the value of the m_nNumber variable
//
/////////////////////////////////////////////////////////////////////////////

BOOL CGridCellNumeric::Format(CString &TheString)
{
    int count = 0;
    CString str = TheString;
    CString buffer;
    CString sDigits;
    TCHAR cDecimal = 0;
    TCHAR cNegative = 0;
    int iNegative = -1;

    // Get the locale's native digits
    count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                          LOCALE_SNATIVEDIGITS,
                          NULL,
                          0);
    count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                          LOCALE_SNATIVEDIGITS,
                          sDigits.GetBuffer(count),
                          count);
    sDigits.ReleaseBuffer();
    if (!count)
        sDigits = _T("0123456789");

    // Get the locale's decimal point
    if (m_dwFlags & Currency)
    {
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SMONDECIMALSEP,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SMONDECIMALSEP,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cDecimal = count ? buffer[0] : _T('.');
        sDigits += cDecimal;
    }
    else
    {
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SDECIMAL,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SDECIMAL,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cDecimal = count ? buffer[0] : _T('.');
    }

    if (m_dwFlags & Negative)
    {
        // Get the locale's negative sign
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SNEGATIVESIGN,
                              NULL,
                              0);
        count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                              LOCALE_SNEGATIVESIGN,
                              buffer.GetBuffer(count),
                              count);
        buffer.ReleaseBuffer();
        cNegative = count ? buffer[0] : _T('-');
        sDigits += cNegative;

        // Get the locale's negative format
        if (m_dwFlags & Currency)
            count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                                  LOCALE_INEGCURR | LOCALE_RETURN_NUMBER,
                                  (LPTSTR)&iNegative,
                                  sizeof(int) / sizeof(TCHAR));
        else
            count = GetLocaleInfo(LOCALE_USER_DEFAULT,
                                  LOCALE_INEGNUMBER | LOCALE_RETURN_NUMBER,
                                  (LPTSTR)&iNegative,
                                  sizeof(int) / sizeof(TCHAR));
        if (!count)
            iNegative = 1;


        if (m_dwFlags & Currency)
        {
            if (iNegative == 0 || iNegative == 4 || iNegative == 14 || iNegative == 15)
                sDigits += _T("()");
        }
        else if (iNegative == 0)
            sDigits += _T("()");
    }

    // We now have all the locale specific settings we need
    // Now we can tackle the string
    if (m_dwFlags & (Real | Currency))
    {
        // replace locale decimal seperator with a decimal point
        str.Replace(cDecimal, _T('.'));

        // add the decimal point to the allowable characters
        sDigits += _T('.');
    }

    if ((m_dwFlags & TypeMask) == Integer)
    {
        // we are dealing with integers, so we will find the decimal point
        // and chop it off. No rounding will occur.
        int pos = str.Find(_T('.'), 0);
        if (pos != -1)
            // chop chop
            str.Delete(pos, str.GetLength() - pos);
    }

    LPCTSTR digits = sDigits;
    for(int i = str.GetLength() - 1; i >= 0; --i)
    {
        // Remove invalid characters such as currency signs
        TCHAR *ptr = _tcschr(digits, str[i]);
        if (ptr == NULL)
            str.Delete(i);
        else
        {
            // No idea if this is really necessary, but this is
            // supposed to convert from native digits to ASCII 0-9
            // that is required for GetNumberFormat and GetCurrencyFormat
            // Maybe there is a windows API that already does this???
            int diff = (int)(ptr - digits);
            if (diff < 10 && (*ptr < _T('0') || *ptr > _T('9')))
            {
                TCHAR ch = (TCHAR)(0x30 + diff);
                str.SetAt(i, ch);                
            }
        }
    }

    if (m_dwFlags & Negative)
    {
        // remove locale negative formatting and replace
        // with leading minus sign
        CString neg = (iNegative ? _T("") : _T("()"));
        neg += cNegative;
        if (str.FindOneOf(neg) != -1)
        {
            str.Remove(cNegative);
            str.Remove(_T('('));
            str.Remove(_T(')'));
            str.Insert(0, _T('-'));
        }
    }

    // The string should now be in the correct format
    // for the locale formatting functions.
    // Here we set a local Number variable, and at
    // same time confirm that the formatting will work.
    // m_nNumber will be updated later to this Number if
    // everything succeeds. Don't want to get out of sync do we?
    TCHAR *EndPtr = NULL;
    double Number = _tcstod(str, &EndPtr);
//    ASSERT (*EndPtr == NULL);

    if (*EndPtr == NULL)
    {
        CString Formatted;
        if (m_dwFlags & Currency)
        {
            count = GetCurrencyFormat(LOCALE_USER_DEFAULT,
                                      NULL,
                                      str,
                                      m_pCurrencyFmt,
                                      NULL,
                                      0);
            count = GetCurrencyFormat(LOCALE_USER_DEFAULT,
                                      NULL,
                                      str,
                                      m_pCurrencyFmt,
                                      Formatted.GetBuffer(count),
                                      count);
            Formatted.ReleaseBuffer();
        }
        else
        {
            count = GetNumberFormat(LOCALE_USER_DEFAULT,
                                    NULL,
                                    str,
                                    m_pNumberFmt,
                                    NULL,
                                    0);
            count = GetNumberFormat(LOCALE_USER_DEFAULT,
                                    NULL,
                                    str,
                                    m_pNumberFmt,
                                    Formatted.GetBuffer(count),
                                    count);
            Formatted.ReleaseBuffer();
        }

        if (count)
        {
            if ((m_dwFlags & TypeMask) == Integer)
            {
                // remove the decimal point and any trailing zeroes
                // we cannot simply chop it off as the formatting
                // may have left trailing sign characters
                int pos = Formatted.Find(cDecimal, 0);
                Formatted.Delete(pos);
                while (pos < Formatted.GetLength() && Formatted[pos] == sDigits[0])
                    Formatted.Delete(pos);
            }

            // Only update m_nNumber if everything above succeeded
            m_nNumber = Number;
            TheString = Formatted;
            return TRUE;
        }
    }
    TRACE (_T("CGridCellNumeric::Format() - Failed to correctly format string."));
//    ASSERT(0);
    return FALSE;
}
