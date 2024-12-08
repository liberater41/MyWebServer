//使用deque实现
#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <deque>
#include <string>
#include <iostream>
#include "../lock/locker.h"
using namespace std;

class QueueBuffer {
public:
    QueueBuffer(size_t max_size) : m_max_size(max_size),m_size(0) {}

    
    bool push(const string& item) {
        m_mutex.lock();
        if (m_size >= m_max_size) {
            m_cond.broadcast();
            m_mutex.unlock();
            return false; 
        }
        m_size++;
        buffer.push_back(item);

        m_cond.broadcast();
        m_mutex.unlock(); 
        return true; 
    }

    bool pop(string& item) {
        m_mutex.lock();
        while(m_size<=0){
            if (!m_cond.wait(m_mutex.get())) {
                m_mutex.unlock();
                return false;
            }
        }

        string p_item = buffer.front();
        buffer.pop_front();
        m_size--;
        m_mutex.unlock(); 
        item= p_item;
        return true;
    }

    int get_size(){
        int t=0;
        m_mutex.lock();
        t=m_size;
        m_mutex.unlock();
        return t;
    }

    bool empty() {
        m_mutex.lock();
        if(m_size==0){
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }

    bool is_full() {
        m_mutex.lock();
        if(m_size>=m_max_size){
            m_mutex.unlock();
            return true;
        }
        m_mutex.unlock();
        return false;
    }


private:
    deque<string> buffer; 
    size_t m_max_size;
    size_t m_size; 

    locker m_mutex;
    cond m_cond;               
};

#endif