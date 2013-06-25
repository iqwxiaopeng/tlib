#include <stdio.h>
#include <string.h>
#include"timer.h"

void Timer::timerCreate(int ms)
{
	timer_t timerid;
	struct sigevent evp;
	memset(&evp, 0, sizeof(struct sigevent));

	evp.sigev_value.sival_ptr = (void *)this;
	evp.sigev_notify = SIGEV_THREAD;
	evp.sigev_notify_function = &notify;

	if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1)
	{
		perror("fail to timer_create");
	}

	struct itimerspec it;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 0;
	it.it_value.tv_sec = ms/1000;
	it.it_value.tv_nsec = (ms%1000)*1000000;

	if (timer_settime(timerid, 0, &it, NULL) == -1)
	{
		perror("fail to timer_settime");
	}
}

void Timer::notify(union sigval v)
{
	Timer *pthis = (Timer*)v.sival_ptr;
	pthis->timerNotify();
}


