#include <iostream>
#include "connectpool.h"

using std::cout;
using std::endl;

bool ConnectPool::checkClients()
{
    bool flag = false;
    for (int i = 0; (i < m_clientfd.size()) && (m_nready > 0); i++)
    {
        int connfd = m_clientfd[i];
        char buf[BUFFER_SIZE];
        if (FD_ISSET(connfd, &m_ready_set))
        {
            --m_nready;
            int length = recv(connfd, buf, BUFFER_SIZE, 0);
            if (length == -1)
            {
                cout << "error occurred when checkClients()" << endl;
            }
            else if (length == 0)
            {
                close(connfd);
                m_clientfd.erase(m_clientfd.begin()+i);
                delReadfd(connfd);
                break;
            }
            string *item = new string(buf);
            m_client_msg->push(item);
            flag = true;
        }
    }
    return flag;
}

void ConnectPool::dumpClientMsg()
{
    cout << "client msg size : " << m_client_msg->size() << endl;
    while (1)
    {
        string *item = m_client_msg->pop();
        if (NULL == item)
        {
            break; // empty
        }
        cout << "\t" << *item << endl;
        delete item;
    }
}
