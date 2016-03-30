/*
  
  Copyright (c) 2007, Chris V

  Licensed under the MIT license. See bundled LICENSE file for details.
  
*/

#ifndef nal_h__
#define nal_h__

#include "nal_enet.h"
#include "nal_base.h"

#include "nal_opcodes.h"
#include "nal_packet.h"
#include "nal_system.h"

#ifdef WIN32
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "NAL.lib")
#endif

#endif // nal_h__
