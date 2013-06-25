#include "sslepoller.h"
#include "csslsocket.h"
#include <openssl/x509.h>
#include <openssl/rand.h>
#include <openssl/err.h>

SSLConnector::SSLConnector(CSslSocket* socket, EpollServer *es):TCPConnector(socket, es)
{
    m_socket = socket;
}

SSLListener::SSLListener(string crtPath, string keyPath, string passwd, int port, EpollServer *es):TCPListener(port, es),m_crtPath(crtPath),m_keyPath(keyPath),m_passwd(passwd),m_sslCtx(NULL)
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *ssl_method = SSLv23_server_method();
    m_sslCtx = SSL_CTX_new(ssl_method);
    SSL_CTX_set_verify(m_sslCtx,SSL_VERIFY_PEER,NULL);
    /*
    if(SSL_CTX_load_verify_locations(ssl_ctx, "../ca.crt", NULL) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(-1);
    }
    */
    SSL_CTX_set_default_passwd_cb_userdata(m_sslCtx, (void*)m_passwd.c_str());
    if (SSL_CTX_use_certificate_file(m_sslCtx, m_crtPath.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(-1);
    }
    if (SSL_CTX_use_PrivateKey_file(m_sslCtx, m_keyPath.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(-1);
    }
    if (!SSL_CTX_check_private_key(m_sslCtx))
    {
        printf("Private key does not match the certificate public key\n");
        exit(-1);
    }
    m_socket = new CSslSocket(m_serverFd,m_sslCtx);

}

int SSLListener::InputNotify()
{
    CSslSocket *ss = m_socket->Accept();
    SSLConnector *sc = (SSLConnector *)GetConnector(ss);
    m_epollServer->AddEpoller(sc);
    return ss->GetFd();
}


