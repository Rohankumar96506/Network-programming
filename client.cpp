#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{   //create socket for server
    int client_socket;
    client_socket = socket(PF_INET,SOCK_STREAM,0);
    char buffer[256];
    //specify an address for socket
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port=htons(9020);
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address));

    recv(client_socket,&buffer,sizeof buffer,0);

    close(client_socket);

    return 0;
}