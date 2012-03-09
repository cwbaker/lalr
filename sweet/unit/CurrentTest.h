#ifndef UNITTEST_CURRENTTESTRESULTS_H
#define UNITTEST_CURRENTTESTRESULTS_H

namespace UnitTest {

class TestResults;
class TestDetails;

namespace CurrentTest
{
	UNITTEST_DECLSPEC TestResults*& Results();
	UNITTEST_DECLSPEC const TestDetails*& Details();
}

}

#endif
