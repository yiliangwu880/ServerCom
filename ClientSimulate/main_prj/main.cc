#include "stdafx.h"
#include "version.h"
#include "include_all.h"
#include "utility/logFile.h"
#include "utility/BacktraceInfo.hpp"
#include "Timer.h"
#include "ClientConnect.h"

namespace
{
const char *SERVER_IP = "192.168.1.119";
const int SERVER_PORT = 11190;
}

int main(int argc, char* argv[]) 
{
	CBacktraceInfo::Instance().RegHangUpHandler();
	LOG_DEBUG("---------------------start---------------------");
	LibEventMgr::Instance().Init();

	ClientTimer timer;
	timer.StartTimer(1*1000, 0, true);

	ClientConnect c;
	c.ConnectInit(SERVER_IP, SERVER_PORT);

	LibEventMgr::Instance().dispatch();
	LOG_DEBUG("---------------------main end---------------------");
	return 0;
}

