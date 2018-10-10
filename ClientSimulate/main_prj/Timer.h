
#include "include_all.h"
#include "utility/logFile.h"




class ClientTimer : public BaseLeTimer
{
public:
	ClientTimer();
private:
	virtual void OnTimer(void *user_data) override;

private:
	uint32_t m_cnt;
};