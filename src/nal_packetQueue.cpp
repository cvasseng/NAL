/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_base.h"
#include "nal_packetQueue.h"
#include <iostream>

NALPacketQueue::NALPacketQueue()
{

}

void NALPacketQueue::push( NALPacket *packet )
{
	std::map<std::string, NALPacketGroup*>::iterator it = m_packetQueue.find(packet->getDestination()->getID());
	if (it != m_packetQueue.end())
	{
	//	std::cout << "ADDING PACKET TO: " << it->first << std::endl;
		//Add packet
		it->second->attachPacket(packet);
	}
	else
	{
	//	std::cout << "INSERTING PACKET IN: " << packet->getDestination()->getID() << std::endl;
		//Insert
		NALPacketGroup *g = new NALPacketGroup;
		g->attachPacket(packet);
		g->setDestination(packet->getDestination());
		m_packetQueue.insert(std::pair<std::string, NALPacketGroup*>(packet->getDestination()->getID(), g));
	}
}

bool NALPacketQueue::isEmpty()
{
	return m_packetQueue.empty();
}

unsigned int NALPacketQueue::size()
{
	return m_packetQueue.size();
}

void NALPacketQueue::buildPacketList( std::vector<NALPacket*> &dest )
{
	for (std::map<std::string, NALPacketGroup*>::iterator it = m_packetQueue.begin(); it != m_packetQueue.end(); it++)
	{
		dest.push_back(it->second);
	}
}

void NALPacketQueue::clear()
{

	m_packetQueue.clear();
}
