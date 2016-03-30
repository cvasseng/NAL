/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef nal_packet_h__
#define nal_packet_h__


#include "nal_opcodes.h"
#include <vector>

class NALPeer;

///Delivery strategy
enum NALDeliveryStrategy
{
	///Ensures that the receiver actually receives the packet
	dsRELIABLE,
	///Packet is not guaranteed to reach the peer
	dsUNRELIABLE,
};

///The actual data for a packet
struct NALPacketHeader
{
	///The type of data encoded in the packet
	unsigned short opCode;
	///The size of the data contained in the packet, in bytes
	unsigned short size;
	///Broadcast the message to other connected peers?
	bool broadcast;
};

//A network packet. :)
/**Only the packet header will be transmitted, the rest is for handling the packet locally.

Packets are grouped together, so that instead of sending several small packets,
one (or more if the packet size exceeds the max packet size) big packets will be sent.
This is more optimal for bandwidth.*/
class NALPacket
{

public:
	///Constructor
	NALPacket();
	///Destructor
	virtual ~NALPacket();
	///Sets the packet priority
	void setPriority(float pri);
	///Get the priority
	const float& getPriority();
	///Sets the op code for the packet - Fails if this is a group packet (NAL_OP_PACKET_GROUP is then forced)
	void setOpCode(const unsigned short& code);
	///Get the packet op code
	unsigned short getOpCode();
	///Sets the broadcast flag
	void setBroadcastFlag(bool flag);
	///Gets the broadcast flag
	bool getBroadcastFlag();
	///Set reliability mode. default is dsUNRELIABLE
	void setDeliveryModel(NALDeliveryStrategy model);
	///Get the delivery model
	NALDeliveryStrategy getDeliveryModel();
	///Attaches data to the packet - fails if this packet is a group packet (NAL_OP_PACKET_GROUP)
	template<class Ta>
	void attachData(Ta* data)
	{
		m_header.size = sizeof(Ta);
		m_data = (char*)data;
		m_totalSize = m_header.size + sizeof(NALPacketHeader);
	}
	template<class Ta>
	void attachData(Ta& data)
	{
		m_header.size = sizeof(Ta);
		m_data = (char*)new Ta(data);
		m_totalSize = m_header.size + sizeof(NALPacketHeader);
	}
	///Attaches data to the packet - fails if this packet is a group packet (NAL_OP_PACKET_GROUP)
	void attachData(void* data, unsigned int size);

	///Serialize the packet
	virtual char* serialize();
	///Get the size of the package in bytes
	short size();
	///Is this packet unique?
	void setUnique(bool flag);
	///Get unique state (can only be one of this packet type in the queue if unique)
	bool getUnique();
	///Set the packet destination
	void setDestination(NALPeer* peer);
	///Get the packet destination
	NALPeer* getDestination();
protected:
	///The priority of the packet
	/**A priority of 1.0 guarantees that the packet will be sent during the next transmission phase.
	It does *NOT* however, gurantees that the packet will be received (use delivery var for that)*/
	float m_priortiy;
	///Delivery strategy
	NALDeliveryStrategy m_deliveryStrat;
	///Time packet was issued
	unsigned int m_issueTime;
	///The packet data
	NALPacketHeader m_header;
	///The actual data
	char* m_data;
	///Total size
	short m_totalSize;
	///Can there only be one of this packet in the queue`? (determined by opcode)
	bool m_onlyOne;
	///The packet destination
	NALPeer* m_destination;
};

///A group of packets
/************************************************************************
Contains a group of packets. Behaves the same as a single packet as
far as the end-user is concerned.

Mechanics in cNALNetworkConnection ensures that all the packets
are extracted properly, and each extracted packets generates a call to
the ReceivedPacket callback. In other words, the end-user need not do anything
that he's not already doing (i.e., catching single packets)

Grouping packets should happen automaticly through the packet queue.
Packets with similar priority and broadcast flags get grouped together to
avoid sending a bunch of small packets all the time.
*************************************************************************/
class NALPacketGroup : public NALPacket
{
	///Contains an embedded serialized packet
	struct NALEmbeddedPacket
	{
		char* data;
		short size;
	};
public:
	///Attaches a packet to the packet
	/**If another packet (or several other packets) are attached to this packet,
	this packet becomes a group packet, identified by NAL_OP_PACKET_GROUP.
	The packet handling routines automatically extracts all the embedded packets,
	and calls the receivedPacket callback for each of them.
	Note that when the packet is a packet group, the header size attribute
	contains the number of embedded packets, and NOT the size of the packet.
	(since the size can be pretty much anything..)*/
	void attachPacket(NALPacket* packet);
	///Serializes/flattens/marshals the packet into a bitstream
	char* serialize();
	///Destructor
	virtual ~NALPacketGroup(){}
protected:
	///The embedded packets
	std::vector<NALEmbeddedPacket> m_embeddedPackets;
private:
};

#endif // nal_packet_h__
