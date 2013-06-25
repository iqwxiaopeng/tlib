
#include "../epollobserver.h"
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define SERVER_PORT 8175
#define LENGTH_OF_LISTEN_QUEUE 10
#define BUFFER_SIZE 255
#define WELCOME_MESSAGE "welcome to connect the server. "

class Server:public Epoller
{
public:
    virtual ~Server(){}
	int InputNotify()
	{
		struct sockaddr_in cliaddr;
		socklen_t length =  sizeof (cliaddr);
		clifd = accept(servfd,( struct sockaddr * ) &cliaddr, &length);
		char buf[BUFFER_SIZE];
		long timestamp;
		strcpy(buf,WELCOME_MESSAGE);

		printf( "from client,IP:%s, Port:%d \n", inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

		timestamp = time(NULL);

		strcat(buf, "\n timestamp in server: " );
		strcat(buf,ctime(&timestamp));

		send(clifd,buf,BUFFER_SIZE, 0 );
		return 0;

	}
	int OutputNotify(){return 0;}

	void Run()
	{
		if ((servfd = socket(AF_INET,SOCK_STREAM, 0 )) <  0 )
		{
		   printf( " create socket error!\n " );
		   exit( 1 );
		}
		this->SetFd(servfd);
		struct sockaddr_in servaddr;
		bzero( &servaddr, sizeof (servaddr));

		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERVER_PORT);
		servaddr.sin_addr.s_addr = htons(INADDR_ANY);

		if (bind(servfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
		   printf( "bind to port %d failure!\n" ,SERVER_PORT);
		   exit( 1 );
		}

		if (listen(servfd,LENGTH_OF_LISTEN_QUEUE) <  0 ){
		   printf( " call listen failure!\n " );
		   exit( 1 );
		}

	}
private:
	int servfd;
	int clifd;
};


int main()
{
	Server *s = new Server();
    s->EnableInput(true);
    s->Run();
	EpollServer *es = new EpollServer(100);
	es->AddEpoller(s);

    while(1)
    {
        es->WaitAndProcess(5000);
    }
}
