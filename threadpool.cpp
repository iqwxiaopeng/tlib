#include <stdio.h>
#include <stdlib.h>
#include "threadpool.h"

ThreadPool::ThreadPool(int threads) : m_threads(threads), m_tasks(0), m_taskq(NULL), m_tids(NULL), m_shutdown(false)
{
    pthread_mutex_init (&(m_qlock), NULL);
    pthread_cond_init (&(m_qready), NULL);

    m_tids =  (pthread_t*)malloc(threads*sizeof(pthread_t));
    for (int i=0; i<threads; ++i)
    {
        pthread_create (&m_tids[i], NULL, &threadRoutine, this);
    }
}

void* ThreadPool::threadRoutine(void *arg)
{
    ThreadPool *pthis = (ThreadPool*)arg;
    while (1)
    {
        pthread_mutex_lock (&pthis->m_qlock);
        while ((pthis->m_tasks== 0) && (!pthis->m_shutdown))
        {
            pthread_cond_wait(&pthis->m_qready, &pthis->m_qlock);
        }

        if(pthis->m_shutdown)
        {
            pthread_mutex_unlock (&pthis->m_qlock);
            pthread_exit(NULL);
        }

        --pthis->m_tasks;
        Task *task = pthis->m_taskq;
        pthis->m_taskq = pthis->m_taskq->next;
        pthread_mutex_unlock (&pthis->m_qlock);

        (*(task->process)) (task->arg);
        free (task);
        task = NULL;
    }

   /*这一句应该是不可达的*/
    pthread_exit (NULL);
}

int ThreadPool::addTask(void *(*process) (void *arg), void *arg)
{
    /*构造一个新任务*/
     Task *newtask = (Task*) malloc(sizeof(Task));
     newtask->process = process;
     newtask->arg = arg;
     newtask->next = NULL;

     pthread_mutex_lock (&m_qlock);
    /*将任务加入到等待队列中*/
     Task *head= m_taskq;
     if (head!= NULL)
     {
        while (head->next != NULL)
             head= head->next;
        head->next = newtask;
     }
    else
    {
        m_taskq = newtask;
    }

    ++m_tasks;
    pthread_mutex_unlock (&m_qlock);
    pthread_cond_signal (&m_qready);
    return 0;
}


ThreadPool::~ThreadPool()
{
    m_shutdown = true;
    pthread_cond_broadcast (&m_qready);

    /*阻塞等待线程退出，否则就成僵尸了*/
    for (int i=0; i<m_threads; i++)
    {
         pthread_join (m_tids[i], NULL);
    }
    free(m_tids);

    /*销毁等待队列*/
    while (m_taskq!= NULL)
    {
        Task *head = m_taskq;
        m_taskq = m_taskq->next;
        free(head);
    }

    /*条件变量和互斥量也别忘了销毁*/
    pthread_mutex_destroy(&m_qlock);
    pthread_cond_destroy(&m_qready);
}
