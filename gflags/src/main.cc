#include "stdio.h"
#include "utility/logFile.h"
#include "gflags/gflags.h"
#include <iostream>

using namespace std;

//定义参数 三个参数分别表示：参数名称、默认值、帮助信息。
DEFINE_string(confPath, "../conf/setup.ini", "program configure file.");

DEFINE_int32(port, 9090, "program listen port");

DEFINE_bool(daemon, true, "bool parameter");

DEFINE_double(ratio, 1.05, "double parameter");

static bool ValidateStringFlags(const char* name, int value)
{
	if (std::string("port") == name)
	{
	}
	return true; //返回flase不能启动
}

int main(int argc, char* argv[]) 
{
	printf("hello world\n");
	LOG_DEBUG("hello");


	const char *p = R"xxx(Usage:
		我的说明
		)xxx";
	google::SetVersionString(std::string("1.10"));
	google::SetUsageMessage(std::string(p));
	google::RegisterFlagValidator(&FLAGS_port, &ValidateStringFlags);//必须设置参数，不然启动不了


	gflags::ParseCommandLineFlags(&argc, &argv, true);//参数解析, true表示不保留定义的flags

	//参数输出
	cout << "confPath = " << FLAGS_confPath << endl;
	cout << "port = " << FLAGS_port << endl;


	return 0;
}

