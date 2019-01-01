#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../Bengi/VM.h"

string testFolder = "C:\\Users\\msi\\Desktop\\VSCode Folder\\Bengi-Lang\\tests\\";
vector<string> test
{ testFolder + "test1.cben",
	testFolder + "test2.cben",
	testFolder + "test3.cben",
	testFolder + "test4.cben",
	testFolder + "test5.cben"
};
vector<i32> testResult
{
	40,
	10,
	11,
	55,
	100
};

TEST_CASE("Arithmetic and Logical operations " "[ALO]")
{
	VM vm;
	vm.loadBinary(test[0]);
	REQUIRE(vm.run() == testResult[0]);
}

TEST_CASE("Register and address referencing operations" "[RAFO]")
{
	{
		VM vm; vm.loadBinary(test[1]);
		REQUIRE(vm.run() == testResult[1]);
	}

	{
		VM vm; vm.loadBinary(test[2]);
		REQUIRE(vm.run() == testResult[2]);
	}
}

TEST_CASE("Arithmetic-Conditinonal operations", "[ACO]")
{
	{
		VM vm; vm.loadBinary(test[3]);
		REQUIRE(vm.run() == testResult[3]);
	}

	{
		VM vm; vm.loadBinary(test[4]);
		REQUIRE(vm.run() == testResult[4]);
	}
}