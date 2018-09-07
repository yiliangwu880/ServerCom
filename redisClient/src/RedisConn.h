/*
 *      Author: YiliangWu
 功能:hiredis 异步调用的 c++接口
 use example:

 HiredisEventMgr::Instance().Init()
 CRedisConn conn;
 conn.Init("127.0.0.1", 9470);
 conn.RedisAsyncCommand(cb, NULL, "SET t1 1");
 while (true)
 {
	HiredisEventMgr::Instance().Proc();
	
	//某个时候 
	....
	conn.RedisAsyncCommand(cb, NULL, "SET t1 2");
 }

 */
#pragma once

#include <string>

#include "config.h"
#include "utility/typedef.h"
#include "hiredis.h"
#include "ae.h"
#include "async.h"

template <typename _Obj>
class GuardDelete
{
public:
	GuardDelete(_Obj *pObj)
		: m_pObj(pObj)
	{
	}

	~GuardDelete()
	{
		if (NULL != m_pObj)
		{
			delete m_pObj;
			m_pObj = NULL;
		}
	}

private:
	_Obj *m_pObj;
};

class BaseCallBackOpt
{
public:
	virtual ~BaseCallBackOpt(){};

	virtual void CmdCallBack(const redisReply *reply) = 0;
};
class RedisConn
{
	friend class HiredisEventMgr;
    public:
        RedisConn();
        virtual ~RedisConn();
		bool Init(const std::string &ip, int port);

		bool IsConnect() const;

		//请求redis 命令， 结果通过回调 CmdCallBack返回
		int RedisAsyncCmd(BaseCallBackOpt * cb_data, const char *format, ...);
		int RedisAsyncCmdArgv(BaseCallBackOpt * cb_data, int argc, const char **argv, const size_t *argvlen);

		//请求创建BaseCallBackOpt派生对象
		//特点：不容易内存泄露
		//注意：创建初始化后，需要调用RedisAsyncCommand，不然会下一次CreateOpt()会销毁
		template <typename DeriveOpt>
		DeriveOpt *CreateOpt()
		{
			if (NULL != m_new_opt)
			{
				delete m_new_opt;
				LOG_ERR("error call, call continue CreateOpt twice!!!\n");
				m_new_opt = NULL;
			}
			DeriveOpt *p = new DeriveOpt();
			m_new_opt = p;
			return p;
		}
private:
	void TryReconnectTimer();
		static void ConnectCallback(const redisAsyncContext * pAeContext, int status);

		static void DisconnectCallback(const redisAsyncContext * pAeContext, int status);
		static void StaticCmdCallback(redisAsyncContext *ac, void *reply, void *cb_data);
		bool Connect();
    private:
		enum State
		{
			S_INIT = 0,
			S_CONNECTIING = 1,
			S_PROC = 2,
			S_DISCONNECTING = 3,
		};

		int m_port;
		std::string m_host;
		redisAsyncContext * m_pAeContext;
		State m_state;
		BaseCallBackOpt *m_new_opt;
};


