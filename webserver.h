#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include "threadpool/threadpool.h"
#include "mysql/sql_pool.h"

class WebServer{
public:
    static WebServer *get_instance(){
        static WebServer instance;
        return &instance;
    }

    void init(string ip,string port,string username,string password,string database,int max_con,
            int thread_num,int max_req,
            string filename,int log_buf_size,int queue_size,int Logwrite);

    void start();
public:
    
private:
    WebServer();
    ~WebServer();
private:        
    //数据库
    Sql_pool *m_con_pool;

    //线程池

    //日志
    Log *log;

    //网络通信
    int m_listen_fd;
    string m_ip;
    string m_port;
};

#endif