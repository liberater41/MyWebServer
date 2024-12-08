#ifndef THREADPOOL_H
#define THREADPOOL_H


#include <pthread.h>
#include <queue>
#include "../lock/locker.h"
#include "../mysql/sql_pool.h"

using namespace std;

template <typename T>
class threadpool{
public:
    static threadpool<T> *get_instance() {
        static threadpool<T> instance; 
        return &instance;
    }

    bool add_req(T *req);

    void init(int thread_num,int max_req);

private:
    threadpool();
    ~threadpool();

    static void* worker(void *arg);
    void run();

private:
    int m_thread_num;
    int m_max_req;
    pthread_t *m_threads;
    queue<T*> m_req_queue;
    int m_req_num;
    locker m_mutex;//队列互斥锁
    sem m_sem;//请求信号量
    Sql_pool *m_con_pool;

};


#endif