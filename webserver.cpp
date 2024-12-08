#include "webserver.h"

WebServer::WebServer(){
    
}

void WebServer::init(string ip,string port,string username,string password,string database,int max_con,
            int thread_num,int max_req,
            string filename,int log_buf_size,int queue_size,int Logwrite){
    //日志初始化
    log=Log::get_instance();
    log->init(filename,log_buf_size,queue_size,Logwrite);
    
    //数据库连接池初始化
    m_con_pool=Sql_pool::get_instance();
    m_con_pool->init(ip,port,username,password,database,max_con);

    //线程池初始化


    
}
    
