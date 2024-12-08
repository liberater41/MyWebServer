#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include "block_queue.h"

using namespace std;
class Log{
public:
    static Log* get_instance(){
        static Log instance;
        return &instance;
    }

    bool init(const string& filename,int log_buf_size,int queue_size,int is_async);

    template <typename... Args>
    static void write_log(int level,const string& format,Args&&... args);

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
    };
    void* queue_to_file();

private:
    Log();
    ~Log();

    template <typename... Args>
    string format_string(const string& format, Args&&... args);

private:
    string m_filename;
    int m_log_buf_size;
    QueueBuffer *m_queuebuffer;
    string m_log_buffer;
    fstream m_file;
    bool m_is_async;
    locker m_file_mutex;

};

#endif