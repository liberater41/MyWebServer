#ifndef SQL_H
#define SQL_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include "../lock/locker.h"
#include "../log/log.h"
#include <queue>

using namespace std;

class Sql_pool{
public:
    static Sql_pool* get_instance(){
        static Sql_pool instance;
        return &instance;
    }
    
    void init(string ip,string port,string username,string password,string database,int max_con);
    MYSQL* get_connection();
    bool release_connection(MYSQL *connection);
    bool is_free_connection();
    
public:
    string m_ip;
    string m_port;
    string m_username;
    string m_password;
    string m_database;


private:
    Sql_pool();
    ~Sql_pool();

    int m_max_con;
    int m_con_count;
    locker m_mutex;
    sem m_sem;
    queue<MYSQL*> m_con_pool;

};


#endif