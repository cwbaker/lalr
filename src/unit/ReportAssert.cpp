#include <unit/Config.h>
#include <unit/AssertException.h>
#include <unit/ReportAssert.h>

namespace UnitTest {

void ReportAssert(char const* description, char const* filename, int lineNumber)
{
    throw AssertException(description, filename, lineNumber);
}

}
