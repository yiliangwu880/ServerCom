/*
*      Author: YiliangWu
*/



#include <stdlib.h>
#include "RedisConn.h"
#include "HiredisEventMgr.h"
#include "adapters/libae.h"

RedisConn::RedisConn()
:m_port(0)
, m_host()
, m_pAeContext(NULL)
, m_state(S_INIT)
, m_new_opt(NULL)
{
	HiredisEventMgr::Instance().AddRedisConn(*this);
}

RedisConn::~RedisConn()
{
	if (NULL != m_pAeContext)
	{
		redisAsyncDisconnect(m_pAeContext);
	}
	HiredisEventMgr::Instance().DelRedisConn(*this);

}

bool RedisConn::Init(const std::string &ip, int port)
{
	if (0 != m_port)
	{
		LOG_ERR("repeated Init");
		return false;
	}
	
	m_port = port;
	m_host = ip;

	return Connect();
}

bool RedisConn::Connect()
{
	aeEventLoop *aeLoop = HiredisEventMgr::Instance().GetaeEventLoop();
	if (NULL == aeLoop)
	{
		LOG_ERR("GetaeEventLoop fail");
		return false;
	}
	if (S_INIT != m_state)
	{
		LOG_ERR("error state %d", m_state);
		return false;
	}

	m_pAeContext = redisAsyncConnect(m_host.c_str(), m_port);
	if (NULL == m_pAeContext)
	{
		return false;
	}
	if (m_pAeContext->err)
	{
		LOG_ERR("try connect redis failed. Host: %s, Port: %d, Err: %s", m_host.c_str(), m_port, m_pAeContext->errstr);
		return false;
	}

	m_pAeContext->data = this;

	redisAeAttach(aeLoop, m_pAeContext);
	redisAsyncSetConnectCallback(m_pAeContext, ConnectCallback);
	redisAsyncSetDisconnectCallback(m_pAeContext, DisconnectCallback);
	m_state = S_CONNECTIING;
	return true;
}

void RedisConn::ConnectCallback(const redisAsyncContext *pAeContext, int status)
{
	if (NULL == pAeContext)
	{
		LOG_ERR("NULL == pAeContext");
		return;
	}
	
	RedisConn *pThis = (RedisConn *)pAeContext->data;
	if (NULL == pThis)
	{
		LOG_ERR("NULL == pThis");
		return;
	}

	if (S_CONNECTIING != pThis->m_state)
	{
		LOG_ERR("error state. %d", pThis->m_state);
	}
	if (REDIS_OK != status)
	{
		LOG_INFO("connect redis failed. Err: %s", pAeContext->errstr);

		pThis->m_state = S_INIT;
		pThis->m_pAeContext = NULL; //回调后，会自动释放
		return;
	}

	LOG_INFO("connect redis success");
	pThis->m_state = S_PROC;

}


void RedisConn::DisconnectCallback(const redisAsyncContext *pAeContext, int status)
{
	if (NULL == pAeContext)
	{
		LOG_ERR("NULL == pAeContext");
		return;
	}
	RedisConn * pThis = (RedisConn *)pAeContext->data;
	if (NULL == pThis)
	{
		LOG_ERR("NULL == pThis");
		return;
	}
	if (S_PROC != pThis->m_state)
	{
		LOG_ERR("error state. %d", pThis->m_state);
	}
	pThis->m_state = S_INIT;
	pThis->m_pAeContext = NULL; //防野。 回调后，hiredis会释放内存.
	LOG_INFO("disconnect redis success, Status: %d",  status);

}

void RedisConn::StaticCmdCallback(redisAsyncContext *pAeContext, void *reply, void *cb_data)
{
	BaseCallBackOpt *opt = (BaseCallBackOpt *)cb_data;
	GuardDelete<BaseCallBackOpt> guard(opt);
	opt->CmdCallBack((const redisReply *)reply);

}

int RedisConn::RedisAsyncCmd(BaseCallBackOpt * cb_data, const char *format, ...)
{
	if (NULL == m_pAeContext)
	{
		LOG_ERR("NULL == pAeContext");
		return REDIS_ERR;
	}
	if (cb_data != m_new_opt)
	{
		LOG_ERR("error call, cb_data must be created by last RedisConn::CreateOpt");
		return REDIS_ERR;
	}
	va_list ap;
	va_start(ap, format);
	int status = redisAsyncCommand(m_pAeContext, StaticCmdCallback, (void *)cb_data, format, ap);
	va_end(ap);
	m_new_opt = NULL;
	return status;
}


int RedisConn::RedisAsyncCmdArgv(BaseCallBackOpt * cb_data, int argc, const char **argv, const size_t *argvlen)
{
	if (NULL == m_pAeContext)
	{
		LOG_ERR("NULL == pAeContext");
		return REDIS_ERR;
	}
	if (cb_data != m_new_opt)
	{
		LOG_ERR("error call, cb_data must be created by last RedisConn::CreateOpt");
		return REDIS_ERR;
	}
	m_new_opt = NULL;
	return redisAsyncCommandArgv(m_pAeContext, StaticCmdCallback, (void *)cb_data, argc, argv, argvlen);
}

void RedisConn::TryReconnectTimer()
{
	if (m_state != S_INIT)
	{
		return;
	}


	LOG_INFO("try connect redis tick");

	Connect();
}

bool RedisConn::IsConnect() const
{
	return S_PROC == m_state;
}

