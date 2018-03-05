#ifndef CLIENT_H
#define CLIENT_H
#include "common.h"

class client:public common{
public:
  client(char *port);
private:
  void break_cmd(const char* cmd,char *&server_ip,char *&server_port);
  bool isvalid(char *server_ip,int port);
};

#endif
