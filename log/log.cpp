#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include "log.h"

using namespace std;

Log::Log(){

}

Log::~Log(){
    if(m_queuebuffer!=nullptr){
        delete m_queuebuffer;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    }
}

void* Log::queue_to_file(){
    string t;
    while(m_queuebuffer->pop(t)){
        m_file_mutex.lock();
        m_file<<t<<endl;
        m_file_mutex.unlock();
    }
}

bool Log::init(const string& filename,int log_buf_size,int queue_size,int is_async){
    m_is_async=is_async;

    if(queue_size>=1&&m_is_async==true){
        
        m_queuebuffer=new QueueBuffer(queue_size);
        pthread_t tid;
        pthread_create(&tid,nullptr,queue_to_file_thread,nullptr);
    }
    m_log_buf_size=log_buf_size;
    m_log_buffer.reserve(m_log_buf_size);

    time_t t=time(nullptr);
    tm local_time=*localtime(&t);

    int pos=filename.find_last_of('/');
    string time_=to_string(local_time.tm_year+1900)+"_"+to_string(local_time.tm_mon+1)+"_"+to_string(local_time.tm_mday)+"_";
    string file_name=filename.substr(0,pos+1)+time_+filename.substr(pos+1);

    m_file.open(file_name,ios::out | ios::app);
    if(!m_file.is_open()){
        return false;
    }
    return true;
} 
template <typename... Args>
void Log::write_log(int level,const string& format,Args&&... args){
    timeval now={0,0};
    time_t t=time(nullptr);
    tm local_time=*localtime(&t);
    string time=to_string(local_time.tm_year+1900)+"-"+to_string(local_time.tm_mon+1)+"-"+to_string(local_time.tm_mday)+" "
    +to_string(local_time.tm_hour)+":"+to_string(local_time.tm_min)+":"+to_string(local_time.tm_sec)+"."+to_string(now.tv_usec);

    string s;
    switch (level)
    {
    case 0:
        s+="[debug]:";
        break;
    case 1:
        s+="[info]:";
        break;
    case 2:
        s+="[warn]:";
        break;
    case 3:
        s+="[erro]:";
        break;  
    default:
        s+="[info]:";
        break;
    }
    string message=format_string(format,forward<Args>(args)...);
    string buf=time+s+message;
    if(m_is_async&&!m_queuebuffer->is_full()){
        m_queuebuffer->push(buf);
    }
    else{
        m_file_mutex.lock();
        m_file<<s<<endl;
        m_file_mutex.unlock();
    }
}

template <typename... Args>
void Log::LOG_DEBUG(const string& format,Args&&... args){
    get_instance()->write_log(0,format,forward<Args>(args)...);
}

template <typename... Args>
void Log::LOG_INFO(const string& format,Args&&... args){
    get_instance()->write_log(1,format,forward<Args>(args)...);
}

template <typename... Args>
void Log::LOG_WARN(const string& format,Args&&... args){
    get_instance()->write_log(2,format,forward<Args>(args)...);
}

template <typename... Args>
void Log::LOG_ERROR(const string& format,Args&&... args){
    get_instance()->write_log(3,format,forward<Args>(args)...);
}

template <typename... Args>
string Log:: format_string(const string& format, Args&&... args){
    size_t size = snprintf(nullptr, 0, format.c_str(), forward<Args>(args)...) + 1;
    unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format.c_str(), forward<Args>(args)...);
    return string(buffer.get(), buffer.get() + size - 1);
}

