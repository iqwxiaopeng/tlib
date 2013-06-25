#ifndef SOCKET_WRAPPER_H_
#define SOCKET_WRAPPER_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

class CSocketBase
{
public:
	CSocketBase(int domain, int type, int protocol);
    CSocketBase(int fd);
	virtual ~CSocketBase();

	virtual int GetFd() const { return m_socketFd; }

protected:
	int m_socketFd;
};

class CTcpSocket : public CSocketBase
{
public:
    CTcpSocket();
    CTcpSocket(int fd);
	virtual ~CTcpSocket() {}

	virtual int Send(const void *buf, size_t len, int flags);
	virtual int Recv(void *buf, size_t len, int flags);

	// server side
	virtual int Bind(const char* bindAddr, const int bindPort);
	virtual int Listen(int backlog);
	virtual CTcpSocket* Accept();

	// client side
	virtual int Connect(const char* serverAddr, const int serverPort);
};

#endif /* SOCKET_WRAPPER_H_ */
