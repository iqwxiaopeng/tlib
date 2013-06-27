#include <time.h>
#include <sys/timerfd.h>
#include "epollobserver.h"

class TimerEpoller:public Epoller
{
public:
    TimerEpoller(int afterMs, int intervalMs=0);
    virtual ~TimerEpoller();
    int InputNotify();
    int OutputNotify();
    virtual void TimerNotify() = 0;

private:
    int SetTime(int ms);
    int m_timerFd;
    int m_afterMs;
    int m_intervalMs;
};
