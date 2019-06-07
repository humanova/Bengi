// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Bengi/Bengi/VM.h"

string testFolder = "C:\\Users\\msi\\Desktop\\VSCode Folder\\Bengi-Lang\\tests\\";
vector<string> test
{ testFolder + "test1.cben",
	testFolder + "test2.cben",
	testFolder + "test3.cben",
	testFolder + "test4.cben",
	testFolder + "test5.cben",
	testFolder + "test6.cben",
	testFolder + "test7.cben",
	
};
vector<i32> testResult
{
	40,
	60,
	11,
	46368,
	100,
	10,
	46656
};

namespace BengiTest
{		
	TEST_CLASS(ArithmeticLogic)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			VM testVM;
			
			testVM.LoadBinary(test[0]);
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
			testVM.LoadBinary(test[1]);
			Assert::AreEqual(testResult[1], testVM.run());
		}

		TEST_METHOD(Test2)
		{
			VM testVM;
			testVM.LoadBinary(test[2]);
			Assert::AreEqual(testResult[2], testVM.run());
		}
	};

	TEST_CLASS(ArithmeticConditional)
	{
	public:

		TEST_METHOD(Test1)
		{
			VM testVM;
			testVM.LoadBinary(test[3]);
			Assert::AreEqual(testResult[3], testVM.run());
		}

		TEST_METHOD(Test2)
		{
			VM testVM;
			testVM.LoadBinary(test[4]);
			Assert::AreEqual(testResult[4], testVM.run());
		}
	};

	TEST_CLASS(Function)
	{
	public:

		TEST_METHOD(Test1)
		{
			VM testVM;
			testVM.LoadBinary(test[5]);
			Assert::AreEqual(testResult[5], testVM.run());
		}
	};

	TEST_CLASS(Labeling)
	{
	public:

		TEST_METHOD(Test1)
		{
			VM testVM;
			testVM.LoadBinary(test[6]);
			Assert::AreEqual(testResult[6], testVM.run());
		}
	};
}