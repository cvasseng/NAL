/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef nal_packetQueue_h__
#define nal_packetQueue_h__

#include <vector>
#include <map>

#include "nal_packet.h"

class NALHost;
class NALPacketQueue
{
	friend class NALBase;
public:
	///Constructor
	NALPacketQueue();
	///Pushes a packet onto the queue
	void push(NALPacket *packet);
	///Is it empty?
	bool isEmpty();
	///Returns the size of the queue
	unsigned int size();
	//Builds a list of all the packets to dispatch
	void buildPacketList(std::vector<NALPacket*> &dest);
	///Clears the queue
	void clear();
protected:
	///The actual data
	std::vector<NALPacket*> m_queue;

	///The actual queue (he he)
	std::map<std::string, NALPacketGroup*> m_packetQueue;


private:
};

#endif // nal_packetQueue_h__
