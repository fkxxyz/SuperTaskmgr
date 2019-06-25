
// WinDebug.h
#pragma once

#ifdef _DEBUG

#pragma warning(disable: 4793)

template <int len>
inline void VDebugPrintA(LPCSTR _Format, va_list _ArgList){
	CHAR s[len];
	vsprintf_s<len>(s,_Format,_ArgList);
	OutputDebugStringA(s);
}

template <int len>
inline void DebugPrintA(LPCSTR _Format, ...){
	va_list ptr;
	va_start(ptr,_Format);

	VDebugPrintA<len>(_Format,ptr);

	va_end(ptr);
}

inline void DebugPrintA(LPCSTR _Format, ...){
	va_list ptr;
	va_start(ptr,_Format);

	VDebugPrintA<1024>(_Format,ptr);

	va_end(ptr);
}



template <int len>
inline void VDebugPrintW(LPCWSTR _Format, va_list _ArgList){
	WCHAR s[len];
	vswprintf_s<len>(s,_Format,_ArgList);
	OutputDebugStringW(s);
}

template <int len>
inline void DebugPrintW(LPCWSTR _Format, ...){
	va_list ptr;
	va_start(ptr,_Format);

	VDebugPrintW<len>(_Format,ptr);

	va_end(ptr);
}


inline void DebugPrintW(LPCWSTR _Format, ...){
	va_list ptr;
	va_start(ptr,_Format);

	VDebugPrintW<1024>(_Format,ptr);

	va_end(ptr);
}

#pragma warning (default: 4793)


#ifndef ASSERT
#define ASSERT assert
#endif

#ifndef VERIFY
#define VERIFY(f) ASSERT(f)
#endif

#ifdef TRACE
#undef TRACE
#define TRACE DebugPrint
#endif


#else   // _DEBUG

#ifdef TRACE
#undef TRACE
#define TRACE __noop
#endif

#define DebugPrintA __noop
#define DebugPrintW __noop

#ifndef ASSERT
#define ASSERT __noop
#endif

#ifndef VERIFY
#define VERIFY(f) f
#endif

#endif // !_DEBUG




#ifdef UNICODE
#define DebugPrint DebugPrintW
#else
#define DebugPrint DebugPrintA
#endif


