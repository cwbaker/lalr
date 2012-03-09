#include <sweet/unit/Config.h>
#include <sweet/unit/AssertException.h>
#include <sweet/unit/ReportAssert.h>

namespace UnitTest {

void ReportAssert(char const* description, char const* filename, int lineNumber)
{
    throw AssertException(description, filename, lineNumber);
}

}
