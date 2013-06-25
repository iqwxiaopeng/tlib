#include "epollobserver.h"
#include "csslsocket.h"
#include <openssl/ssl.h>
#include <string>
using std::string;

class SSLConnector:public TCPConnector
{
public:
    SSLConnector(CSslSocket* socket, EpollServer *es);
protected:
    CSslSocket* m_socket;
};

class SSLListener:public TCPListener
{
public:
    SSLListener(string crtPath, string keyPath, string passwd, int port, EpollServer *es);
    virtual int InputNotify();

protected:
    string m_crtPath;
    string m_keyPath;
    string m_passwd;
    SSL_CTX *m_sslCtx;
    CSslSocket* m_socket;
};

