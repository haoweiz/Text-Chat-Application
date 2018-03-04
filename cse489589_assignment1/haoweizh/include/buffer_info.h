#ifndef BUFFER_INFO_H
#define BUFFER_INFO_H

#include <string.h>

struct buffer_info{
  buffer_info(){
    bzero(&des_ip,sizeof(des_ip));
    bzero(&mesg,sizeof(mesg));
    bzero(&fr,sizeof(fr));
  }
  char des_ip[32];
  char mesg[1024];
  char fr[32];
};


#endif
