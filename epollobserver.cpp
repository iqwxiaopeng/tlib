/*
 * epollobserver.cpp
 *
 *  Created on: 2013-6-8
 *      Author: joelliu
 */

#include "epollobserver.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Epoller::Epoller():m_fd(-1)
{
	bzero(&m_event, sizeof(struct epoll_event));
}

void Epoller::EnableInput(bool b)
{
	if(b)
		m_event.events |= EPOLLIN;
	else
		m_event.events &= ~EPOLLIN;
}

void Epoller::DisableInput()
{
	m_event.events &= ~EPOLLIN;
}

void Epoller::EnableOutput(bool b)
{
	if(b)
		m_event.events |= EPOLLOUT;
	else
		m_event.events &= ~EPOLLOUT;
}

void Epoller::DisableOutput()
{
	m_event.events &= ~EPOLLOUT;
}

void Epoller::SetFd(int fd)
{
	m_fd = fd;
	m_event.data.fd = fd;
}

//EpollServer------------------------------------------

EpollServer::EpollServer(int maxPollers):m_epfd(-1)
{
    m_maxPollers = maxPollers;
	if ((m_epfd = epoll_create(m_maxPollers)) == -1)
	{
		perror("epoll_create error");
	}
	m_events = (struct epoll_event*)calloc(m_maxPollers, sizeof(struct epoll_event));
}

EpollServer::~EpollServer()
{
    close(m_epfd);
}


int EpollServer::AddEpoller(Epoller* e)
{
	m_epollers[e->GetFd()] = e;
	return epoll_ctl(m_epfd, EPOLL_CTL_ADD, e->GetFd(), e->GetEventPointer());
}

int EpollServer::DelEpoller(Epoller* e)
{
	m_epollers.erase(e->GetFd());
	return epoll_ctl(m_epfd, EPOLL_CTL_DEL, e->GetFd(), e->GetEventPointer());
}

void EpollServer::WaitAndProcess(int ms)
{
	int numEvents = epoll_wait (m_epfd, m_events, m_maxPollers, ms);
	for(int i=0; i<numEvents; ++i)
	{
		if(m_events[i].events & EPOLLIN)
		{
			m_epollers[m_events[i].data.fd]->InputNotify();
		}
		if(m_events[i].events & EPOLLOUT)
		{
			m_epollers[m_events[i].data.fd]->OutputNotify();
		}
	}
}

//TCPConnector--------------------------------------------

TCPConnector::TCPConnector(CTcpSocket *socket, EpollServer *es)
{
    m_socket = socket;
	SetFd(m_socket->GetFd());
    m_epollServer = es;
	EnableInput();
	//EnableOutput();
}


//TCPListener---------------------------------------------
TCPListener::TCPListener(int port, EpollServer *es):m_serverPort(port),m_epollServer(es)
{
	if ((m_serverFd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 )
	{
	   perror(" create socket error" );
	   exit( 1 );
	}
	SetFd(m_serverFd);

	struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof (servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(m_serverPort);
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);

	if (bind(m_serverFd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
	   printf( "bind to port %d failure!\n" ,m_serverPort);
	   exit( 1 );
	}

	if (listen(m_serverFd,10) <  0 )
	{
	   printf( " call listen failure!\n " );
	   exit( 1 );
	}

	EnableInput();

}

int TCPListener::InputNotify()
{
	struct sockaddr_in cliaddr;
	socklen_t length =  sizeof (cliaddr);
	int clifd = accept(m_serverFd,(struct sockaddr*) &cliaddr, &length);
	if (-1 == clifd)
	{
		perror("Server accept error");
        return -1;
	}
    CTcpSocket *ts = new CTcpSocket(clifd);
    TCPConnector *tc = GetConnector(ts);
    m_epollServer->AddEpoller(tc);
	return clifd;
}

int TCPListener::OutputNotify()
{
    DisableInput();
    return 0;
}

