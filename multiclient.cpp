#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#define BUFFER_SIZE 100




void* reciver_handler(void* args)
{   
    int client_socket = *(int*)args;
    char buffer[BUFFER_SIZE];
    
    printf("%d",client_socket);

    while(1)
    {
    int n = recv(client_socket,buffer,sizeof buffer,0);
    printf("%s1\n",buffer);
    }

    return NULL;
}

int main()
{   pthread_t pid;
    int client_socket;
    client_socket = socket(PF_INET,SOCK_STREAM,0);
    char buffer[256];
    //specify an address for socket
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port=htons(4515);
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
    printf("%d\n",client_socket);

    int e = connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address));
    
    if(e == -1)
    {
        printf("Error connect");
        return 1;
    }

    pthread_create(&pid,NULL,&reciver_handler,(void*)&client_socket);
    sleep(1);
    int uid = rand();
    while(1)
    {
    printf("client%d: ",uid);
    memset(buffer,0,sizeof buffer);
    scanf("%s",buffer);
    printf("\n");
    send(client_socket,&buffer,strlen(buffer),0);
     sleep(1);
    }


    close(client_socket);
}