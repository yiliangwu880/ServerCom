#include "ClientConnect.h"

using namespace std;

namespace
{
	void Str2MsgPack(const string str, MsgPack &msg)
	{
		msg.len = str.length() + 1;
		memcpy(msg.data, str.c_str(), str.length() + 1);
	}
}

void ClientConnect::OnRecv(const MsgPack &msg)
{
	LOG_DEBUG("1 OnRecv %s", &msg.data);

}

void ClientConnect::OnConnected()
{
	LOG_DEBUG("1 OnConnected, send first msg");
	MsgPack msg;
	Str2MsgPack("1 msg", msg);
	send_data(msg);
}

void ClientConnect::on_disconnected()
{

}


void ClientConnect::OnError(short events)
{
	LOG_DEBUG("on_error");
}
