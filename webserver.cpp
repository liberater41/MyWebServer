#include <arpa/inet.h>

#include "webserver.h"
#include "log/log.h"


using namespace std;

WebServer::WebServer(){
    
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
    
    
    
    m_epoll_fd=epoll_create(3);
    epoll_event listen_event;
    listen_event.events=EPOLLIN;
    listen_event.data.fd=m_listen_fd;
    epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,m_listen_fd,&listen_event);

    Log::LOG_INFO("server is initializing");
}

void WebServer::loop(){
    sockaddr_in address_client;
    socklen_t addlen=sizeof(address_client);
    while(1){
        int num_of_fds=epoll_wait(m_epoll_fd,m_events,MAX_CLIENTS,-1);
        if(num_of_fds<=0){
            break;
        }
        for(int i=0;i<num_of_fds;i++){
            if(m_events[i].data.fd==m_listen_fd){
                int client_fd=accept(m_listen_fd,(struct sockaddr *)&address_client,&addlen);
                if(client_fd<0){
                    Log::LOG_ERROR("accept client error");
                    continue;
                }
                m_clients[client_fd].init(client_fd);

                epoll_event client_event;
                client_event.events=EPOLLIN|EPOLLOUT;
                client_event.data.fd=client_fd;
                epoll_ctl(m_epoll_fd,EPOLL_CTL_ADD,client_fd,&client_event);

                int port_client = ntohs(address_client.sin_port);
                cout<<"client "<<port_client<<"is accepted"<<endl;
                Log::LOG_INFO("client %d is accepted",port_client);
            }
            else{
                if((m_events[i].events & EPOLLIN) && (m_events[i].events & EPOLLOUT)){
                    m_threadpool->add_req(&m_clients[m_events->data.fd]);
                }
            }
        }
    }
}
    
WebServer::~WebServer(){

}