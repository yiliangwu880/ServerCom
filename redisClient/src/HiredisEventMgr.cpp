/*
 *      Author: YiliangWu
 */


#include "HiredisEventMgr.h"
#include "config.h"
#include "RedisConn.h"
#include <string.h>

using namespace std;

bool HiredisEventMgr::Init()
{
	if (NULL != m_pAeLoop)
    {
        LOG_ERR("duplicate init");
        return false;
    }

	m_pAeLoop = aeCreateEventLoop(MAX_REDIS_EVENT_SIZE);
    if (NULL == m_pAeLoop)
    {
        LOG_ERR("create event loop failed");
		return false;
    }

	m_pAeLoop->stop = 0;
	SetTimer();
	LOG_INFO("BaseRedisMgr init success");

	return true;
}

void HiredisEventMgr::Proc()
{
	if (!m_pAeLoop->stop)
	{
		aeProcessEvents(m_pAeLoop, AE_DONT_WAIT | AE_ALL_EVENTS);
	}
}

HiredisEventMgr::HiredisEventMgr()
:m_pAeLoop(NULL)
{

}

HiredisEventMgr::~HiredisEventMgr()
{
	if (NULL != m_pAeLoop)
	{
		aeDeleteEventLoop(m_pAeLoop);
		m_pAeLoop = NULL;
	}
}

bool HiredisEventMgr::SetTimer()
{
	long long id = aeCreateTimeEvent(m_pAeLoop, TIMER_MILLISECONDS, TimerCb, this, TimerFinCb);
	if (id < 0)
	{
		LOG_ERR("create time event error\n");
		return false;
	}
	return true;
}

//返回 下次timer间隔 ms.  返回AE_NOMORE表示timer停止
int HiredisEventMgr::TimerCb(struct aeEventLoop *eventLoop, long long id, void *clientData)
{
	HiredisEventMgr *p = (HiredisEventMgr *)clientData;
	for (SetConnect::const_iterator it = p->m_connectSet.begin(); it != p->m_connectSet.end(); ++it)
	{
		RedisConn *conn = *it;
		conn->TryReconnectTimer();
	}
	return p->TIMER_MILLISECONDS;
}

void HiredisEventMgr::TimerFinCb(struct aeEventLoop *eventLoop, void *clientData)
{
	LOG_DBG("timer_fin_cb, timestamp %ld, clientData %s\n", time(NULL), (char *)clientData);
}

bool HiredisEventMgr::AddRedisConn(RedisConn &pRedisConn)
{
	return m_connectSet.insert(&pRedisConn).second;
}

aeEventLoop * HiredisEventMgr::GetaeEventLoop()
{
	return m_pAeLoop; 
}

bool HiredisEventMgr::DelRedisConn(RedisConn &pRedisConn)
{
	return m_connectSet.erase(&pRedisConn) == 1;
}





