#include "stdio.h"
#include "utility/logFile.h"
#include "serialcode_parser.h"

void test();
int main(int argc, char* argv[]) 
{
	//test();
	RedeemCodeTool::Instance().Run(argc, argv);
	return 0;
}

