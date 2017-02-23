#pragma once
#include "Net.h"
using namespace Network;
class NetworkHandler
{
public:
	void Send(MessageData* data, const char * destHost, int destPort);
	void Send(MessageData* data);
	ReceivedData Receive();

	static NetworkHandler & Instance()
	{
		static NetworkHandler myInstance;
		return myInstance;
	}

	NetworkHandler(NetworkHandler const&) = delete;
	NetworkHandler(NetworkHandler&&) = delete;
	NetworkHandler& operator=(NetworkHandler const&) = delete;
	NetworkHandler& operator=(NetworkHandler &&) = delete;
	bool GetConnected() const { return _connected; };
	void SetConnected(bool connected) {	_connected = connected;	}
	void Disconnect();
	void SetSessionID(int id) { _sessionID = id; }
	void SetPlayerID(int id) { _playerID = id; }
	bool GetSessionID() const { return _sessionID; };
	bool GetPlayerID() const { return _playerID; };

private:
	const int PORT = 6000; //5228; when using ec2
	Net _net;
	bool _connected;
	int _sessionID;
	int _playerID;
	IPaddress _serverIP;

private:
	NetworkHandler();
	~NetworkHandler();
};
