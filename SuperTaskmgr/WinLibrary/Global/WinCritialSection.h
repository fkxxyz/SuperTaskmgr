
// WinCritialSection.h

#pragma once

class CCriticalSection {
public:
	CCriticalSection(){ InitializeCriticalSection(&m_CriticalSection); }
	~CCriticalSection(){ DeleteCriticalSection(&m_CriticalSection); }
	void Enter(){ EnterCriticalSection(&m_CriticalSection); }
	void Leave(){ LeaveCriticalSection(&m_CriticalSection); }

#if(_WIN32_WINNT >= 0x0400)
	BOOL TryEnter(){ return TryEnterCriticalSection(&m_CriticalSection); }
#endif /* _WIN32_WINNT >= 0x0400 */

private:
	CRITICAL_SECTION m_CriticalSection;
};

class CCriticalSectionFunction {
public:
	CCriticalSectionFunction(CCriticalSection &objCriticalSection):m_objCriticalSection(objCriticalSection){
		m_objCriticalSection.Enter();
	}
	~CCriticalSectionFunction(){
		m_objCriticalSection.Leave();
	}

private:
	CCriticalSection &m_objCriticalSection;
};



