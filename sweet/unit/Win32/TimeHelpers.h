#ifndef UNITTEST_TIMEHELPERS_H
#define UNITTEST_TIMEHELPERS_H

#include <sweet/unit/Config.h>

#ifdef UNITTEST_MINGW
    #ifndef __int64
        #define __int64 long long
    #endif
#endif

namespace UnitTest {

class UNITTEST_DECLSPEC Timer
{
public:
    Timer();
	void Start();
	int GetTimeInMs() const;    

private:
    __int64 GetTime() const;

    void* m_threadHandle;

#if defined(_WIN64)
    unsigned __int64 m_processAffinityMask;
#else
    unsigned long m_processAffinityMask;
#endif

	__int64 m_startTime;
	__int64 m_frequency;
};


namespace TimeHelpers
{
UNITTEST_DECLSPEC void SleepMs (int ms);
}


}

#endif
