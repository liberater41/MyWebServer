#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include "block_queue.h"
#include <memory>

using namespace std;
class Log{
public:
    static Log* get_instance(){
        static Log instance;
        return &instance;
    }

    bool init(const string& filename,int log_buf_size,int queue_size,int is_async);

    static void write_log(int level,string message);

    template <typename... Args>
    static void LOG_DEBUG(const string& format,Args&&... args);

    template <typename... Args>
    static void LOG_INFO(const string& format,Args&&... args);

    template <typename... Args>
    static void LOG_WARN(const string& format,Args&&... args);

    template <typename... Args>
    static void LOG_ERROR(const string& format,Args&&... args);



    static void* queue_to_file_thread(void *){
        get_instance()->queue_to_file();
        return nullptr;
    };
    void queue_to_file();

private:
    Log();
    ~Log();

    template <typename... Args>
    static string format_string(const string& format, Args&&... args);

private:
    string m_filename;
    int m_log_buf_size;
    QueueBuffer *m_queuebuffer;
    string m_log_buffer;
    fstream m_file;
    bool m_is_async;
    locker m_file_mutex;

};

template <typename... Args>
void Log::LOG_DEBUG(const string& format,Args&&... args){
    string message=format_string(format,forward<Args>(args)...);
    get_instance()->write_log(0,message);
}

template <typename... Args>
void Log::LOG_INFO(const string& format,Args&&... args){
    string message=format_string(format,forward<Args>(args)...);
    get_instance()->write_log(1,message);
}

template <typename... Args>
void Log::LOG_WARN(const string& format,Args&&... args){
    string message=format_string(format,forward<Args>(args)...);
    get_instance()->write_log(2,message);
}

template <typename... Args>
void Log::LOG_ERROR(const string& format,Args&&... args){
    string message=format_string(format,forward<Args>(args)...);
    get_instance()->write_log(3,message);
}

template <typename... Args>
string Log:: format_string(const string& format, Args&&... args){
    size_t size = snprintf(nullptr, 0, format.c_str(), forward<Args>(args)...) + 1;
    unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format.c_str(), forward<Args>(args)...);
    return string(buffer.get(), buffer.get() + size - 1);
}

#endif