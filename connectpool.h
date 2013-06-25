#ifndef __CONNECTPOOL_H
#define __CONNECTPOOL_H

#include <vector>
#include <queue>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tqueue.h"

using std::vector;
using std::queue;
using std::string;

const int BUFFER_SIZE = 2048;

class ConnectPool
{
public:
    ConnectPool(int queueKind = 0);
    int getMaxFd();

    void addReadfd(int fd);
    void delReadfd(int fd);
    
    void setReadSet(fd_set fdset);
    fd_set getReadSet();

    void setReadySet(fd_set fdset);
    fd_set getReadySet();

    void addClientFd(int fd);
    void addServerFd(int fd);
    
    bool checkServer();
    bool checkClients();

    string* getMessage();
    void dumpClientMsg();

public:
    fd_set m_ready_set;
    int    m_nready;
private:
    int m_maxfd;
    fd_set m_read_set;
    int m_serverfd;
    vector<int> m_clientfd;
    TQueue<string*>  *m_client_msg;
};







inline ConnectPool::ConnectPool(int queueKind) : m_maxfd(-1), m_nready(0), m_serverfd(-1)
{
    FD_ZERO(&m_read_set);
    FD_ZERO(&m_ready_set);
    if (0 == queueKind)
    {
        m_client_msg = new OneQueue<string*>();
    } 
    else if (1 == queueKind) 
    {
        m_client_msg = new OneOneQueue<string*>();
    }
    else
    {
        m_client_msg = new OneQueue<string*>();
    }
}

inline int ConnectPool::getMaxFd()
{
    return m_maxfd;
}

inline void ConnectPool::addReadfd(int fd)
{
    if (fd > m_maxfd)
    {
        m_maxfd = fd;
    }        

    FD_SET(fd, &m_read_set);
}

inline void ConnectPool::delReadfd(int fd)
{
    if (fd == m_maxfd)
    {
        int max_cfd = -1;
        for (int i=0; i<m_clientfd.size(); ++i)
        {
            if (m_clientfd[i]>max_cfd)
            {
                max_cfd = m_clientfd[i];
            }
        }
        m_maxfd = (m_serverfd>max_cfd)?m_serverfd:max_cfd;
    }        

    FD_CLR(fd, &m_read_set);
}

inline void ConnectPool::setReadSet(fd_set fdset)
{
    m_read_set = fdset;
}

inline fd_set ConnectPool::getReadSet()
{
    return m_read_set;
}

inline void ConnectPool::setReadySet(fd_set fdset)
{
    m_ready_set = fdset;
}

inline fd_set ConnectPool::getReadySet()
{
    return m_ready_set;
}

inline void ConnectPool::addClientFd(int fd)
{
    m_clientfd.push_back(fd);
    addReadfd(fd);
}

inline void  ConnectPool::addServerFd(int fd)
{
    m_serverfd = fd;
    addReadfd(fd);
}

inline bool ConnectPool::checkServer()
{
    bool flag = false;
    if ((m_nready>0) && FD_ISSET(m_serverfd, &m_ready_set))
    {
        socklen_t clientlen = sizeof(struct sockaddr_in);
        struct sockaddr_in clientaddr; 
        int clientfd = accept(m_serverfd, (struct sockaddr*)&clientaddr, &clientlen);
        addClientFd(clientfd);
        --m_nready;
        flag = true;
    }
    return flag;
}

inline string* ConnectPool::getMessage()
{
    return m_client_msg->pop();
}

#endif
