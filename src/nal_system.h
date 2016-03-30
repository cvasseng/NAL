/*
  
  Copyright (c) 2007, Chris V

  Licensed under the MIT license. See bundled LICENSE file for details.
  
*/

#ifndef nal_system_h__
#define nal_system_h__

#include "nal_base.h"

unsigned int nal_getTime();

NALHost* nal_createHost();

template <class Ta>
Ta* nal_cast(const NALPacketHeader &packet, void* data)
{
	if (sizeof(Ta) == packet.size)
	{
		return (Ta*)data;
	}
	//std::cout << "ERROR IN nal_cast<T>(header, data) : SIZE MIS-MATCH!" << std::endl;
	return 0;
}


#endif // nal_system_h__
