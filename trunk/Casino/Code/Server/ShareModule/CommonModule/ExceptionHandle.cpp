#include "stdafx.h"
#include "ExceptionHandle.h"
#include "se_translator.h"
#include "exception_trap.h"
#include "unhandled_report.h"
#include "sym_engine.h"
#include "debug_stream.h"
#include "exception2.h"
#include <fstream>
#include <iosfwd>


bool  GT_DumpToStream(std::ostream& os,const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR pszMsg)
{
	os << TEXT("[Type]") << szType << std::endl;

	SYSTEMTIME sysTm;
	::GetLocalTime(&sysTm);
	os <<  TEXT("[Time]") << sysTm.wMonth << TEXT("-") << sysTm.wDay << TEXT(" ")
		<< sysTm.wHour << TEXT(":") << sysTm.wMinute << TEXT(":") << sysTm.wSecond <<std::endl;

	TCHAR szModule[256] = "";
	GetModuleFileName(NULL,szModule,256);
	os << TEXT("[Module]") << szModule << TEXT(" [File]") << pszFileName << TEXT(" [Line]") << nLineNo << std::endl;
	os << TEXT("[ThreadID]") << GetCurrentThreadId() << std::endl;
	os << TEXT("[Message]") << pszMsg << std::endl;
	
	return true;
}
bool  GT_DumpToFile(const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR pszMsg)
{
	std::ofstream ofs;
	ofs.open(EXCEPTOINHANDLE_FILEPATH,std::ios::app);
	if(ofs.is_open())
	{
		ofs << TEXT("**********************************************************************") <<std::endl;
		GT_DumpToStream(ofs,szType, pszFileName,nLineNo,pszMsg);
		ofs << TEXT("[Trace]") << std::endl;
		sym_engine::stack_trace(ofs);
		ofs <<  TEXT("**********************************************************************") <<std::endl;

		ofs.close();
	}
	return true;
}
bool  GT_DumpSimple(const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR szMsg)
{
	GT_DumpToFile(szType, pszFileName,nLineNo,szMsg);
	return true;
}
bool  GT_DumpDetail(const TCHAR* szType,const TCHAR *pszFileName, int nLineNo, LPTSTR szFormat,...)
{
	TCHAR szBuffer[2048]={0};
	const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	const int LASTCHAR = NUMCHARS - 1;

	va_list pArgs;
	va_start(pArgs, szFormat);

	_vsntprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	va_end(pArgs);

	szBuffer[LASTCHAR] = TEXT('\0');

	GT_DumpToFile(szType, pszFileName,nLineNo,szBuffer);
	return true;
}
bool  GT_Dump_Access_violation(const TCHAR *pszFileName, int nLineNo, se_translator::access_violation& ex)
{
	std::ofstream ofs;
	ofs.open(EXCEPTOINHANDLE_FILEPATH,std::ios::app);
	if(ofs.is_open())
	{
		ofs << TEXT("**********************************************************************") <<std::endl;
		GT_DumpToStream(ofs,"Error", pszFileName,nLineNo,"");
		ofs << TEXT("[Trace]") << std::endl;
		ofs	<< ex.name() << " at 0x" << std::hex << ex.address() 
			<< ", thread attempts to " << (ex.is_read_op() ? "read" : "write") 
			<< " at 0x" << std::right << ex.inaccessible_address() << std::endl
			<< "stack trace : " << std::endl;
		sym_engine::stack_trace(ofs, ex.info()->ContextRecord);		
		ofs <<  TEXT("**********************************************************************") <<std::endl;

		ofs.close();
	}
	return true;
}
bool  GT_Dump_Se_translator(const TCHAR *pszFileName, int nLineNo, se_translator::no_memory& ex)
{
	std::ofstream ofs;
	ofs.open(EXCEPTOINHANDLE_FILEPATH,std::ios::app);
	if(ofs.is_open())
	{
		ofs << TEXT("**********************************************************************") <<std::endl;
		GT_DumpToStream(ofs,"Error", pszFileName,nLineNo,"");
		ofs  << ex.name() << " at 0x" << std::hex << ex.address() 
			<< ", unable to allocate " << std::dec << ex.mem_size() << " bytes" << std::endl
			<< "stack trace : " << std::endl;
		sym_engine::stack_trace(ofs, ex.info()->ContextRecord);		
		ofs <<  TEXT("**********************************************************************") <<std::endl;

		ofs.close();
	}
	return true;
}
bool	GT_Dump_Exception2(const TCHAR *pszFileName, int nLineNo, exception2& ex)
{
	std::ofstream ofs;
	ofs.open(EXCEPTOINHANDLE_FILEPATH,std::ios::app);
	if(ofs.is_open())
	{
		ofs << TEXT("**********************************************************************") <<std::endl;
		GT_DumpToStream(ofs,"Error", pszFileName,nLineNo,"");
		ofs  << ex.what() << std::endl
			<< "stack trace :" << std::endl
			<< ex.stack_trace() << std::endl;		
		ofs <<  TEXT("**********************************************************************") <<std::endl;

		ofs.close();
	}
	return true;
}