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
#define MAX_USER 4

struct user_info{
    unsigned char username[100];
    unsigned char password[100];
    unsigned char sessionID[100];
    unsigned char IP[100];
    int port;
    int sockfd;
    bool isLogin;
    bool isInSession;
};

struct user_info database[MAX_USER];

