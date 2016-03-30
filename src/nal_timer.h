/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#ifndef nal_timer_h__
#define nal_timer_h__

#include "nal_system.h"

class NALTimer
{
public:
	///Constructor
	NALTimer();
	///Destructor
	~NALTimer();
	///Start timer
	void start();
	///Stop timer (this pauses the timer, so it resumes on play())
	void stop();
	///Reset the timer
	void reset();
	///Retreive the time
	unsigned int time();
protected:
	///Time offset
	unsigned int m_timeOffset;
	///Stop offset
	unsigned int m_stopOffset;
	///Is the timer running?
	bool m_isRunning;
private:
};

#endif // nal_timer_h__
