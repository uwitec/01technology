
#pragma once

#include <sstream>
#include <string>
#include <exception>
#include "sym_engine.h"
#include "./ComClassDef.h"

namespace
{
	// the mixin class for exception
	struct call_stack
	{	
		call_stack (unsigned skip = 3)
			: m_s((skip == unsigned(-1)) ? "" : call_stack::get(skip))		
		{}
		static std::string get(unsigned skip = 2)
		{
			std::stringstream ss(std::ios::out);		
			sym_engine::stack_trace(ss, skip);			
			return ss.str();		
		}
		const char * stack_trace() const { return m_s.c_str(); }
	 private:
		std::string m_s;
	};
}

#pragma inline_depth(0) // must disable inlining due to using skip value
class COM_SERVICE_CLASS exception2 : public std::exception, public call_stack 
{
  public:
	  exception2(const __exString& s, unsigned skip = 4 /* -1 for disable call stack*/)
		  : std::exception(s), call_stack(skip)		
	  {}
};

/*
class bug_check : public exception2 
{
 public:
	  bug_check() : exception2("bug check", 5) {}
};
*/

#pragma inline_depth() // restore back the default value


