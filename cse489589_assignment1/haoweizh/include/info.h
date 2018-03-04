#ifndef INFO_H
#define INFO_H
#include <list>
#include <socket_info.h>
#include <string.h>

struct info{
  info():yes(1),clients_number(0){
    bzero(&ip_address,sizeof(ip_address));
    bzero(&port_number,sizeof(port_number));
  }
  std::list<socket_info> clients;
  std::list<block> block_list;
  char ip_address[1024];
  char port_number[1024];
  int listener;
  int yes;        
  int clients_number;
};

#endif
