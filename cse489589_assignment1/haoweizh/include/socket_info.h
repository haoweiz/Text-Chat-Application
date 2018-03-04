#ifndef SOCKET_INFO_H
#define SOCKET_INFO_H

#include <string.h>
#include <queue>
#include "block.h"
#include "buffer_info.h"

struct socket_info{
  socket_info():num_msg_sent(0),num_msg_rcv(0),list_id(-1),fd(-1),port_num(-1){
    bzero(&hostname,sizeof(hostname));
    bzero(&ip_addr,sizeof(ip_addr));
    bzero(&status,sizeof(status));
  }
  int fd;
  int list_id;
  char hostname[40];
  char ip_addr[20];
  int port_num;
  int num_msg_sent;
  int num_msg_rcv;
  char status[16];
  std::list<block> blocked_list;
  std::queue<buffer_info> buffer;
};

#endif
