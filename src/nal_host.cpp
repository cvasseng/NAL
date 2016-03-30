/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_host.h"

NALHost::NALHost()
{
	PollIntervalMS = 20;
	KeepAlive = 8;
	m_port = 0;
	m_peers = 0;
	m_connected = false;
	m_addressString = "";
	m_isServer = false;
}

void NALHost::setInfo( short port, const std::string& address, bool isServer )
{
	m_isServer = isServer;
	m_port = port;
	m_addressString = address;

	
}

void NALHost::receive( NALPeer* sender, NALPacketHeader* header, void* data )
{
	//	std::cout << "Received Packet - OPCODE : " << header->opCode << " SIZE: " << (header->size + sizeof(sNALPacketHeader) ) << " byte" << std::endl;

	for (unsigned int i = 0; i < m_listeners.size(); i++)
	{
		m_listeners[i]->onPacket(sender, *header, data);
	}
}

void NALHost::log( const std::string& str )
{
	//std::cout << str << std::endl;
}

bool NALHost::validateSender( unsigned int sender, std::string mdHash )
{
	/*	std::stringstream ss;
	ss << sender;
	std::string res = md5(ss.str());
	if (res == mdHash)
	return true;
	return false;*/
	return true;
}

NALPacketHeader* NALHost::extractPackets( NALPeer* sender, char* data, unsigned int size )
{
	if (!sender)
	{
		
	}
	
	NALPacketHeader *header = (NALPacketHeader*)(data);
	if (header->opCode == NAL_OP_PACKET_KEEP_ALIVE)
	{//Drop the packet, it's juts a "hey, im still here!" signal
	//	log("Received keep alive packet");
		return 0;
	}

	if (header->opCode == NAL_OP_PACKET_GROUP)
	{//We're dealing with a group of packets. Extract all of them.
		//std::cout << "Received packet queue with " << header->size << " packets" << std::endl;
		int pCount = header->size;
		unsigned short bPos = sizeof(NALPacketHeader);
		for (int i = 0; i < pCount; i++)
		{
			//Extract the header for the packet
			NALPacketHeader *nHeader = (NALPacketHeader*)(data + bPos);
			//Allocate some room for the data
			char *vdata = new char[nHeader->size];
			//Extract the packet data and call receive
			memcpy(vdata, data + bPos + sizeof(NALPacketHeader), nHeader->size);
			receive(sender, nHeader, vdata);
			//Increment the byte position
			bPos += sizeof(NALPacketHeader) + nHeader->size;
		}
	}
	else if (header->size > 0)
	{//Assuming there's only one packet in the packet.
		char *vdata = new char[header->size];
		memcpy(vdata, data + sizeof(NALPacketHeader), header->size);
		receive(sender, header, vdata);
	}
	else
	{//No valid tail data in packet, so just forward the header and let the callback handle the operation
		receive(sender, header, 0);
	}
	return header;
}

void NALHost::attachListener( NALListener* listener )
{
	m_listeners.push_back(listener);
}

void NALHost::detachListener( NALListener* listener )
{

}

void NALHost::doOnConnect( NALPeer* peer )
{
	for (unsigned int i = 0; i < m_listeners.size(); i++)
	{
		m_listeners[i]->onConnect(peer);
	}
}

void NALHost::doOnDisconnect( NALPeer* peer )
{
	for (unsigned int i = 0; i < m_listeners.size(); i++)
	{
		m_listeners[i]->onDisconnect(peer);
	}
}

void NALHost::update()
{

	if (!m_enabled)
		return;

	if (m_keepAliveTimer.time() > (unsigned int)KeepAlive * 1000 && KeepAlive != 0)
	{///Send keep-alive packet
		NALPacket *packet = new NALPacket;
		packet->setOpCode(NAL_OP_PACKET_KEEP_ALIVE);
		sendPacket(packet);
		delete packet;
		m_keepAliveTimer.reset();
	}

	if (m_pollTimer.time() > (unsigned int)PollIntervalMS || PollIntervalMS == 0)
	{	
		sendPacketQueue();
		//m_peers->sendPacketQueue();

		listen();
		m_pollTimer.reset();
	}
}

NALPeerList* NALHost::getPeerList()
{
	return m_peers;
}
