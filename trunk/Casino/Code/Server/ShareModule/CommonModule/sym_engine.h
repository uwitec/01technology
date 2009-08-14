
#pragma once

#ifdef _WIN64
#error Win64 is not supported
#endif

#include <imagehlp.h>
#include <ostream>
#include "./ComClassDef.h"

class COM_SERVICE_CLASS sym_engine
{
 public:
	sym_engine (unsigned);
	~sym_engine();

	void address(unsigned a)		{ m_address = a; }
	unsigned address(void) const	{ return m_address; }
	
	// symbol handler queries
	unsigned module  (char *, unsigned);
	unsigned symbol  (char *, unsigned, unsigned * = 0);
	unsigned fileline(char *, unsigned, unsigned *, unsigned * = 0);

	// stack walk
	bool stack_first (CONTEXT* pctx);
	bool stack_next  ();

	/*
		format argument
		%m  : module
		%f  : file
		%l  : line
		%ld : line's displacement
		%s  : symbol
		%sd : symbol's displacement
		
		for example	
		"%f(%l) : %m at %s\n"
		"%m.%s + %sd bytes, in %f:line %l\n"
	*/	
	static bool stack_trace(std::ostream&, CONTEXT *, unsigned skip = 0, const char * fmt = default_fmt());
	static bool stack_trace(std::ostream&, unsigned skip = 1, const char * fmt = default_fmt()); 	
	static bool stack_trace(std::ostream&, sym_engine&, CONTEXT *,
			unsigned skip = 1, const char * fmt = default_fmt());	
 private:
	static const char * default_fmt() { return "%f(%l) : %m at %s\n"; }	
	static bool get_line_from_addr (HANDLE, unsigned, unsigned *, IMAGEHLP_LINE *);
	static unsigned get_module_basename (HMODULE, char *, unsigned);
	
	bool check();

 private:
	unsigned		m_address;
	bool			m_ok;
	STACKFRAME *	m_pframe;
	CONTEXT *		m_pctx;
	
 private:
	class guard
	{	
	 private:
		guard();
	 public:
		~guard();
		bool init();		
		bool fail() const { return m_ref == -1; }
		static guard & instance() 
		{
			static guard g; 
			return g;
		}
	private:		
		void clear();
		bool load_module(HANDLE, HMODULE);
		int  m_ref;
	};
};
