#pragma once
#include <cstdio>
#include <string>

#define ASSERT_TRUE(condition) if (!condition)\
{ ::test::setErrorInfo(__FILE__, __FUNCTION__, __LINE__); ::test::handleError(); }
#define ASSERT_FALSE(condition) ASSERT_TRUE(!condition)

namespace test {
	static std::string FileName;
	static std::string FuncName;
	static unsigned long LineNumber = 0;
	static bool ErrorOccurred = false;

	static void setErrorInfo(const char* file, const char* func, unsigned long line) {
		FileName = file;
		FuncName = func;
		LineNumber = line;
		ErrorOccurred = true;
	}

	static void handleError() {
		if (!ErrorOccurred)
			return;
		ErrorOccurred = false;
		printf("Assert failed!\n%s: %d\nIn function called: %s\n", FileName.c_str(),
			LineNumber, FuncName.c_str());
	}
}