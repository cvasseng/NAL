/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_base.h"

#ifdef WIN32
    #include "../enet/include/enet.h"
#else
	#include <enet/enet.h>
#endif

bool nal_create()
{
	if (enet_initialize() != 0)
	{
		std::cout << "Failed to initialize network!" << std::endl;
		return false;
	}
	//std::cout << "Networking Initialized..." << std::endl;
	return true;
}

void nal_destroy()
{
	atexit(enet_deinitialize);
}



//////////////////////////////////////////////////////////////////////////

NALBase::NALBase()
{
	m_peers = 0;
}

NALBase::~NALBase()
{
	//Free the peer list
	if (m_peers)
	{
		delete m_peers;
	}
}

void NALBase::sendPacketQueue()
{
	//Dispatch packets from the queue
	std::vector<NALPacket*> packets;
	PacketQueue.buildPacketList(packets);
	for (unsigned int i = 0; i < packets.size(); i++)
	{
		sendPacket(packets[i]);
	}
	PacketQueue.clear();
}

void NALBase::sendPacket( NALPacket *packet )
{
	//if (packet)
	//	if (packet->getDestination())
	//		std::cout << "SENDING PACKET TO " << packet->getDestination()->getID() << std::endl;
	if (m_peers)
		m_peers->sendPacket(packet);
}


//////////////////////////////////////////////////////////////////////////

NALPeer::NALPeer( const std::string& id )
{
	m_id = id;
}


NALPeer::~NALPeer()
{

}

const std::string& NALPeer::getID()
{
	return m_id;
}

//////////////////////////////////////////////////////////////////////////

void NALPeerList::add( NALPeer* peer )
{
	m_peers.push_back(peer);
}

unsigned int NALPeerList::peerCount()
{
	return m_peers.size();
}

void NALPeerList::remove( unsigned int id )
{
	if (id < m_peers.size())
		m_peers.erase(m_peers.begin() + id);
}

void NALPeerList::remove( NALPeer *peer )
{
	for (unsigned int i = 0; i < m_peers.size(); i++)
		if (peer->getID() == m_peers[i]->getID())
		{
			m_peers.erase(m_peers.begin() + i);
			return;
		}
}

NALPeer* NALPeerList::getPeer( unsigned int i )
{
	if (i < m_peers.size())
		return m_peers[i];
	return 0;
}

NALPeer* NALPeerList::getPeer(const std::string& id)
{
	for (unsigned int i = 0; i < m_peers.size(); i++)
		if (id == m_peers[i]->getID())
		{
			return m_peers[i];
		}
	return 0;
}
