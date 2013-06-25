#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <vector>


#define SERVER_PORT 8175 // define the defualt connect port id 
#define BUFFER_SIZE 255 

using std::vector;

int main(int argc, char** argv)
{
    int clifd,length = 0;
    vector<int> clifds;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t socklen = sizeof(servaddr);
    char buf[BUFFER_SIZE];

    if (argc < 2 ){
        printf( "usage: %s IpAddr\n" ,argv[0]);
        exit( 1 );
    } 

    while(true)
    {
        if ((clifd = socket(AF_INET,SOCK_STREAM, 0 )) < 0 ){
            perror("create socket error!" );
            getchar();
            break;
        }

        clifds.push_back(clifd);
 
        bzero( &cliaddr, sizeof (cliaddr));
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_addr.s_addr = htons(INADDR_ANY);

        bzero( &servaddr, sizeof (servaddr));
        servaddr.sin_family = AF_INET;
        inet_aton(argv[ 1 ], &servaddr.sin_addr);
        servaddr.sin_port = htons(SERVER_PORT);

        if (connect(clifd,( struct sockaddr * )&servaddr, socklen) < 0 ){
            perror( "can't connect to server");
            continue;
        } 
    
        send(clifd,"Hello",sizeof("Hello"),0);

        length = recv(clifd, buf, BUFFER_SIZE, 0);
        if (length < 0){
            printf( "error comes when recieve data from server %s!", argv[1] );
            exit( 1 );
        } 

        printf( "clients: %d, from server : %s", clifds.size(), buf);
    }

    for (vector<int>::iterator it = clifds.begin() ; it != clifds.end(); ++it)
    {
        close(*it);
    }
    return 0;
} 
