#ifndef SOCKET_INFO_H
#define SOCKET_INFO_H

struct socket_info{
  int fd;
  int list_id;
  char hostname[40];
  char ip_addr[20];
  int port_num;
  int num_msg_sent;
  int num_msg_rcv;
  char status[16];
};

#endif
