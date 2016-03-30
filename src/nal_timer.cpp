/*
  
  Copyright (c) 2007, Chris V

  Licensed under the MIT license. See bundled LICENSE file for details.
  
*/

#include "nal_timer.h"

unsigned int NALTimer::time()
{
	return nal_getTime() - m_timeOffset;
}

NALTimer::NALTimer()
{
	m_timeOffset = 0;
	m_stopOffset = 0;
}

void NALTimer::start()
{
	m_isRunning = true;
	m_timeOffset = nal_getTime() - m_stopOffset;
}

void NALTimer::stop()
{
	m_isRunning = false;
	m_stopOffset = nal_getTime();
}
void NALTimer::reset()
{
	m_stopOffset = 0;
	m_timeOffset = nal_getTime();

}

NALTimer::~NALTimer()
{

}
