#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#include "csocket.h"

CSocketBase::CSocketBase(int domain, int type, int protocol)
: m_socketFd(-1)
{
    m_socketFd = socket(domain, type, protocol);
    if(0 > m_socketFd)
    {
        perror("create socket failed");
    }
}

CSocketBase::CSocketBase(int fd)
: m_socketFd(fd)
{
    ;
}

CSocketBase::~CSocketBase()
{
    close(m_socketFd);
}

CTcpSocket::CTcpSocket()
: CSocketBase(AF_INET, SOCK_STREAM, 0)
{
    ;
}

CTcpSocket::CTcpSocket(int fd)
: CSocketBase(fd)
{
    ;
}

int CTcpSocket::Send(const void *buf, size_t len, int flags)
{
    return send(m_socketFd, buf, len, flags);
}

int CTcpSocket::Recv(void *buf, size_t len, int flags)
{
    return recv(m_socketFd, buf, len, flags);
}

int CTcpSocket::Bind(const char* bindAddr, const int bindPort)
{
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(bindPort);
	if(inet_pton(AF_INET, bindAddr, &(servaddr.sin_addr)) <= 0)
	{
		return -1;
	}
    
    if(::bind(m_socketFd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind port failed!");
        return -1;
    }
    
    return 0;
}

int CTcpSocket::Listen(int backlog)
{
    if(::listen(m_socketFd, backlog) == -1)
    {
    	perror("listen failed");
    	return -1;
    }
    return 0;
}

CTcpSocket* CTcpSocket::Accept()
{
    int clientFd = ::accept(m_socketFd, NULL, NULL);

    if(clientFd < 0)
    {
        return NULL;
    }
    
    return new CTcpSocket(clientFd);
}

int CTcpSocket::Connect(const char* strServerAddr, const int serverPort)
{
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serverPort);
	if(inet_pton(AF_INET, strServerAddr, &(servaddr.sin_addr)) <= 0)
	{
		return -1;
	}
    
    return ::connect(m_socketFd, (struct sockaddr *)&servaddr, sizeof(servaddr));
}
