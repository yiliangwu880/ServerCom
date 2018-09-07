#include "stdio.h"
#include "utility/logFile.h"
#include "gflags/gflags.h"
#include <iostream>

using namespace std;

//������� ���������ֱ��ʾ���������ơ�Ĭ��ֵ��������Ϣ��
DEFINE_string(confPath, "../conf/setup.ini", "program configure file.");

DEFINE_int32(port, 9090, "program listen port");

DEFINE_bool(daemon, true, "bool parameter");

DEFINE_double(ratio, 1.05, "double parameter");

static bool ValidateStringFlags(const char* name, int value)
{
	if (std::string("port") == name)
	{
	}
	return true; //����flase��������
}

void test_gflags(int argc, char* argv[]) 
{

	const char *p = R"xxx(Usage:
		�ҵ�˵��
		)xxx";
	google::SetVersionString(std::string("1.10"));
	google::SetUsageMessage(std::string(p));
	google::RegisterFlagValidator(&FLAGS_port, &ValidateStringFlags);//�������ò�������Ȼ��������


	gflags::ParseCommandLineFlags(&argc, &argv, true);//��������, true��ʾ�����������flags

	//�������
	cout << "confPath = " << FLAGS_confPath << endl;
	cout << "port = " << FLAGS_port << endl;


}

