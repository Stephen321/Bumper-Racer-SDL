#pragma once

#include <SDL2/SDL_net.h>
#include <iostream>

class Net
{
public:
	enum class MessageType : Uint8 {
		Connect,
		Disconnect,
		State
	};

	struct MessageData {
		float ts;
		int length;
		MessageType GetType() const {
			return type;
		}
	protected:
		MessageType type;
	};

	struct StateData : MessageData {
		StateData() { type = MessageType::State; }
		float xPos;
		float yPos;
		float xVel;
		float yVel;
	};

	//test data
	struct ConnectData : MessageData {
		ConnectData() { type = MessageType::Connect; }
		std::string s;
	};
	//test data
	struct DisconnectData : MessageData {
		DisconnectData() { type = MessageType::Disconnect; }
		std::string s;
	};

	class ReceivedData {
	public:
		ReceivedData()
			: _data(nullptr)
		{
		}
		ReceivedData(const ReceivedData& rhs)
		{
			if (!rhs._data)
			{
				return;
			}
			switch (rhs._data->GetType())
			{ //template specialization to use enum type in order to determine Type
				case MessageType::Connect:
				{
					_data = new ConnectData(*rhs.GetData<ConnectData>());
					break;
				}
				case MessageType::Disconnect:
				{
					_data = new DisconnectData(*rhs.GetData<DisconnectData>());
					break;
				}
				case MessageType::State:
				{
					_data = new StateData(*rhs.GetData<StateData>());
					break;
				}
			}
		}

		template <typename T>
		void SetData(T data)
		{
			_data = new T(data);
		}

		~ReceivedData()
		{
			if (_data != nullptr)
			{
				delete _data;
				_data = nullptr;
			}
		}

		bool Empty() const
		{
			return _data == nullptr;
		}

		MessageType GetType() const
		{
			return _data->GetType();
		}

		template <typename T>
		T* GetData() const
		{
			return static_cast<T*>(_data);
		}

	private:
		MessageData* _data;
	};

	Net(const std::string& targetHost, int packetSize = 256, int serverPort = 4023, int clientPort = 4000);

	void Send(MessageData* data, UDPsocket* socket = nullptr)
	{
		if (!data)
		{
			std::cout << "Send tried to send nullptr" << std::endl;
			return;
		}

		MessageType type = data->GetType();
		_packet->len = 0;
		WriteInt((Uint8)type); 

		switch (type)
		{
			case MessageType::Connect:
			{
				ConnectData* cd = (ConnectData*)data;
				WriteString(cd->s);
				break;
			}
			case MessageType::Disconnect:
			{
				DisconnectData* dd = (DisconnectData*)data;
				WriteString(dd->s);
				break;
			}
		}

		//SetPacketAddress();
		if (!socket)
		{
			if (SDLNet_UDP_Send(_socket, -1, _packet) == 0)
				std::cout << "Failed to send packet." << std::endl;
		}
		else
		{
			if (SDLNet_UDP_Send(*socket, -1, _packet) == 0)
				std::cout << "Failed to send packet." << std::endl;
		}
	}
	
	ReceivedData Receive()
	{
		ReceivedData receiveData;
		if (SDLNet_UDP_Recv(_socket, _packet) > 0)
		{
			int byteOffset = 0;
			MessageType type = (MessageType)ReadInt(byteOffset);

			switch (type)
			{
				case MessageType::Connect:
				{
					ConnectData cd;
					cd.s = ReadString(byteOffset);
					receiveData.SetData(cd);
					break;
				}
				case MessageType::Disconnect:
				{
					DisconnectData dd;
					dd.s = ReadString(byteOffset);
					receiveData.SetData(dd);
					break;

				}
			}
		}
		return receiveData;
	}

private: 
	//void SetPacketAddress();

	void WriteInt(int value);
	int ReadInt(int& byteOffset);

	void WriteString(std::string& s);
	std::string ReadString(int& byteOffset);

	//IPaddress _ipAddress;
	UDPpacket* _packet;
	UDPsocket _socket;
};