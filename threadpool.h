#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <pthread.h>

typedef struct task Task;

class ThreadPool
{
public:
    ThreadPool(int threads=1);

    int addTask(void *(*process) (void *arg), void *arg);

    virtual ~ThreadPool();

private:
    static void *threadRoutine (void *arg);

private:
    int m_threads;
    int m_tasks;
    Task *m_taskq;
    pthread_t *m_tids;
    bool m_shutdown;
    pthread_mutex_t m_qlock;
    pthread_cond_t m_qready;
    
};

struct task
{
    /*回调函数，任务运行时会调用此函数*/
    void *(*process) (void *arg);
    void *arg;/*回调函数的参数*/
    struct task *next;

};

#endif
