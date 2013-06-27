/*
 * timerserver.h
 *
 *  Created on: 2013-6-3
 *      Author: joelliu
 */

#ifndef TIMER_H_
#define TIMER_H_

#include<time.h>


class Timer
{
public:
    Timer();
    int timeDiff();
	virtual void timerNotify()=0;
	void timerCreate(int ms);
	virtual ~Timer(){}
private:
    static void notify(union sigval v);
    int setBegin();
    int setEnd();
    struct timeval m_begin;
    struct timeval m_end;
};


#endif
