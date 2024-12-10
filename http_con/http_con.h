#ifndef HTTP_CON_H
#define HTTP_CON_H

class http_con{
public:
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
private:
    
};

#endif