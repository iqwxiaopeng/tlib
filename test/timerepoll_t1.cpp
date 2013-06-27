#include "../epollobserver.h"
#include "../timerepoller.h"
#include <stdio.h>

class Timer:public TimerEpoller
{
public:
	Timer(int afterMs, int intervalMs=0): TimerEpoller(afterMs, intervalMs)
	{
    }
    void TimerNotify()
    {
        printf("m_int : %d\n", m_int++);
    }
private:
    int m_int;
    
};

int main()
{
	EpollServer *es = new EpollServer(400000);
	Timer *t = new Timer(1,60);
	//Timer *t = new Timer(1000);
    es->AddEpoller(t);
    while(1)
    {
        es->WaitAndProcess(5000);
    }
}


