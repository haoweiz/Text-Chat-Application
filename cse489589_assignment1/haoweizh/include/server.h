#ifndef SERVER_H
#define SERVER_H
#include "common.h"

class server:public common{
public:
  server(char* port);
  bool isvalid(char *server_ip);
};

#endif
