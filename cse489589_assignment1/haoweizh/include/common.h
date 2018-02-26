#ifndef COMMON_H
#define COMMON_H
#include "info.h"

class common{
protected:
  info information;
public:
  common();

  void print_error(const char* command_str);
  void print_author();
  void print_ip();
  void print_port();
};
#endif
