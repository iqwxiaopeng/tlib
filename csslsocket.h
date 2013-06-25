#ifndef SSL_SOCKET_H_
#define SSL_SOCKET_H_

#include <openssl/ssl.h>

#include "csocket.h"

class CSslSocket : public CTcpSocket
{
public:
    // new socket from nothing
	CSslSocket(SSL_CTX *sslCtx);
	CSslSocket(int fd,SSL_CTX *sslCtx);
    // new socket from established ssl connection
    CSslSocket(int fd, SSL_CTX *sslCtx, SSL *ssl);
	virtual ~CSslSocket();
    
    // CTcpSocket
	virtual int Send(const void *buf, size_t len, int flags);
	virtual int Recv(void *buf, size_t len, int flags);
    virtual int GetSslError() { return m_lastSslError; }
	virtual CSslSocket* Accept();
	virtual int Connect(const char* serverAddr, const int serverPort);

private:
    void SaveSslError(int ret);
    
protected:
    SSL_CTX *m_sslCtx;
    SSL *m_ssl;
    int m_lastSslError;
};

#endif /* SSL_SOCKET_H_ */
