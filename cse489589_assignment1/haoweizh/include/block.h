#ifndef BLOCK_H
#define BLOCK_H

struct block{
  block():listen_port_num(-1){
    bzero(&host,sizeof(host));
    bzero(&ip,sizeof(ip));
  }
  char host[1024];
  char ip[1024];
  int listen_port_num;
};

#endif
