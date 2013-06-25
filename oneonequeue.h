#ifndef __ONEONEQUEUE_H
#define __ONEONEQUEUE_H

#include <stdlib.h>
#include "tqueue.h"

template<typename T> class OneOneQueue : public TQueue<T>
{
public:
    //the real size is size-1
    OneOneQueue(int size=10001);
    int push(T item);
    T pop();
    int size();

private:
    T *m_ring;
    int m_bufSize;
    int m_begin;
    int m_end;
};

template<typename T> OneOneQueue<T>::OneOneQueue(int size) : m_bufSize(size), m_begin(0), m_end(0)
{
    m_ring = new T[size];
}

template<typename T> int OneOneQueue<T>::push(T item)
{
    int save_end = m_end;
    m_ring[save_end] = item;
    m_end = (save_end+1)%m_bufSize;
    if (m_end == m_begin)
    {
        m_end = save_end;
        return QOP_FULL; //ring is full
    }
    return QOP_SUCC;
}

template<typename T> T OneOneQueue<T>::pop()
{
    T save_tp = m_ring[m_begin];
    if (m_begin == m_end)
    {
        return NULL; //ring is empty
    }
    m_begin = (m_begin+1)%m_bufSize;
    return save_tp;
}

template<typename T> int OneOneQueue<T>::size()
{
    int size = m_end-m_begin;
    return (size<0)?(size+m_bufSize):size;
}

#endif
