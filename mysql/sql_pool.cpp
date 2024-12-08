#include "sql_pool.h"
using namespace std;

Sql_pool::Sql_pool(){
    m_con_count=0;
}

Sql_pool::~Sql_pool(){

}

void Sql_pool::init(string ip,string port,string username,string password,string database,int max_con){
    m_ip=ip;
    m_port=port;
    m_username=username;
    m_password=password;
    m_database=database;
    m_max_con=max_con;

    for(int i=0;i<max_con;i++){
        MYSQL *con=nullptr;
        con=mysql_init(con);

        if(con==nullptr){
            Log::LOG_ERROR("MySQL Init Error");
            exit(1);
        }

        con=mysql_real_connect(con,ip.c_str(),username.c_str(),password.c_str(),database.c_str(),stoi(port),nullptr,0);

        if(con==nullptr){
            Log::LOG_ERROR("MySQL Connect Error");
            exit(1);
        }

        m_con_pool.push(con);
        m_con_count++;
        
    }
    m_sem=sem(m_con_count);
}

MYSQL* Sql_pool::get_connection(){
    MYSQL *con=nullptr;
    m_sem.wait();

    m_mutex.lock();
    con=m_con_pool.front();
    if (mysql_ping(con)) {
        m_mutex.unlock();
        return nullptr;//连接无效
    }
    m_con_pool.pop();
    m_con_count--;
    m_mutex.unlock();

    return con;
}

bool Sql_pool::release_connection(MYSQL *con){
    if(con==nullptr){
        return false;
    }

    m_mutex.lock();
    m_con_pool.push(con);
    m_con_count++;
    m_mutex.unlock();

    m_sem.post();
    return true;
}

bool Sql_pool::is_free_connection(){
    m_mutex.lock();
    bool res=(m_con_count==m_max_con);
    m_mutex.unlock();
    return res;
}