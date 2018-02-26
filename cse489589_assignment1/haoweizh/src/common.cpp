#include "../include/logger.h"
#include "../include/common.h"
#include <iostream>
#include <string.h>
using namespace std;

common::common(){
}

void common::print_error(const char* command_str){
  cse4589_print_and_log("[%s:ERROR]\n",command_str);
  cse4589_print_and_log("[%s:END]\n",command_str);
}
void common::print_author(){
  cse4589_print_and_log("[AUTHOR:SUCCESS]\n");
  cse4589_print_and_log("I, haoweizh, have read and understood the course academic integrity policy.\n");
  cse4589_print_and_log("[AUTHOR:END]\n");
}
void common::print_ip(){
  cse4589_print_and_log("[IP:SUCCESS]\n");
  cse4589_print_and_log("IP:%s\n",information.ip_address);
  cse4589_print_and_log("[IP:END]\n");
}
void common::print_port(){
  cse4589_print_and_log("[PORT:SUCCESS]\n");
  cse4589_print_and_log("PORT:%s\n",information.port_number);
  cse4589_print_and_log("[PORT:END]\n");
}

