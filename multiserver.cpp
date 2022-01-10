#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#define CLIENT_SIZE 50
#define BUFFER_SIZE 150
#define NAME_BUFFER_SIZE 25
#define NAME_BUFFER 180
#define HOST_ADDR 9001

/*CLIENT_SIZE is number of client can be connect at one time
  BUFFER_SIZE is size of message receive at one time from client
  NAME_BUFFER_SIZE is size of name os client
  NAME_BUFFER is to store both client name and his or her message
  HOST_ADDR is port address in which server will listen */



struct client_data *client_info[CLIENT_SIZE];
int no_of_client; //to store number of client present in server


// for storing client info at one place
struct client_data
{
    int sock;
    int uid;
    char name[50];   
    
};


//array for storing all client
void add_in_arr(struct client_data* client_sock)
{
    for(int i = 0;i<CLIENT_SIZE;i++)
        {
            if(client_info[i]==NULL)
            {
                client_info[i] = client_sock;
    
                break;

            }
        }
}

//to concat name and his or her message
void concat(char name[],char message[],char buffer[])
{ 
    strcpy(buffer,name);
    strcat(buffer,": \0");
    strcat(buffer,message);
  
}

//for handling send and recieve message from client
void* client_handler(void* args)
{ struct client_data* client = (struct client_data*)args;
  char buffer[BUFFER_SIZE];
  char name_buffer[NAME_BUFFER_SIZE];
  
  while(1)
  {
  memset(buffer,0,sizeof buffer);
  
  
  int flag = recv(client->sock,buffer,sizeof buffer,0);
  concat(client->name,buffer,name_buffer);
  if(flag>0)
  {   
      printf("%s(%d) has send a message and packet size is %d\n",client->name,client->uid,flag);
      
      for(int i = 0;i<CLIENT_SIZE;i++)
      {
          if(client_info[i]!=NULL && client_info[i]->uid!=client->uid)
          {   
              send(client_info[i]->sock,name_buffer,strlen(name_buffer)+1,0);
          }
      }

  }
  else
  if(flag <= 0)
  {
      for(int i = 0;i<CLIENT_SIZE;i++)
      {
          if(client_info[i]!=NULL && client_info[i]->uid==client->uid)
            {

            printf("client has left id is %d and name is %s\n",client->uid,client->name);
            client_info[i] = NULL;
            close(client->sock);
            free(client);
            no_of_client--;
            break;
            }

      }
      return NULL;
      

   
  }

  }

return NULL;

}



int main()
{   pthread_t pid;
    int server_sock;
    int client_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_storage client_addr;
    struct client_data *pclient;
    no_of_client = 0;
    int user_id = 0;                        
    char name_buffer[NAME_BUFFER_SIZE];  //for storing name get from client

    memset(&server_addr,0,sizeof server_addr);

    for(int i = 0;i<50;i++)
    {
        client_info[i] = NULL;
    }

    socklen_t client_addr_size = sizeof client_addr;
  
    //giving server info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(HOST_ADDR);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_sock = socket(PF_INET,SOCK_STREAM,0);


    if(server_sock==-1)
    {
        printf("ERROR IN SOCKET\n");
        return 1;
    }

    if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof server_addr)==-1)
    {
        printf("ERROR IN BIND\n");
        return 1;
    }

    if(listen(server_sock,10)==-1)
    {
        printf("ERROR IN LISTEN\n");
        return 1;
    }

    while(1)
    {   
        
        client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&client_addr_size);
        
        if(client_sock == -1)
        {
            printf("Error in accept\n");
            return 0;
        }

        if(no_of_client <= CLIENT_SIZE)
        {
        pclient = (struct client_data*)malloc(sizeof(client_data));
        pclient->sock = client_sock;
        pclient->uid = user_id++;
        int errno = recv(client_sock,name_buffer,sizeof name_buffer,0);
        if(errno == 0)
        {
            printf("CLIENT HAS LEFT %s\n",name_buffer);
        }
        else
        if(errno < 0)
        {
            printf("Error in RECEIVE\n");
            free(pclient);
            continue;
        }
        printf("Client has enter the chat %s\n",name_buffer);
        strcpy(pclient->name,name_buffer);
        add_in_arr(pclient);
        no_of_client++;
        pthread_create(&pid,NULL,&client_handler,pclient);
        }
        else
        {   printf("MAX CLIENT LIMIT REACH(%d)\n",no_of_client);
            close(client_sock);    
        }

        
      
    }


    return 0;
}
