#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include "../webserver.h"
#include "http_con.h"

http_con::http_con(){
    base_path="root/";
}

http_con::~http_con(){

}

void http_con::init(int client_fd,Sql_pool *sql_pool){
    m_client_fd=client_fd;
    m_sql_pool=sql_pool;
    socklen_t addlen;
    getpeername(m_client_fd,(struct sockaddr *)&m_address_client, &addlen);
    m_port=ntohs(m_address_client.sin_port);
}

void http_con::process(){
    RESPONSE_MODE resp=process_read();
    process_write(resp);
}

http_con::RESPONSE_MODE http_con::process_read(){
    char buffer[READ_BUFFER_SIZE]={0};
    ssize_t read_size=read(m_client_fd,buffer,READ_BUFFER_SIZE);
    buffer[READ_BUFFER_SIZE]='\0';
    cout<<buffer<<endl;

    if(read_size<=0){
        WebServer::get_instance()->discon(m_client_fd,m_port);
    }

    char *method = strtok(buffer, " ");  // 提取方法
    char *path = strtok(NULL, " ");     // 提取路径
    char *version = strtok(NULL, "\r\n"); // 提取版本
    
    //解析请求头
    RESPONSE_MODE resp;
    METHOD met;

    if(strcmp(method,"GET")==0){
        met=GET;
    }
    else if(strcmp(method,"POST")==0){
        met=POST;
    }
    else{
        resp=NO_REQUEST;
        return resp;
    }

    string temp_path=path;
    string final_path=base_path+temp_path;
    struct stat buf;
    if(stat(final_path.c_str(),&buf)==0){
        if(met==GET){
            resp=GET_REQUEST;
        }
        else if(met==POST){
            resp=POST_REQUEST;
        }
    }
    else{
        return NO_RESOURCE;
    }

    
    //解析请求行
    char *line = strtok(NULL, "\r\n");
    while (line && strlen(line) > 0) {
        char *key = strtok(line, ": ");  
        char *value = strtok(NULL, "\r\n");  
        line = strtok(NULL, "\r\n");  
    }
    
    //解析请求体
    char *content_length_header = strstr(buffer, "Content-Length");
    int content_length = 0;
    if (content_length_header) {
        content_length = atoi(content_length_header + strlen("Content-Length: "));
    }

    if (content_length > 0) {
        req_content = buffer + (read_size - content_length);
        std::cout << "Body: " << req_content << std::endl;
    }

    sour_addr=final_path;
    return resp;


}

void http_con::process_write(RESPONSE_MODE resp){
    string resp_buf;
    switch (resp)
    {
    case NO_REQUEST:
        response_405();
        break;
    case NO_RESOURCE:
        response_404();
        break;
    case FORBIDDEN_REQUEST:
        response_403();
        break;
    case GET_REQUEST:
        /* code */
        break;
    case POST_REQUEST:
        /* code */
        break;    
    default:
        break;
    }
}

void http_con::response_GET(){
    string content_type=get_content_type(sour_addr);
    ifstream file;
    if(file_open_mode==0){
        file.open(sour_addr);
    }
    else if(file_open_mode==1){
        file.open(sour_addr,ios::binary);
    }
    if(!file.is_open()){
        response_404();
    }

    file.seekg(0, ios::end);
    int file_size = file.tellg();
    file.seekg(0, ios::beg);

    string head="HTTP/1.1 200 OK\r\n"
        "Content-Type: "+content_type+"\r\n"
        "Content-Length: "+to_string(file_size)+"\r\n"
        "\r\n";
      
}

void http_con::response_POST(){
    string content_type=get_content_type(sour_addr);
}

void http_con::response_404(){
    string resp_buf;
    resp_buf=
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Length: 13\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "404 Not Found";
    send(m_client_fd,resp_buf.c_str(),resp_buf.length(),0);    
}

void http_con::response_403(){
    string resp_buf;
    resp_buf=
    "HTTP/1.1 403 Forbidden\r\n"
    "Content-Length: 13\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "403 Forbidden";
    send(m_client_fd,resp_buf.c_str(),resp_buf.length(),0);
}

void http_con::response_405(){
    string resp_buf;
    resp_buf=
    "HTTP/1.1 405 Method Not Allowed\r\n"
    "Content-Length: 22\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "405 Method Not Allowed";
    send(m_client_fd,resp_buf.c_str(),resp_buf.length(),0);
}

string http_con::get_content_type(string path){
    const char *ext = strrchr(path.c_str(), '.');
    string content_type;
    
    if (strcmp(ext, ".html") == 0) {content_type="text/html";file_open_mode=0;}
    else if (strcmp(ext, ".css") == 0) {content_type="text/css";file_open_mode=0;}
    else if (strcmp(ext, ".js") == 0) {content_type="application/javascript";file_open_mode=0;}
    else if (strcmp(ext, ".jpeg") == 0) {content_type="image/jpeg";file_open_mode=1;}
    else if (strcmp(ext, ".jpg") == 0) {content_type="image/jpeg";file_open_mode=1;}
    else if (strcmp(ext, ".gif") == 0) {content_type="image/gif";file_open_mode=1;}
    else if (strcmp(ext, ".ico") == 0) {content_type="image/x-icon";file_open_mode=1;}
    else content_type="application/octet-stream";
    
    return content_type;
}