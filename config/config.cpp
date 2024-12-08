#include "config.h"

Config::Config(){
    //数据库
    string ip="127.0.0.1";
    string port="8080";
    string username="chipmunk";
    string password="123";
    string database="webserver_db";
    int max_con=8;
    
    //线程池
    int thread_num=8;
    int max_req=10000;
    
    //日志
    string filename="log";
    int log_buf_size=1024;
    int queue_size=1024;
    int Logwrite=1;//为异步日志
}