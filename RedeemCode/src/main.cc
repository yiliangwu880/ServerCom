#include "stdio.h"
#include "utility/logFile.h"

void test();
int main(int argc, char* argv[]) 
{
	printf("hello world\n");
	LOG_DEBUG("hello");
	test();
	return 0;
}

