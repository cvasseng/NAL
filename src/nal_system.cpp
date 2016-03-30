/*
  
  Copyright (c) 2007, Chris V

  Licensed under the MIT license. See bundled LICENSE file for details.
  
*/

#include "nal_system.h"
#include "nal_enet.h"

#ifdef WIN32
    #include <windows.h>
    #include <MMSystem.h>
#endif

unsigned int nal_getTime()
{
#ifdef WIN32
	return timeGetTime();
#endif

#ifdef linux
    return time(NULL) * 1000;
#endif

}

NALHost* nal_createHost()
{
	return new NALHostEnet;
}
