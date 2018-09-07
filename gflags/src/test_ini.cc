
#include "SimpleIni.h"
#include "utility/logFile.h"

void test_ini()
{

	LOG_DEBUG("test_ini");
	CSimpleIniA ini;
	//ini.SetUnicode(true);
	SI_Error rc = ini.LoadFile("testsi-UTF8.ini");
	if (rc < 0)
	{
		LOG_ERROR("load file fail");
		return;
	}
	{
		const char * pVal = ini.GetValue("standard", "foo", "default");
		LOG_DEBUG("standard.foo=%s", pVal);
	}
	{
		const char * pVal = ini.GetValue("standard", "中文", "default");
		LOG_DEBUG("standard.中文=%s", pVal);
	}
	{
		const char * pVal = ini.GetValue("standard", "k1", "default");
		LOG_DEBUG("standard.k1=%s", pVal);
	}
	{
		const char * pVal = ini.GetValue("standard", "日本語", "default");
		//ini.SetValue("section", "key", "newvalue");
		LOG_DEBUG("standard.日本語=%s", pVal);
	}
}