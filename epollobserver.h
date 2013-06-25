/*
 * epollobserver.h
 *
 *  Created on: 2013-6-8
 *      Author: joelliu
 */

#ifndef EPOLLOBSERVER_H_
#define EPOLLOBSERVER_H_

#include <map>
#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csocket.h"

using std::map;

//-----------------------------------------

class Epoller
{
public:
	virtual int InputNotify()=0;
	virtual int OutputNotify()=0;
	Epoller();
	virtual ~Epoller(){};
	int GetFd()
	{
		return m_fd;
	}
	struct epoll_event *GetEventPointer()
	{
		return &m_event;
	}
	void EnableInput(bool b=true);
	void DisableInput();
	void EnableOutput(bool b=true);
	void DisableOutput();

protected:
	void SetFd(int fd); //一个阑尾，子类必须调用此方法...

private:
	int m_fd;
	struct epoll_event m_event;

};

//EpollServer------------------------------------------------

class EpollServer
{
public:
	EpollServer(int maxPollers);
    ~EpollServer();

	int AddEpoller(Epoller* e);
    int DelEpoller(Epoller* e);

	void WaitAndProcess(int ms);

private:
	int m_epfd;
    int m_maxPollers;
	map<int,Epoller*> m_epollers;
	struct epoll_event *m_events;

};

//TCPConnector---------------------------------------------------
class TCPConnector:public Epoller
{
public:
	TCPConnector(CTcpSocket *socket, EpollServer *es);

protected:
    CTcpSocket *m_socket;
    EpollServer *m_epollServer;
};

//TCPListener---------------------------------------------------
class TCPListener:public Epoller
{
public:
	TCPListener(int port, EpollServer *es);
    virtual TCPConnector* GetConnector(CTcpSocket *ts)=0;
	int InputNotify();
    int OutputNotify();
protected:
	int m_serverFd;
	int m_serverPort;
	EpollServer *m_epollServer;

};

//-----------------------------------------------------




#endif /* EPOLLOBSERVER_H_ */
