#include "se_translator.h"
#include "exception_trap.h"
#include "unhandled_report.h"
#include "sym_engine.h"
#include "debug_stream.h"
#include "exception2.h"

#include "../../CommonDefinition\BuildDefine.h"

void COM_SERVICE_CLASS InitExceptoinHandle();
bool COM_SERVICE_CLASS GT_DumpSimple(const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR szMsg);
bool COM_SERVICE_CLASS GT_DumpDetail(const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR szFormat,...);
bool COM_SERVICE_CLASS GT_Dump_Access_violation(const TCHAR *pszFileName, int nLineNo, se_translator::access_violation& ex);
bool COM_SERVICE_CLASS GT_Dump_Se_translator(const TCHAR *pszFileName, int nLineNo, se_translator::no_memory& ex);
bool COM_SERVICE_CLASS GT_Dump_Exception2(const TCHAR *pszFileName, int nLineNo, exception2& ex);
bool inline GT_DumpEmpty(const TCHAR* szEmpty,LPTSTR szFormat,...){return true;}
