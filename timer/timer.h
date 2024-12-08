#ifndef TIME_H
#define TIME_H


#include <arpa/inet.h>

#include "../log/log.h"

struct client{
    sockaddr_in address;
    int sockfd;
    Mytimer *timer;
};

class Mytimer{
    
};


#endif