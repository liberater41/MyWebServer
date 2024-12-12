#ifndef HTTP_CON_H
#define HTTP_CON_H

#include <arpa/inet.h>
#include "mysql/sql_pool.h"

#define READ_BUFFER_SIZE 1024
#define WRITE_BUFFER_SIZE 1024

class http_con{
public:
    http_con();
    ~http_con();
    enum METHOD{
        GET,
        POST
    };
    enum RESPONSE_MODE{
        NO_REQUEST,//405
        GET_REQUEST,
        POST_REQUEST,
        NO_RESOURCE,//404
        FORBIDDEN_REQUEST,//403

    };
    void init(int client_fd,Sql_pool *sql_pool);
    void process();
private:
    RESPONSE_MODE process_read();
    void process_write(RESPONSE_MODE resp);
    void response_GET();
    void response_POST();
    void handle_login();
    void response_404();
    void response_405();
    void response_403();
    string get_content_type(string path);
private:
    int m_client_fd;
    Sql_pool *m_sql_pool;
    sockaddr_in m_address_client;
    int m_port;

    string base_path;
    int file_open_mode;
    string sour_addr;
    char *req_content;
};

#endif