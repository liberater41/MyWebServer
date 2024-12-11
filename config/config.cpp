#include "config.h"

Config::Config(){
    //数据库
    ip="127.0.0.1";
    port="8080";
    username="chipmunk";
    password="123";
    database="webserver_db";
    max_con=8;
    
    //线程池
    thread_num=8;
    max_req=10000;
    
    //日志
    filename="log";
    log_buf_size=1024;
    queue_size=1024;
    Logwrite=1;//为异步日志
}

Config::~Config(){

}