/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef h__networkingEnet__
#define h__networkingEnet__

//LINK WITH
//	enet-d.lib
//	ws2_32.lib

#ifdef WIN32
    #include "../enet/include/enet.h"
	#ifndef LIB
		#pragma comment(lib, "winmm.lib")
		#pragma comment(lib, "ws2_32.lib")
	#endif
#else
	#include <enet/enet.h>
#endif

#include "nal_base.h"
#include "nal_host.h"
#include "md5.h"

#include <sstream>
#include <vector>

class NALPeerEnet : public NALPeer
{
public:
	NALPeerEnet(const std::string& id, ENetPeer* peer) : NALPeer(id)
	{
		m_peer = peer;
	}
	unsigned char getType()
	{
		return 0;
	}
	ENetPeer* getEnetPeer()
	{
		//m_peer->host->address.host
		return m_peer;
	}
protected:
	ENetPeer *m_peer;
private:
};

class NALPeerListEnet : public NALPeerList
{
public:
	void sendPacket( NALPacket *packet );
protected:
private:
};

class NALHostEnet : public NALHost
{
public:
	///Constructor
	NALHostEnet();
	///Destructor
	~NALHostEnet();
	///Enable networking
	bool enable(bool flag);
protected:
	///Listen for new events
	void listen();
	///Connects to the server
	bool clientConnect();
	///The address
	ENetAddress m_address;
	///The host
	ENetHost *m_host;
private:
};

#endif

