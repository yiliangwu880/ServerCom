#include "stdafx.h"
#include "version.h"
#include "include_all.h"
#include "utility/logFile.h"

DebugLog g_logFile("log.txt");
int main(int argc, char* argv[]) 
{
	printf("cur version is [%d]\n", VERSION_REVISION);
		LibEventMgr::Instance().Init();


		LibEventMgr::Instance().dispatch();
	return 0;
}

