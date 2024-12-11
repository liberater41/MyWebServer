server: main.cpp   ./http_con/http_con.cpp ./log/log.cpp ./mysql/sql_pool.cpp  ./threadpool/threadpool.cpp  webserver.cpp ./config/config.cpp
	g++ -o server $^ -g -lpthread -lmysqlclient
