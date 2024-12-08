#include "threadpool.h"
#include "../log/log.h"

using namespace std;

template <typename T>
void threadpool<T>::init(int thread_num,int max_req){
    m_thread_num=thread_num;
    m_max_req=max_req;
    m_threads=new pthread_t[thread_num];
    if(m_threads==nullptr){
        Log::LOG_ERROR("thread create error!");
        exit(1);
    }
    for(int 1=0;i<thread_num;i++){
        if(pthread_create(&m_threads[i],nullptr,worker,this)!=0){
            Log::LOG_ERROR("thread create error!");
            exit(1);
        }
        if(pthread_detach(m_threads[i])){
            Log::LOG_ERROR("thread create error!");
            exit(1);            
        }
        
    }
}

template <typename T>
threadpool<T>::threadpool(){
    m_req_num=0;

    m_sem(m_req_num);
}


template <typename T>
threadpool<T>::~threadpool(){
    delete[] m_threads;
}

template <typename T>
bool threadpool<T>::add_req(T *req){
    m_mutex.lock();
    if(m_req_num>=m_max_req){
        m_mutex.unlock();
        return false;

    }
    m_req_queue.push(req);
    m_req_num++;
    m_mutex.unlock();
    m_sem.post();
    return true;
}

template <typename T>
void *threadpool<T>::worker(void *arg){
    threadpool *pool=(threadpool *)arg;
    pool->run();
}

template <typename T>
void threadpool<T>::run(){
    while(true){
        m_sem.wait();
        m_mutex.lock();
        T *req=m_req_queue.front();
        m_req_queue.pop();
        m_mutex.unlock();
        req->process();

    }


}