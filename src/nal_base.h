/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef h__nal_networking__
#define h__nal_networking__

#include <string>
#include <iostream>
#include <vector>

#include "nal_packetQueue.h"
#include "nal_opcodes.h"
#include "nal_timer.h"

bool nal_create();
void nal_destroy();

#define NAL_PEER_TYPE_ENET 0

class NALPeer
{
public:
	///Hack to make dynamic_cast work :(IE; make the class polymorphic)
	virtual unsigned char getType() = 0;
	///Constructor
	NALPeer(const std::string& id);
	///Destructor
	virtual ~NALPeer();
	///Retreive the ID
	const std::string& getID();
protected:
	///Peer id
	std::string m_id;
private:
};

class NALHost;
class NALListener
{
	friend class NALHost;
public:
    virtual ~NALListener(){}
protected:
	///OnConnect
	virtual bool onConnect( NALPeer* peer ) = 0;
	///OnDisconnect
	virtual bool onDisconnect( NALPeer* peer ) = 0;
	///OnPacket
	virtual bool onPacket( NALPeer* sender, const NALPacketHeader& header, void* data ) = 0;
private:
};

class NALPeerList;

class NALBase
{
public:
	///Constructor
	NALBase();
	///Destructor
	virtual ~NALBase();
	///Sends a packet to the peer
	virtual void sendPacket(NALPacket *packet);
	///The packet queue
	NALPacketQueue PacketQueue;
	///Sends the packet queue
	void sendPacketQueue();
	///Send a custom object
	template<typename T> void sendCustomData(unsigned short opCode, const T &data, NALPeer *receiver = 0, NALDeliveryStrategy dModel = dsRELIABLE)
	{
		NALPacket* pck = new NALPacket();
		pck->setDestination(receiver);
		pck->setOpCode(opCode);
		pck->attachData(data);
		pck->setDeliveryModel(dModel);
		PacketQueue.push(pck);
	}

	///Send a reliable opcode
	void sendOpCode(unsigned short opCode, NALPeer *receiver = 0)
	{
		NALPacket* pck = new NALPacket();
		pck->setDestination(receiver);
		pck->setOpCode(opCode);
		pck->setDeliveryModel(dsRELIABLE);
		PacketQueue.push(pck);
	}
protected:
	///Enabled?
	bool m_enabled;
	///List of connected peers
	NALPeerList *m_peers;

private:
};

class NALHost;
class NALPeerList : public NALBase
{
public:
    virtual ~NALPeerList(){}

	///THIS FUNCTION IS RESPONSIBLE FOR DELETING THE PACKET AFTER SENDING!
	virtual void sendPacket(NALPacket *packet) = 0;
	///Number of peers
	unsigned int peerCount();
	///operator overload to get peers
	NALPeer& operator[](unsigned int index)
	{
		return *m_peers[index];
	}
	///Get a peer
	NALPeer* getPeer(unsigned int i);
	///Add a peer
	void add(NALPeer* peer);
	///Remove a peer
	void remove(unsigned int id);
	///Remove a peer
	void remove(NALPeer *peer);

	///Get a peer from the string id
	NALPeer* getPeer(const std::string& id);
protected:
	std::vector<NALPeer*> m_peers;
private:
};



#endif
