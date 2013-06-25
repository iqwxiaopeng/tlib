#ifndef __ONEQUEUE_H
#define __ONEQUEUE_H

#include <queue>
#include "tqueue.h"
using std::queue;

template<typename T> class OneQueue : public TQueue<T>
{
public:
    int push(T item)
    {
        m_queue.push(item);
        return QOP_SUCC;
    }

    T pop()
    {
        if (!m_queue.empty())
        {
            T tmp = m_queue.front();
            m_queue.pop();
            return tmp;
        }
        else
        {
            return NULL;
        }
    }

    int size()
    {
        return (int)m_queue.size();
    }
private:
    queue<T> m_queue;
};

#endif
