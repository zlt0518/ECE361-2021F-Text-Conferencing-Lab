#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "client.h"
#include "packet.h"
#include "user.h"
#include <sys/types.h>
#include <sys/socket.h>


//part of the code is refered from Beej's guide

int main(int argc, char** argv){

    //get the port number
    if (argc !=2) {
        printf("Invalid number of arguments!");
        return(1);
    }

    // socket()
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int rv = getaddrinfo(NULL, argv[1], &hints, &res);

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    //check if the socket is legel
    if (s < 0){
        printf("Error Socket in Server");
        return 1;
    }








    return 0;
}