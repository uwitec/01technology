
#pragma once

#ifdef _WIN64
#error Win64 is not supported
#endif

#include <eh.h>
#include "./ComClassDef.h"
#pragma warning(disable:4535)

#define DECLARE_EXCEPTION_CLASS(name) class name : public exception \
	{ \
	 friend class se_translator; \
	protected: \
		name (EXCEPTION_POINTERS * pexp) : exception(pexp) {} \
	};

class COM_SERVICE_CLASS se_translator
{		
 public:
		
	class COM_SERVICE_CLASS exception
	{
		friend class se_translator;
		
	public:	
		unsigned				code() const			{ return m_pexp->ExceptionRecord->ExceptionCode; }
		void *					address() const			{ return m_pexp->ExceptionRecord->ExceptionAddress; }	
		EXCEPTION_POINTERS *	info() const			{ return m_pexp; }	
		const char *			name() const			{ return se_translator::name(code()); }
		const char *			description() const		{ return se_translator::description(code()); }
		
		~exception() {}
		
	 protected:
		 exception(EXCEPTION_POINTERS * pexp) : m_pexp(pexp) { _ASSERTE(pexp != 0); }	
 		 exception(const exception& se) : m_pexp(se.m_pexp) {}

	 private:
		 EXCEPTION_POINTERS * m_pexp;		
	};
	
	class COM_SERVICE_CLASS access_violation : public exception
	{
		friend class se_translator;
	 protected:
		access_violation (EXCEPTION_POINTERS * pexp) : exception(pexp) {}
	 public:
		 bool is_read_op() const { return !info()->ExceptionRecord->ExceptionInformation [0]; }
		 ULONG_PTR inaccessible_address() const { return info()->ExceptionRecord->ExceptionInformation [1]; }
	};

	class COM_SERVICE_CLASS no_memory : public exception
	{
		friend class se_translator;
	 protected:
		no_memory (EXCEPTION_POINTERS * pexp) : exception(pexp) {}
	 public:		 
		 size_t mem_size() const { return info()->ExceptionRecord->ExceptionInformation [0]; }
	};
	
//	DECLARE_EXCEPTION_CLASS(no_memory)	
//	DECLARE_EXCEPTION_CLASS(access_violation)
	DECLARE_EXCEPTION_CLASS(datatype_misalignment)
	DECLARE_EXCEPTION_CLASS(breakpoint)
	DECLARE_EXCEPTION_CLASS(single_step)
	DECLARE_EXCEPTION_CLASS(array_bounds_exceeded)
	DECLARE_EXCEPTION_CLASS(flt_denormal_operand)
	DECLARE_EXCEPTION_CLASS(flt_divide_by_zero)
	DECLARE_EXCEPTION_CLASS(flt_inexact_result)
	DECLARE_EXCEPTION_CLASS(flt_invalid_operation)
	DECLARE_EXCEPTION_CLASS(flt_overflow)
	DECLARE_EXCEPTION_CLASS(flt_stack_check)
	DECLARE_EXCEPTION_CLASS(flt_underflow)
	DECLARE_EXCEPTION_CLASS(int_divide_by_zero)
	DECLARE_EXCEPTION_CLASS(int_overflow)
	DECLARE_EXCEPTION_CLASS(priv_instruction)
	DECLARE_EXCEPTION_CLASS(in_page_error)
	DECLARE_EXCEPTION_CLASS(illegal_instruction)
	DECLARE_EXCEPTION_CLASS(noncontinuable_exception)
	DECLARE_EXCEPTION_CLASS(stack_overflow)
	DECLARE_EXCEPTION_CLASS(invalid_disposition)
	DECLARE_EXCEPTION_CLASS(guard_page)
	DECLARE_EXCEPTION_CLASS(invalid_handle)
	DECLARE_EXCEPTION_CLASS(microsoft_cpp)
 
 public:
	se_translator() : m_fn(_set_se_translator(translator)) {}
	~se_translator() { _set_se_translator(m_fn); }

	static void translator(unsigned, EXCEPTION_POINTERS * pexp);
	static const char *	name(unsigned code);
	static const char *	description(unsigned code);

 private:
	_se_translator_function m_fn;
};



