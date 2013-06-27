#include "timerepoller.h"

TimerEpoller::TimerEpoller(int afterMs, int intervalMs):m_afterMs(afterMs), m_intervalMs(intervalMs)
{
    m_timerFd = timerfd_create(CLOCK_REALTIME, 0);
    SetFd(m_timerFd);
    EnableInput();
    SetTime(m_afterMs);
}

TimerEpoller::~TimerEpoller()
{
    close(m_timerFd);
}

int TimerEpoller::SetTime(int ms)
{
    struct itimerspec ts;
    int ret = 0;
    bzero(&ts,sizeof(ts));
    ts.it_value.tv_sec = ms/1000;
    ts.it_value.tv_nsec = (ms%1000)*1000000;
    if ((ret = timerfd_settime(m_timerFd, 0, &ts, NULL)) == -1) 
    {
        perror("TimerEpoller::SetTime");
    }
    return ret;

}

int TimerEpoller::InputNotify()
{
    SetTime(m_intervalMs);
    TimerNotify();
    return 0;
}

int TimerEpoller::OutputNotify()
{
    DisableOutput();
    return 0;
}
