#ifndef HTTP_CON_H
#define HTTP_CON_H

#include "mysql/sql_pool.h"

class http_con{
public:
    http_con();
    ~http_con();
    enum METHOD{
        GET,
        POST
    };
    enum HTTP_status{
        NO_REQUEST,
        GET_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,

    };
    void init(int client_fd,Sql_pool *sql_pool);
    void process();
private:
    void process_read();
    void process_write();
private:
    int m_client_fd;
    Sql_pool *m_sql_pool;
};

#endif