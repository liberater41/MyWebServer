#include "http_con.h"

http_con::http_con(){

}

http_con::~http_con(){

}

void http_con::init(int client_fd,Sql_pool *sql_pool){
    m_client_fd=client_fd;
    m_sql_pool=sql_pool;
}

void http_con::process(){
    process_read();
    process_write();
}