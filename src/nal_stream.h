/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef nal_stream_h__
#define nal_stream_h__

#include "nal_packet.h"

#include <iostream>
#include <vector>

template <typename T>
class NALStream
{
public:
	NALPacket* buildPacket()
	{
		NALPacket* res = new NALPacket;
		short size = sizeof(T) * m_content.size();
		//res->data.data = new char[size];

		//std::cout << "Building packet of size " << size << std::endl;
		for (unsigned int i = 0; i < m_content.size(); i++)
		{
			T val = (*m_content[i]);
			//memcpy(&res->data.data + (i * sizeof(T)), &val, sizeof(T) );
		}	
		return res;
	}

	unsigned short serialize(char* dest)
	{

	}

	void operator<<(T &val)
	{
		m_content.push_back(&val);
	}
protected:
	///The contents of the stream
	std::vector<T*> m_content;
private:
};


#endif // nal_stream_h__
