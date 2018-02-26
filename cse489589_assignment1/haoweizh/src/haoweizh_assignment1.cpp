/**
 * @haoweizh_assignment1
 * @author  Haowei Zhou <haoweizh@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../include/global.h"
#include "../include/logger.h"
#include "../include/server.h"
#include "../include/client.h"

using namespace std;

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
  /* The number of argument should more than 2. */
  if (argc <= 2)
    return 0;

  /*Init. Logger*/
  cse4589_init_log(argv[2]);

  /* Clear LOGFILE*/
  fclose(fopen(LOGFILE, "w"));

  /*Start Here*/
  if (strcmp(argv[1],(char*)"c") == 0)
    client clt(argv[2]);
  if (strcmp(argv[1],(char*)"s") == 0)
    server sev(argv[2]);
  
  return 0;
}
