/*
 * epoll_t2.cpp
 *
 *  Created on: 2013-6-8
 *      Author: joelliu
 */

#include "../sslepoller.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

class Client:public SSLConnector
{
public:
	Client(CSslSocket* socket, EpollServer *es):SSLConnector(socket, es)
	{
    }
	int InputNotify()
	{
		char buf[255];
		int length = m_socket->Recv(buf, 255,0);
        if(length > 0)
        {
		    printf( "from client: %s\n", buf);

            long timestamp;
            strcpy(buf,"welcome");
            timestamp = time(NULL);
            strcat(buf, " timestamp in server: " );
            strcat(buf,ctime(&timestamp));

            m_socket->Send(buf,255,0);
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

class Server:public SSLListener
{
public:
	Server(string crtPath, string keyPath, string passwd, int port, EpollServer *es):SSLListener(crtPath, keyPath, passwd, port, es)
	{}
    TCPConnector *GetConnector(CTcpSocket *ts)
    {
        return new Client((CSslSocket*)ts, m_epollServer);
    }
};

int main()
{
	EpollServer *es = new EpollServer(400000);
	Server *s = new Server(string("../../algorithm/20130609cppSSL/server.crt"),string("../../algorithm/20130609cppSSL/server.key"),string("lj8175"),8175,es);
    es->AddEpoller(s);
    while(1)
    {
        es->WaitAndProcess(5000);
    }
}


