#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>


int main()
{
    //create socket for server
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    printf("I am running\n");
    //specify an address for socket
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof server_address);
    server_address.sin_family = PF_INET;
    server_address.sin_port=htons(5011);
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    char server_message[256]="<==============START THE CHAT==============>";

    bind(server_socket,(struct sockaddr*)&server_address,sizeof server_address);

    listen(server_socket,5);

    int client_socket;

    client_socket = accept(server_socket,NULL,NULL);

    
    while(1)
    {
    send(client_socket,server_message,sizeof server_message,0);

    recv(client_socket,server_message,sizeof server_message,0);
    printf("client: %s\n",server_message);
    printf("servrside:");
    sleep(1);
    scanf("%s\n",server_message);
    }
    close(server_socket);

    return 0;
}