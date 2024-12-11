#include <arpa/inet.h>

#include "webserver.h"
#include "log/log.h"


using namespace std;

WebServer::WebServer(){
    ;
}

void WebServer::init(string ip,string port,string username,string password,string database,int max_con,
            int thread_num,int max_req,
            string filename,int log_buf_size,int queue_size,int Logwrite){
    m_ip=ip;
    m_port=port;

    m_username=username;
    m_password=password;
    m_database=database;
    m_max_con=max_con;

    m_thread_num=thread_num;
    m_max_req=max_req;

    m_filename=filename;
    m_log_buf_size=log_buf_size;
    m_queue_size=queue_size;
    m_Logwrite=Logwrite;    
}

void WebServer::con_pool_init(){
    //数据库连接池初始化
    m_con_pool=Sql_pool::get_instance();
    m_con_pool->init(m_ip,m_port,m_username,m_password,m_database,m_max_con);
}

void WebServer::log_init(){
    //日志初始化
    log=Log::get_instance();
    log->init(m_filename,m_log_buf_size,m_queue_size,m_Logwrite);
}

void WebServer::threadpool_init(){
    //线程池初始化
}



void WebServer::start(){
    struct sockaddr_in server_address;
    const char *ip=m_ip.c_str();
    cout<<m_port<<endl;
    int port=stoi(m_port);
    cout<<port;
    server_address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port=htons(port);

    m_listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(m_listen_fd==-1){
        Log::LOG_ERROR("listen_fd error");
        exit(1);
    }

    if(bind(m_listen_fd,(struct sockaddr *)&server_address,sizeof(server_address))<0){
        Log::LOG_ERROR("bind failed");
        exit(1);
    }

    if(listen(m_listen_fd,3)<0){
        Log::LOG_ERROR("listen fail");
    }
    Log::LOG_INFO("server is initializing");
}
    
WebServer::~WebServer(){

}