#include "config/config.h"
#include "webserver.h"

int main(int argc,char** argv){
    Config config;

    WebServer *server;
    server=WebServer::get_instance();
    server->init(config.ip,config.port,config.username,config.password,config.database,config.max_con,
            config.thread_num,config.max_req,
            config.filename,config.log_buf_size,config.queue_size,config.Logwrite);
}