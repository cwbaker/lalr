#include <sweet/unit/Config.h>
#include <sweet/unit/TestReporterStdout.h>
#include <cstdio>
#include <sweet/unit/TestDetails.h>

#if defined(BUILD_OS_WINDOWS)
#include <windows.h>
#endif

namespace UnitTest {

void TestReporterStdout::ReportFailure(TestDetails const& details, char const* failure)
{
#if defined(__APPLE__) || defined(__GNUG__)
    char const* const errorFormat = "%s:%d: error: Failure in %s: %s\n";
#else
    char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";
#endif

	using namespace std;
    printf(errorFormat, details.filename, details.lineNumber, details.testName, failure);

#if defined(BUILD_OS_WINDOWS)
    char buffer [1024];
    _snprintf(buffer, sizeof(buffer), errorFormat, details.filename, details.lineNumber, details.testName, failure);
    buffer [sizeof(buffer) - 1] = '\0';
    OutputDebugStringA( buffer );
#endif
}

void TestReporterStdout::ReportTestStart(TestDetails const& /*test*/)
{
}

void TestReporterStdout::ReportTestFinish(TestDetails const& /*test*/, float)
{
}

void TestReporterStdout::ReportSummary(int const totalTestCount, int const failedTestCount,
                                       int const failureCount, float secondsElapsed)
{
	using namespace std;

    if (failureCount > 0)
        printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
    else
        printf("Success: %d tests passed.\n", totalTestCount);

    printf("Test time: %.2f seconds.\n", secondsElapsed);
}

}
