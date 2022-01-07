#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#define BUFFER_SIZE 150




void* reciver_handler(void* args)
{   
    int client_socket = *(int*)args;
    char buffer[BUFFER_SIZE];

    while(1)
    {
    int errno = recv(client_socket,buffer,sizeof buffer,0);
    if(errno == 0)
    {
        printf("you had leave the chat\n");
    }
    else
    if(errno < 0)
    {
        printf("Error in Receive\n");
    }

    printf("%s\n",buffer);
    
    }

    return NULL;
}

int main()
    pthread_t pid;
    int client_socket;
    client_socket = socket(PF_INET,SOCK_STREAM,0);
    char buffer[256];
    char name[25];
    int leave_flag = 0;
    printf("Enter your name: ");
    scanf("%s",name);
    printf("\n");
    while(strlen(name)<=0)
    {
        printf("Please Enter the Right name: ");
        scanf("%s",name);
        printf("\n");
    }
    //specify an address for socket
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port=htons(4515);
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    int errno = connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address));

    if(errno == -1)
    {
        printf("Not able to connect to server please try again\n");
        return 1;
    }

    send(client_socket,name,strlen(name),0);

    pthread_create(&pid,NULL,&reciver_handler,(void*)&client_socket);
    
    printf("For Leaving the chat write \"quit_me\"\n");
    
    while(1)
    {
    printf("%s",name);
    memset(buffer,0,sizeof buffer);
    scanf("%s",buffer);
    if(!strcmp(buffer,"quit_me"));
    {   printf("You have left the Chat.\n");
        break;
    }

    printf("\n");
    send(client_socket,&buffer,strlen(buffer),0);
    }

    close(client_socket);
}
