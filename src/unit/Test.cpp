#include <unit/Config.h>
#include <unit/Test.h>
#include <unit/TestList.h>
#include <unit/TestResults.h>
#include <unit/AssertException.h>
#include <unit/MemoryOutStream.h>
#include <unit/ExecuteTest.h>

#ifdef UNITTEST_POSIX
    #include <unit/Posix/SignalTranslator.h>
#endif

namespace UnitTest {

TestList& Test::GetTestList()
{
    static TestList s_list;
    return s_list;
}

Test::Test(char const* testName, char const* suiteName, char const* filename, int lineNumber)
    : m_details(testName, suiteName, filename, lineNumber)
    , next(0)
    , m_timeConstraintExempt(false)
{
}

Test::~Test()
{
}

void Test::Run()
{
	ExecuteTest(*this, m_details);
}

void Test::RunImpl() const
{
}

}
