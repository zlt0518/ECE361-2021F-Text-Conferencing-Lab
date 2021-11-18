#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


int processLogInCommand(unsigned char *commandLine[5]);
int processNotInSessionCommand(unsigned char *command[2]);
int processInSessionCommand(unsigned char *command);



#endif
