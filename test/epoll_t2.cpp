/*
 * epoll_t2.cpp
 *
 *  Created on: 2013-6-8
 *      Author: joelliu
 */

#include "../epollobserver.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

class Client:public TCPConnector
{
public:
	Client(CTcpSocket *ts, EpollServer *es):TCPConnector(ts, es)
	{}
	int InputNotify()
	{
		char buf[255];
		int length = m_socket->Recv(buf, 255, 0);
		if (length < 0){
		   printf( "error comes when recieve data from server !" );
		}
        else if(length > 0)
        {
		    printf( "from client: %s\n", buf);

            long timestamp;
            strcpy(buf,"welcome");
            timestamp = time(NULL);
            strcat(buf, " timestamp in server: " );
            strcat(buf,ctime(&timestamp));

            m_socket->Send(buf,255, 0 );
        }
        else
        {
            printf("client disconect\n");
            m_epollServer->DelEpoller(this);
            delete this;
        }
	    return 0;

	}
	int OutputNotify()
	{
		return 0;
	}
};

class Server:public TCPListener
{
public:
	Server(int port, EpollServer *es):TCPListener(port, es)
	{}
    TCPConnector *GetConnector(CTcpSocket *ts)
    {
        return new Client(ts, m_epollServer);
    }
    int InputNotify()
    {
        int clifd = TCPListener::InputNotify();
        if (clifd == -1)
        {
            getchar();
        }
        return clifd;
    }
    
};

int main()
{
	EpollServer *es = new EpollServer(400000);
	Server *s = new Server(8175,es);
    es->AddEpoller(s);
    while(1)
    {
        es->WaitAndProcess(5000);
    }
}


