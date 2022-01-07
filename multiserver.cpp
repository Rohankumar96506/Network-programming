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
#define BUFFER_SIZE 100
#define NAME_BUFFER_SIZE 25



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



//for handling send and recieve message from client
void* client_handler(void* args)
{ struct client_data* client = (struct client_data*)args;
  char buffer[BUFFER_SIZE];
  char message[BUFFER_SIZE + NAME_BUFFER_SIZE];
  
  while(1)
  {
  memset(buffer,0,sizeof buffer);
  memset(message,0,sizeof message);

  int flag = recv(client->sock,buffer,sizeof buffer,0);
  
  if(flag == -1)
  {
      printf("ERROR RECIVE");
      exit(0);
  }
  else
  if(flag>0)
  {   
      sprintf(message,"%s %s\n",client->name,buffer);
      printf("%s(%d) has send a message\n",client->name.client->uid);
      for(int i = 0;i<CLIENT_SIZE;i++)
      {
          if(client_info[i]!=NULL && client_info[i]->uid!=client->uid)
          {
              send(client_info[i]->sock,message,strlen(message),0);
          }
      }

  }
  else
  if(flag == 0)
  {
      for(int i = 0;i<CLIENT_SIZE;i++)
      {
          if(client_info[i]->uid==client->uid)
            {

            printf("client has left id is %d and name is %s\n",client->uid,client->name);
            client_info[i] = NULL;
            close(client->sock);
            free(client);
            no_of_client--;
            break;
            }

      }
      

   
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
    user_id = 0;
    int user_id;                        
    int name_buffer[NAME_BUFFER_SIZE];  //for storing name get from client

    memset(&server_addr,0,sizeof server_addr);

    for(int i = 0;i<50;i++)
    {
        client_info[i] = NULL;
    }

    socklen_t client_addr_size = sizeof client_addr;
  
    //giving server info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4515);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_sock = socket(PF_INET,SOCK_STREAM,0);


    if(server_sock==-1)
    {
        printf("ERROR IN SOCKET");
        return 1;
    }

    if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof server_addr)==-1)
    {
        printf("ERROR IN BIND");
        return 1;
    }

    if(listen(server_sock,10)==-1)
    {
        printf("ERROR IN LISTEN");
        return 1;
    }

    while(1)
    {   
        client_sock = accept(server_sock,(struct sockaddr*)&client_addr,&client_addr_size);
        

        if(no_of_client <= CLIENT_SIZE)
        {
        pclient = (struct client_data*)malloc(sizeof(client_data));
        pclient->sock = client_sock;
        pclient->uid = user_id++;
        int errno = recv(client_sock,name_buffer,sizeof name_buffer,0);
        if(errno == 0)
        {
            printf("CLIENT HAS LEFT %s\n",buffer);
        }
        else
        if(errno < 0)
        {
            printf("Error in RECEIVE\n");
            return 1;
        }
        strcpy(pclient->name,name_buffer);
        add_in_arr(pclient);
        no_of_client++;
        pthread_create(&pid,NULL,&client_handler,pclient);
        }
        else
        {   printf("MAX CLIENT LIMIT REACH(%d)\n",no_of_client)
            close(client_sock);    
        }

        
      
    }


    return 0;
}
