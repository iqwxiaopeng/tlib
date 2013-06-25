#ifndef __TQUEUE_H
#define __TQUEUE_H

template<typename T> class TQueue;
const int QOP_SUCC =  0;
const int QOP_FULL = -1;

#include "onequeue.h"
#include "oneonequeue.h"

template<typename T> class TQueue
{
public:
    virtual int push(T item) = 0;
    virtual T pop() = 0;
    virtual int size() = 0;
};

#endif
