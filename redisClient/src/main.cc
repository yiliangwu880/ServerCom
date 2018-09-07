#include "stdio.h"
#include "config.h"
#include "utility/logFile.h"
#include <vector>

DebugLog g_logFile("log.log");
void test();
int main(int argc, char* argv[]) 
{

	printf("hello world\n");
	LOG_DBG("hello");
	test();
	return 0;
}

