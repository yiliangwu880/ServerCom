#include "stdio.h"
#include "utility/logFile.h"
#include <iostream>

using namespace std;

void test_gflags(int argc, char* argv[]);
void test_ini();

int main(int argc, char* argv[]) 
{
	DebugLog::GetDefaultLog().setStdOut(true);
	printf("hello world\n");
	LOG_DEBUG("hello");
	//test_gflags(argc, argv);
	test_ini();
	return 0;
}

