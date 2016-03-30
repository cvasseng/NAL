/*
  
  Copyright (c) 2007, Chris V

  Licensed under the MIT license. See bundled LICENSE file for details.
  
*/

#ifndef nal_opcodes_h__
#define nal_opcodes_h__

/************************************************************************
 Contains the opcodes used by the system.                             
Note that all codes from [0x000..0x025] are reserved for the system!
*************************************************************************/


#define NAL_OP_NOOP					0x0000
#define NAL_OP_PACKET_GROUP			0x0001
#define NAL_OP_PACKET_KEEP_ALIVE	0x0002
#define NAL_OP_PACKET_TEXT			0x0003
#define NAL_OP_PACKET_IDENT			0x0005
#define NAL_OP_PACKET_AUTHORIZE		0x0006
#define NAL_OP_PACKET_AC_DENIED		0x0007
#define NAL_OP_SPAWN				0x0008
#define NAL_OP_DISCONNECT			0x0009

#endif // nal_opcodes_h__
