/*
*      Author: YiliangWu
*/

#pragma once

#include <string>
#include "hiredis.h"
#include "ae.h"
#include "async.h"
#include <vector>
#include <set>

class RedisConn;

class HiredisEventMgr
{
	friend class RedisConn;
public:
	static HiredisEventMgr& Instance()
	{
		static HiredisEventMgr s_instance;
		return s_instance;
	}

public:
	bool Init();
	void Proc();

private:
	HiredisEventMgr();
	~HiredisEventMgr();
	bool AddRedisConn(RedisConn &pRedisConn);
	bool DelRedisConn(RedisConn &pRedisConn);
	aeEventLoop *GetaeEventLoop();
	static int TimerCb(struct aeEventLoop *eventLoop, long long id, void *clientData);
	static void TimerFinCb(struct aeEventLoop *eventLoop, void *clientData);

private:
	bool SetTimer();


private:
	const static long long TIMER_MILLISECONDS = 5 * 1000;
	const static int MAX_REDIS_EVENT_SIZE = 64;
	typedef std::set<RedisConn *> SetConnect;
	SetConnect m_connectSet;

	aeEventLoop *m_pAeLoop;
};



