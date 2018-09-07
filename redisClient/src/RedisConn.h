/*
 *      Author: YiliangWu
 ����:hiredis �첽���õ� c++�ӿ�
 use example:

 HiredisEventMgr::Instance().Init()
 CRedisConn conn;
 conn.Init("127.0.0.1", 9470);
 conn.RedisAsyncCommand(cb, NULL, "SET t1 1");
 while (true)
 {
	HiredisEventMgr::Instance().Proc();
	
	//ĳ��ʱ�� 
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

		//����redis ��� ���ͨ���ص� CmdCallBack����
		int RedisAsyncCmd(BaseCallBackOpt * cb_data, const char *format, ...);
		int RedisAsyncCmdArgv(BaseCallBackOpt * cb_data, int argc, const char **argv, const size_t *argvlen);

		//���󴴽�BaseCallBackOpt��������
		//�ص㣺�������ڴ�й¶
		//ע�⣺������ʼ������Ҫ����RedisAsyncCommand����Ȼ����һ��CreateOpt()������
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


