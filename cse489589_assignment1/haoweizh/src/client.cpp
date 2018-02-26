#include "../include/client.h"
#include "../include/logger.h"
#include "../include/common.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>

#define STD_IN 0
using namespace std;


client::client(char *port){
  /* Save port number */
  strcpy(information.port_number,port);

  /* Save IP address */
  struct hostent *ht;
  char hostname[1024];
  if (gethostname(hostname,1024) < 0){
    cerr<<"gethostname"<<endl;
    exit(1);
  }
  if ((ht=gethostbyname(hostname)) == NULL){
    cerr<<"gethostbyname"<<endl;
    exit(1);
  }
  struct in_addr **addr_list = (struct in_addr **)ht->h_addr_list;
  for(int i = 0;addr_list[i] != NULL;++i){
    strcpy(information.ip_address,inet_ntoa(*addr_list[i]));
  }

  /* Create socket. */
  if ((information.listener = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    cerr<<"socket\n";
    exit(1);
  }

  /* Bind socket */
  struct sockaddr_in client_addr; 
  bzero(&client_addr,sizeof(client_addr));
  client_addr.sin_family = AF_INET; 
  client_addr.sin_port = htons(atoi(port)); 
  client_addr.sin_addr = *((struct in_addr*)ht->h_addr);
  if (bind(information.listener, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    cerr<<"bind";
    exit(1);
  }

  /* Main loop */
  char buf[100];
  for(;;){
    read(STD_IN,buf,100);
    buf[strlen(buf)-1]='\0';
    if (strcmp(buf,"EXIT") == 0){
      cse4589_print_and_log("[EXIT:SUCCESS]\n");
      cse4589_print_and_log("[EXIT:END]\n");
      break;
    }
    else if (strcmp(buf,"AUTHOR") == 0){
      print_author();
    }
    else if (strcmp(buf,"PORT") == 0){
      print_port();
    }
    else if (strcmp(buf,"IP") == 0){
      print_ip();
    }
    else if (strcmp(buf,"LIST") == 0){
      cse4589_print_and_log("[LIST:SUCCESS]\n");
        for(list<socket_info>::iterator iter = information.clients.begin();iter != information.clients.end();++iter){
          if (strcmp(iter->status,"logged-in") == 0)
             cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",iter->list_id,iter->hostname,iter->ip_addr,iter->port_num);
        }
        cse4589_print_and_log("[LIST:END]\n");
    }
    else if (strncmp(buf,"LOGIN",5) == 0){
      char *server_ip;
      char *server_port;
      strtok(buf," ");
      server_ip = strtok(NULL," ");
      server_port = strtok(NULL," ");
      
      /* Invalid ip address */
      if (!isvalid(server_ip)){
        print_error("LOGIN");
        exit(1);
      }
      else{
        struct addrinfo hints;
        struct addrinfo *result;
        bzero(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (/*getaddrinfo(server_ip, server_port, &hints, &result) != 0*/false) {
          print_error("LOGIN");
          exit(1);
        }
        else{
          /* Get socket fd */
          if ((information.listener = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            cerr<<"socket"<<endl;
            exit(1);
          }

          /* Connect to server */
          struct sockaddr_in dest_addr; 
          bzero(&dest_addr,sizeof(dest_addr));
          dest_addr.sin_family = AF_INET;
          int port = atoi(server_port);
          dest_addr.sin_port = htons(port);
          dest_addr.sin_addr.s_addr = inet_addr(server_ip);
          if ((connect(information.listener, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) < 0){
            cerr<<"connect"<<endl;
            exit(1);
          }
          cse4589_print_and_log("[LOGIN:SUCCESS]\n");
          cse4589_print_and_log("[LOGIN:END]\n");

          char buf[100];
          for(;;){
            /* Add the listener to read set */
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(STD_IN,&read_fds);
            FD_SET(information.listener,&read_fds);

            int fd_max = information.listener;
            select(fd_max+1, &read_fds, NULL, NULL, NULL);
            if (FD_ISSET(STD_IN, &read_fds)){
              read(STD_IN,buf,100);
              buf[strlen(buf)-1]='\0';

              if (strcmp(buf,"AUTHOR") == 0){
                print_author();
              }
              else if (strcmp(buf,"PORT") == 0){
                print_port();
              }
              else if (strcmp(buf,"IP") == 0){
                print_ip();
              }
              else if (strcmp(buf,"LIST") == 0){
                  cse4589_print_and_log("[LIST:SUCCESS]\n");
                  for(list<socket_info>::iterator iter = information.clients.begin();iter != information.clients.end();++iter){
                    if (strcmp(iter->status,"logged-in") == 0)
                      cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",iter->list_id,iter->hostname,iter->ip_addr,iter->port_num);
                  }
                  cse4589_print_and_log("[LIST:END]\n");
              }
              else if(strcmp(buf,"REFRESH") == 0){
              }
              else if(strncmp(buf,"SEND",4) == 0){
                if(send(information.listener,buf,strlen(buf),0)<0){
                  print_error("SEND");
                }
              }
              else if(strncmp(buf,"BROADCAST",9) == 0){
              }
              else if(strncmp(buf,"BLOCK",5) == 0){
              }
              else if(strncmp(buf,"UNBLOCK",7) == 0){
              }
              else if(strcmp(buf,"LOGOUT") == 0){
              }
              else if(strcmp(buf,"EXIT") == 0){
                close(information.listener);
                exit(0);
              }
            }
            else{
              int msg[1024];
              int recvbytes;
              recvbytes = recv(information.listener,msg,sizeof msg,0);
              if(FD_ISSET(information.listener,&read_fds)){
                
              }
            }
          }
        }
      }
    }
  }
}


bool client::isvalid(char *server_ip){



  return true;
}
