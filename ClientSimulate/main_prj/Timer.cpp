#include "Timer.h"

ClientTimer::ClientTimer()
:m_cnt(0)
{

}

//ÿ��һ��
void ClientTimer::OnTimer(void *user_data)
{
	LOG_DEBUG("on timer");
	m_cnt++;
	if (m_cnt>2)
	{
		LibEventMgr::Instance().StopDispatch();
	}
}
