#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

void init_database();

bool login(struct sockaddr their_addr,int sfd, unsigned char* un, unsigned char* pw, unsigned char* reply);

void logout(unsigned char* un);

bool joinSession(unsigned char* un, unsigned char* sessionID, unsigned char* reply);

bool createSession(unsigned char* un, unsigned char* sessionID, unsigned char* reply);

void leaveSession(unsigned char* un);

void send_txt(unsigned char* un, unsigned char* txt);

void listUserSession(unsigned char* reply);

#endif
