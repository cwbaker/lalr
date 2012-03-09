#include <sweet/unit/Config.h>
#include <sweet/unit/Test.h>
#include <sweet/unit/TestList.h>
#include <sweet/unit/TestResults.h>
#include <sweet/unit/AssertException.h>
#include <sweet/unit/MemoryOutStream.h>
#include <sweet/unit/ExecuteTest.h>

#ifdef UNITTEST_POSIX
    #include <sweet/unit/Posix/SignalTranslator.h>
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
