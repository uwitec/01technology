#pragma	  once

#include "./BuildDefine.h"

#ifdef __GLOBALTRACE__

#include <afx.h>
#include <afxwin.h> 
#include "../ShareModule/CommonModule/CommonModule.h"

#ifdef _DEBUG
#define GT_ASSERT(f)	{(void) ((f) || GT_DumpSimple("ASSERT",__FILE__,__LINE__,""));ASSERT(f);}
#define GT_TRACE(f)		{GT_DumpSimple("TRACE",__FILE__,__LINE__,f);TRACE(f);}
#define GT_WARN(f)		{GT_DumpSimple("WARN",__FILE__,__LINE__,f);TRACE(f);}
#define GT_ERROR(f)		{GT_DumpSimple("ERROR",__FILE__,__LINE__,f);TRACE(f);}
#else
#define GT_ASSERT(f)	(void) ((f) || GT_DumpSimple("ASSERT",__FILE__,__LINE__,""))
#define GT_TRACE(f)		GT_DumpSimple("TRACE",__FILE__,__LINE__,f)
#define GT_WARN(f)		GT_DumpSimple("WARN",__FILE__,__LINE__,f)
#define GT_ERROR(f)		GT_DumpSimple("ERROR",__FILE__,__LINE__,f)
#endif

#define GT_INFOBASIC	"INFO",__FILE__,__LINE__
#define GT_INFO			GT_DumpDetail

#define _BEGIN_TRY					{se_translator translator;	try
#define CATCH_COMMON_EXCEPTION(s)	 catch(se_translator::access_violation& ex){GT_Dump_Access_violation(__FILE__,__LINE__,ex);s;}catch(se_translator::no_memory& ex){GT_Dump_Se_translator(__FILE__,__LINE__,ex);s;}catch(exception2& ex){GT_Dump_Exception2(__FILE__,__LINE__,ex);s;}
#define CATCH_UNKNOWN_EXCEPTION(s)	 catch(...){GT_ERROR("Unknow Exception");s;}
#define _END_CATCH					}

#else

#define GT_ASSERT(f)	((void)0)
#define GT_TRACE(f)		((void)0)
#define GT_WARN(f)		((void)0)
#define GT_ERROR(f)		((void)0)
#define GT_INFOBASIC	""
#define GT_INFO			GT_DumpEmpty

#define _BEGIN_TRY					try
#define CATCH_COMMON_EXCEPTION(s) 
#define CATCH_UNKNOWN_EXCEPTION(s)	catch(...){s;}
#define _END_CATCH					

#endif