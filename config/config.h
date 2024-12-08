#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using namespace std;

class Config{
public:
    Config();
    ~Config();
public:
    //数据库
    string ip;
    string port;
    string username;
    string password;
    string database;
    int max_con;
    
    //线程池
    int thread_num;
    int max_req;
    
    //日志
    string filename;
    int log_buf_size;
    int queue_size;
    int Logwrite;//是否为异步日志
};

#endif