/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef nal_host_h__
#define nal_host_h__

#include "nal_base.h"

class NALHost : public NALBase
{
public:
	NALHost();
	virtual ~NALHost(){}
	///Set the connection info
	void setInfo(short port, const std::string& address, bool isServer);
	///Enables networking and connects (or starts the server if server) based on info set in setInfo
	virtual bool enable(bool flag) = 0;
	///Attaches a listener to the host
	void attachListener(NALListener* listener);
	///Detaches a listener from the host
	void detachListener(NALListener* listener);
	///Updates 
	virtual void update();
	///Get the peer list
	NALPeerList* getPeerList();
	///Get the host port
	short getHostPort(){return m_port;}
	///IS server?
	bool isServer(){return m_isServer;}
	///Is connected?
	bool isConnected(){return m_connected;}
	///Get the address which the host is bound to
	const std::string& getHostAddress(){return m_addressString;}
	
	
	///Keep alive delay (0 = disabled)
	unsigned short KeepAlive;
	///Poll interval, how often to check for events
	unsigned short PollIntervalMS;
protected:
	///Stores the host id if client
	std::string m_id;
	///Polling timer
	NALTimer m_pollTimer;
	///Keep alive timer
	NALTimer m_keepAliveTimer;
	///Attached listeners
	std::vector<NALListener*> m_listeners;
	///Connected flag
	bool m_connected;
	///The port
	short m_port;
	///The address
	std::string m_addressString;
	///Is server?
	bool m_isServer;
	///Receives a packet from the client
	void receive(NALPeer* sender, NALPacketHeader* header, void* data);
	///Logging function
	void log(const std::string& str);
	///Validates that the sender is indeed the sender
	bool validateSender(unsigned int sender, std::string mdHash);
	///Extracts packages, and calls receive when applicable
	NALPacketHeader* extractPackets(NALPeer* sender, char* data, unsigned int size);
	///Executes onConnect
	void doOnConnect(NALPeer* peer);
	///Executes onDisconnect
	void doOnDisconnect(NALPeer* peer);
	///Listen for new connections and events
	virtual void listen() = 0;
private:
};

#endif // nal_host_h__
