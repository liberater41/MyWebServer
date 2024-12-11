#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <sys/epoll.h>
#include "threadpool/threadpool.h"
#include "http_con/http_con.h"
#include "mysql/sql_pool.h"

const int MAX_CLIENTS=10000;

class WebServer{
public:
    static WebServer *get_instance(){
        static WebServer instance;
        return &instance;
    }

    void init(string ip,string port,string username,string password,string database,int max_con,
            int thread_num,int max_req,
            string filename,int log_buf_size,int queue_size,int Logwrite);
    void con_pool_init();
    void log_init();
    void threadpool_init();
    void start();
    void loop();
public:
    
private:
    WebServer();
    ~WebServer();
private:        
    //数据库
    Sql_pool *m_con_pool;
    string m_username;
    string m_password;
    string m_database;
    int m_max_con;

    //线程池
    int m_thread_num;
    int m_max_req;
    threadpool<http_con> *m_threadpool;

    //日志
    Log *log;
    string m_filename;
    int m_log_buf_size;
    int m_queue_size;
    int m_Logwrite;

    //网络通信
    int m_listen_fd;
    string m_ip;
    string m_port;

    int m_epoll_fd;

    epoll_event m_events[MAX_CLIENTS];

    http_con m_clients[MAX_CLIENTS];

};

#endif