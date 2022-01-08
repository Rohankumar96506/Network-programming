#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<iostream>
#define BUFFER_SIZE 150
#define NAME_BUFFER_SIZE 25
#define HOST_ADDR 9001


char name[NAME_BUFFER_SIZE];


void* reciver_handler(void* args)
{   
    int client_socket = *(int*)args;
    char buffer[BUFFER_SIZE];

    memset(buffer,0,sizeof buffer);
    

    while(1)
    {
    int errnum = recv(client_socket,buffer,sizeof buffer,0);
    if(errnum == 0)
    {
        printf("you had leave the chat\n");
        break;
    }
    else
    if(errnum < 0)
    {
        printf("Error in Receive\n");
    }
    
    std::cout<<buffer;
    
    }

    return NULL;
}

void newline_remove(char* arr)
{
    int n = strlen(arr);
    arr[n-1] = '\0';

}

int main()
{
    pthread_t pid;
    int client_socket;
    client_socket = socket(PF_INET,SOCK_STREAM,0);
    char buffer[BUFFER_SIZE];
    printf("Enter your name: ");
    fgets(name, sizeof name, stdin);
    newline_remove(name);
    printf("\n");
    while(strlen(name)<=0)
    {
        printf("Please Enter the Right name: ");
        fgets(name, sizeof name, stdin);
        newline_remove(name);
        printf("\n");
    }
    //specify an address for socket
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port=htons(HOST_ADDR);
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");

    int errnum = connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address));

    if(errnum == -1)
    {
        printf("Not able to connect to server please try again\n");
        return 1;
    }

    send(client_socket,name,strlen(name)+1,0);

    if(pthread_create(&pid,NULL,&reciver_handler,(void*)&client_socket)!=0)
    {
        printf("Error in Pthread\n");
    }
    
    
    printf("For Leaving the chat write \"quit_me\"\n");
    char quit[] = "quit_me\n";
    while(1)
    {
    memset(buffer,0,sizeof buffer);
    fgets(buffer, sizeof buffer, stdin);
    if(!strcmp(buffer,quit))
    {   printf("\nYou have left the Chat.\n");
        break;
    }
    send(client_socket,&buffer, strlen(buffer)+1,0);
    }
    close(client_socket);
}
