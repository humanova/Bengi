// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../Bengi/Bengi/VM.h"

// my test directiories
#if defined(__WIN32__)
	string testFolder = "C:\\Users\\msi\\Desktop\\VSCode Folder\\Bengi-Lang\\tests\\";
#elif defined (__linux__)
	string testFolder = "home\\humanova\\Documents\\pjs\\Bengi\\bengi\\tests\\";
#endif

// test cases
vector<string> test
{ testFolder + "test1.cben",
	testFolder + "test2.cben",
	testFolder + "test3.cben",
	testFolder + "test4.cben",
	testFolder + "test5.cben",
	testFolder + "test6.cben"
};

vector<i32> testResult
{
	40,
	60,
	11,
	46368,
	100,
	10
};

TEST_CASE("Arithmetic and Logical operations " "[ALO]")
{
	VM vm;
	vm.LoadBinary(test[0]);
	REQUIRE(vm.run() == testResult[0]);
}

TEST_CASE("Register and address referencing operations" "[RAFO]")
{
	{
		VM vm; vm.LoadBinary(test[1]);
		REQUIRE(vm.run() == testResult[1]);
	}

	{
		VM vm; vm.LoadBinary(test[2]);
		REQUIRE(vm.run() == testResult[2]);
	}
}

TEST_CASE("Arithmetic-Conditinonal operations", "[ACO]")
{
	{
		VM vm; vm.LoadBinary(test[3]);
		REQUIRE(vm.run() == testResult[3]);
	}

	{
		VM vm; vm.LoadBinary(test[4]);
		REQUIRE(vm.run() == testResult[4]);
	}
}

TEST_CASE("Function operations", "[FO]")
{
	{
		VM vm; vm.LoadBinary(test[5]);
		REQUIRE(vm.run() == testResult[5]);
	}
}