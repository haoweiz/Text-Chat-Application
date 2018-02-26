#include "../include/server.h"
#include "../include/logger.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>


#define STD_IN 0
using namespace std;

server::server(char* port){
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
  struct sockaddr_in my_addr;
  bzero(&my_addr,sizeof(my_addr));
  my_addr.sin_family = AF_INET; 
  my_addr.sin_port = htons(atoi(port)); 
  my_addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(information.listener, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
    cerr<<"bind";
    exit(1);
  }

  /* Listen socket */
  if (listen(information.listener, 4) < 0) {
    cerr<<"listen";
    exit(1);
  }

  /* Add the listener to master set */
  fd_set master; 
  fd_set read_fds; 
  FD_ZERO(&master); 
  FD_ZERO(&read_fds);
  FD_SET(information.listener,&master);
  FD_SET(STD_IN,&master);

  /* Main loop */
  int fdmax = information.listener;
  int addrlen;
  int newfd;
  int nbytes;
  char buf[100];
  struct sockaddr_in remoteaddr;
  for(;;){
    read_fds = master; 
    int ret;
    if ((ret = select(fdmax+1, &read_fds, NULL, NULL, NULL)) < 0) {
      cerr<<"error";
      exit(1);
    }
    for(int i = 0; i <= fdmax; i++) {
      memset((void *)&buf,'\0',sizeof(buf));
      if (FD_ISSET(i, &read_fds)) {
        if (i == STD_IN){
          /* Standard input */
          read(STD_IN,buf,100);
          buf[strlen(buf)-1]='\0';
          if (strcmp(buf,"AUTHOR")==0){
            print_author();
          }
          if (strcmp(buf,"IP")==0){
              print_ip();
          }
          if (strcmp(buf,"PORT")==0){
            print_port();
          }
          if (strcmp(buf,"LIST")==0){
            cse4589_print_and_log("[LIST:SUCCESS]\n");
              for(list<socket_info>::iterator iter = information.clients.begin();iter != information.clients.end();++iter){
                 if (strcmp(iter->status,"logged-in") == 0)
                    cse4589_print_and_log("%-5d%-35s%-20s%-8d\n",iter->list_id,iter->hostname,iter->ip_addr,iter->port_num);
              }
              cse4589_print_and_log("[LIST:END]\n");
          }
          if (strncmp(buf,"BLOCKED",7)==0){
            





          }
          if (strcmp(buf,"STATISTICS")==0){
            cse4589_print_and_log("[STATISTICS:SUCCESS]\n");
            for(list<socket_info>::iterator iter = information.clients.begin();iter != information.clients.end();++iter)
              cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n",iter->list_id,iter->hostname,iter->num_msg_sent,iter->num_msg_rcv,iter->status);
            cse4589_print_and_log("[STATISTICS:END]\n");
          }
        }
        else if (i == information.listener) {
          /* Handle new connections */
          addrlen = sizeof(remoteaddr);
          if ((newfd = accept(information.listener, (struct sockaddr*)&remoteaddr, (socklen_t*)&addrlen)) == -1) {
            cerr<<"accept";
          } 
          else {
            FD_SET(newfd, &master);  /* Add to master set */
            if (newfd > fdmax) {     /* Keep track of the maximum */
              fdmax = newfd;
            }

            /* New client */
            struct sockaddr_in *sin = (struct sockaddr_in*)&remoteaddr;
            const char *sts = "logged-in";
            struct socket_info si;
            si.list_id = ++information.clients_number;
            char *ip = inet_ntoa(sin->sin_addr);
            
            /* Get hostname */
            struct in_addr *addr_temp;
            struct sockaddr_in saddr;
            struct hostent *hoste;
            if(!inet_aton(ip,&saddr.sin_addr)){
              cerr<<"inet_aton"<<endl;
              exit(1);
            }
            
            if((hoste = gethostbyaddr((const void *)&saddr.sin_addr,sizeof(ip),AF_INET)) == NULL){
              cerr<<"gethostbyaddr"<<endl;
              exit(1);
            }
            char *n = hoste->h_name;
            strncpy(si.hostname,n,strlen(n));

            strcpy(si.ip_addr,ip);
            si.port_num = sin->sin_port;
            si.fd = newfd;
            strncpy(si.status,sts,strlen(sts));
            information.clients.push_back(si);
          }
        } 
        else {
          char msg[1024];
          /* Handle data from a client */
          if ((nbytes = recv(i, msg, sizeof(msg), 0)) <= 0) {
            /* Got error or connection closed by client */
            if (nbytes == 0) {
              /* Connection closed */
              information.clients_number--;
              for(list<socket_info>::iterator iter = information.clients.begin();iter != information.clients.end();++iter){
                if(iter->fd == i){
                  const char *stso = "logged-out";
                  strncpy(iter->status,stso,strlen(stso));
                }
              }
              cout<<"selectserver: socket "<<i<<" hung up"<<endl;
            } 
            else {
              cerr<<"recv";
            }
            close(i); 
            FD_CLR(i, &master); /* Remove from master set */
          } 
          else {
            /* We got some data from a client */
            cout<<msg<<endl;
            char *arg[3];
            arg[0] = strtok(msg," ");
            for(int i = 1;i != 3;++i){
              arg[i] = strtok(NULL," ");
              if(!arg[i]) break;
            }
            
          }
        }
      } 
    }
  }
}





