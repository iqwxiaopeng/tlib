#include <stdlib.h>
#include <arpa/inet.h>
 
#include "csslsocket.h"

CSslSocket::CSslSocket(SSL_CTX *sslCtx)
: CTcpSocket(),
  m_sslCtx(sslCtx), m_ssl(NULL), m_lastSslError(0)
{
    ;
}

CSslSocket::CSslSocket(int fd, SSL_CTX *sslCtx)
: CTcpSocket(fd),
  m_sslCtx(sslCtx), m_ssl(NULL), m_lastSslError(0)
{
    ;
}

CSslSocket::CSslSocket(int fd, SSL_CTX *sslCtx, SSL *ssl)
: CTcpSocket(fd),
  m_sslCtx(sslCtx), m_ssl(ssl), m_lastSslError(0)
{
    ;
}

CSslSocket::~CSslSocket()
{
    SSL_shutdown(m_ssl);
    SSL_free(m_ssl);
    m_ssl = NULL;
}

int CSslSocket::Send(const void *buf, size_t len, int flags)
{
    int ret = SSL_write(m_ssl, buf, len);
    if(0 >= ret)
    {
        SaveSslError(ret);
    }
    return ret;
}

int CSslSocket::Recv(void *buf, size_t len, int flags)
{
    int ret = SSL_read(m_ssl, buf, len);
    if(0 >= ret)
    {
        SaveSslError(ret);
    }
    return ret;
}

CSslSocket* CSslSocket::Accept()
{
    int clientFd = ::accept(m_socketFd, NULL, NULL);

    if(clientFd < 0)
    {
        return NULL;
    }
    SSL *ssl = SSL_new(m_sslCtx);
    if(NULL == ssl)
    {
        close(clientFd);
        return NULL;
    }
    if(1 != SSL_set_fd(ssl, clientFd))
    {
        close(clientFd);
        return NULL;
    }
    int ret = SSL_accept(ssl);
    if(1 != ret)
    {
        SaveSslError(ret);
        close(clientFd);
        return NULL;
    }
    
    return new CSslSocket(clientFd, m_sslCtx, ssl);
}

int CSslSocket::Connect(const char* strServerAddr, const int serverPort)
{
    int ret = CTcpSocket::Connect(strServerAddr, serverPort);
    if(ret != 0)
    {
        return -1;
    }
    m_ssl = SSL_new(m_sslCtx);
    if(NULL == m_ssl)
    {
        close(m_socketFd);
        return -1;
    }
    if(1 != SSL_set_fd(m_ssl, m_socketFd))
    {
        close(m_socketFd);
        return -1;
    }
    ret = SSL_connect(m_ssl);
    if(1 != ret)
    {
        SaveSslError(ret);
        close(m_socketFd);
        return -1;
    }
    
    return 0;
}

void CSslSocket::SaveSslError(int ret)
{
    m_lastSslError = SSL_get_error(m_ssl, ret);
}
