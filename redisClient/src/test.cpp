#include "config.h"
#include "HiredisEventMgr.h"
#include "RedisConn.h"
#include "malloc.h"
#include <string.h>

static const int PORT = 9470;
class MyCallBackOpt : public BaseCallBackOpt
{
	virtual void CmdCallBack(const redisReply *reply)
	{
		if (REDIS_REPLY_ERROR == reply->type)
		{
			LOG_ERR("redis callback. type: %d, interger: %lld, str: %s, element_size: %d"
				,reply->type, reply->integer
				, reply->str, reply->elements);
		}
		LOG_INFO("type=%d, integer=%d, str=[%s] ", reply->type, reply->integer, reply->str);
	}
};

void testAynCmd(RedisConn &conn)
{
	const char * argv[50];
	size_t argvlen[50];
	int argc = 0;

	argv[argc++] = "SET";
	argv[argc++] = "T2";
	argv[argc++] = "SS";

	for (int i = 0; i < argc; ++i)
	{
		argvlen[i] = strlen(argv[i]);
	}
	MyCallBackOpt *opt = conn.CreateOpt<MyCallBackOpt>();
	conn.RedisAsyncCmdArgv(opt, argc, argv, argvlen);

}
void testNormal()
{
	if (!HiredisEventMgr::Instance().Init())
	{
		LOG_ERR("init fail");
		return;
	}

	RedisConn conn;
	conn.Init("127.0.0.1", PORT);
	{
		MyCallBackOpt *opt = conn.CreateOpt<MyCallBackOpt>();
		conn.RedisAsyncCmd(opt, "SET t1 1");
	}	
	{
		MyCallBackOpt *opt = conn.CreateOpt<MyCallBackOpt>();
		conn.RedisAsyncCmd(opt, "SETs t1 1");
	}
	testAynCmd(conn);

	time_t start_tm = time(NULL);
	while (true)
	{
		HiredisEventMgr::Instance().Proc();

		time_t cur = time(NULL);
		if (cur - start_tm > 2)
		{
			return;
		}
	}
}

//运行期间，需要重启redis服务器多次
void testLeapMemory()
{
	if (!HiredisEventMgr::Instance().Init())
	{
		LOG_ERR("init fail");
		return;
	}
	//test leap memory
	for (int i = 0; i < 60; ++i)
	{
		RedisConn conn;
		conn.Init("127.0.0.1", PORT);

	}

	RedisConn conn;
	conn.Init("127.0.0.1", PORT);
	MyCallBackOpt *opt = conn.CreateOpt<MyCallBackOpt>();
	conn.RedisAsyncCmd(opt, "SET t1 1");

	time_t start_tm = time(NULL);
	while (true)
	{
		HiredisEventMgr::Instance().Proc();

		time_t cur = time(NULL);

		if (cur - start_tm > 10)
		{
			return;
		}

	}
	LOG_DBG("test end");

}
void test() 
{
	testNormal();
	LOG_DBG("test end");
}

