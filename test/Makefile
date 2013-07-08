ALL = server1 server2 client clients  sslserver sslclient sslclients timer1 sort \
        log1


server1_SRC = epoll_t1.cpp \
         ../epollobserver.cpp \
         ../csocket.cpp

server2_SRC = epoll_t2.cpp \
         ../epollobserver.cpp \
         ../csocket.cpp

sslserver_SRC = sslepoll_t1.cpp \
         ../sslepoller.cpp \
         ../epollobserver.cpp \
         ../csslsocket.cpp \
         ../csocket.cpp

client_SRC = epoll_client.cpp
clients_SRC = epoll_clients.cpp
sslClient_SRC = ssl_client.cpp
sslClients_SRC = ssl_clients.cpp
timer1_SRC = timerepoll_t1.cpp \
            ../epollobserver.cpp \
            ../timerepoller.cpp \
            ../csocket.cpp

sort_SRC = sort_t.cpp
log1_SRC = log_t1.cpp \
           ../log.cpp

CC  = gcc
CXX = g++
CFLAGS  = -g -fPIC -Wall  -pipe -fno-ident -MMD -D_GNU_SOURCE -D_REENTRNT
CXXFLAGS += -g -fPIC -Wall  -pipe -fno-ident -D_REENTRNT -std=c++0x
LDFLAGS = -lpthread -lssl

.PHONY: all clean
            
all: $(ALL) 

CLEAN = *.o \
        *.d \
        $(ALL)

server1: $(server1_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

server2: $(server2_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

sslserver: $(sslserver_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

client: $(client_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

clients: $(clients_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

sslclient: $(sslClient_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

sslclients: $(sslClients_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

timer1: $(timer1_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

sort: $(sort_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

log1: $(log1_SRC:.cpp=.o)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(CLEAN)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
