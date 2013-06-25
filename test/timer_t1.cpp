/*
 * timer_t1.cpp
 *
 *  Created on: 2013-6-3
 *      Author: joelliu
 */

#include<iostream>
#include"../timer.h"

using std::cout;
using std::endl;

class Timer1: public Timer
{
public:
	void timerNotify()
	{
		cout << "Timer1 is Notifyed" << endl;
		this->timerCreate(1000);
	}
};

class Timer2: public Timer
{
public:
	void timerNotify()
	{
		cout << "Timer2 is Notifyed" << endl;
		this->timerCreate(10000);
	}
};

int main()
{
	Timer1* t1 = new Timer1();
	Timer2* t2 = new Timer2();
	t1->timerCreate(100);
	t2->timerCreate(10000);
	while(1)
	{
		pause();
	}

}


