#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Bengi/VM.h"

string testFolder = "C:\\Users\\msi\\Desktop\\VSCode Folder\\Bengi-Lang\\tests\\";
vector<string> test
{	testFolder + "test1.cben",
	testFolder + "test2.cben",
	testFolder + "test3.cben"
};
vector<i32> testResult
{
	40,
	10,
	11
};

namespace BengiTest
{		
	TEST_CLASS(ArithmeticLogic)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			VM testVM;
			
			testVM.loadBinary(test[0]);
			i32 result = testVM.run();
			Assert::AreEqual(testResult[0], result);
		}

	};

	TEST_CLASS(RegisterAddressReferencing)
	{
	public:

		TEST_METHOD(Test1)
		{
			VM testVM;
			testVM.loadBinary(test[1]);
			Assert::AreEqual(testResult[1], testVM.run());
		}

		TEST_METHOD(Test2)
		{
			VM testVM;
			testVM.loadBinary(test[2]);
			Assert::AreEqual(testResult[2], testVM.run());
		}
	};
}