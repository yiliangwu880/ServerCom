
#include "include_all.h"
#include "utility/logFile.h"
//#include "google/include/google/protobuf/message.h"




class ClientConnect : public BaseConnect
{
private:
	virtual void OnRecv(const MsgPack &msg) override;
	virtual void OnConnected() override;
	virtual void OnError(short events) override;
	virtual void on_disconnected() override;
};