/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_base.h"
#include "nal_packet.h"

NALPacket::NALPacket()
{
	m_deliveryStrat = dsRELIABLE;
	m_data = 0;
	m_header.size = 0;
	m_header.opCode = NAL_OP_NOOP;
	m_header.broadcast = false;
	m_totalSize = sizeof(NALPacketHeader);
	m_priortiy = 0.4f;
	m_onlyOne = false;
	m_destination = 0;
}

void NALPacket::setOpCode( const unsigned short& code )
{
	m_header.opCode = code;
}

void NALPacket::setBroadcastFlag( bool flag )
{
	m_header.broadcast = flag;
}

void NALPacket::setDeliveryModel( NALDeliveryStrategy model )
{
	m_deliveryStrat = model;
}

NALDeliveryStrategy NALPacket::getDeliveryModel()
{
	return m_deliveryStrat;
}

void NALPacket::attachData( void* data, unsigned int size )
{
	m_header.size = size;
	m_data = (char*)data;
	m_totalSize = size + sizeof(NALPacketHeader);
}


char* NALPacket::serialize()
{
	//Find size and allocate dest
	int size = m_header.size + sizeof(NALPacketHeader);
	//	std::cout << "Serializing Packet (" << size << " bytes total)" << std::endl;
	char* dest = new char[m_totalSize]; 

	//Attach the header
	memcpy(dest, &m_header, sizeof(NALPacketHeader));

	if (m_data)
		//Attach the data
		memcpy(dest + sizeof(NALPacketHeader), m_data, m_header.size);

	return dest;
}

short NALPacket::size()
{
	return m_totalSize;
}

NALPacket::~NALPacket()
{
	//	if (m_data)
	//		delete m_data;
}

bool NALPacket::getBroadcastFlag()
{
	return m_header.broadcast;
}

const float& NALPacket::getPriority()
{
	return m_priortiy;
}

void NALPacket::setPriority( float pri )
{
	m_priortiy = pri;
}

unsigned short NALPacket::getOpCode()
{
	return m_header.opCode;
}

void NALPacket::setUnique( bool flag )
{
	m_onlyOne = flag;
}

bool NALPacket::getUnique()
{
	return m_onlyOne;
}

void NALPacket::setDestination( NALPeer* peer )
{
	m_destination = peer;
}

NALPeer* NALPacket::getDestination()
{
	return m_destination;
}
//////////////////////////////////////////////////////////////////////////

void NALPacketGroup::attachPacket( NALPacket* packet )
{
	NALEmbeddedPacket tmp;
	tmp.data = packet->serialize();
	tmp.size = packet->size();
	m_header.opCode = NAL_OP_PACKET_GROUP;

	m_embeddedPackets.push_back(tmp);
	m_header.size++;
	m_totalSize += packet->size();
}

char* NALPacketGroup::serialize()
{
	if (m_data && m_embeddedPackets.size() == 0)
		return 0;

	m_header.opCode = NAL_OP_PACKET_GROUP;
	//Find size and allocate dest
	int size = m_header.size + sizeof(NALPacketHeader);
	char* dest = new char[m_totalSize]; 

	//Attach the header
	memcpy(dest, &m_header, sizeof(NALPacketHeader));

	///Attach all the contained packages
	short pOffset = sizeof(NALPacketHeader);
	for (unsigned int i = 0; i < m_embeddedPackets.size(); i++)
	{
		memcpy(dest + pOffset, m_embeddedPackets[i].data, m_embeddedPackets[i].size);
		pOffset += m_embeddedPackets[i].size;
	}

	return dest;
}
