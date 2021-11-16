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
#include <random.h>
#include "packet.h"

#define buffer_size 1050

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

    // bind()
    int bindinfo = bind(s, res->ai_addr, res->ai_addrlen);

    //check if the binding is success else return the failure message
    if (bindinfo == -1) {
        printf("Binding failure");
        exit (1);
    }

    // start listening to the socket
    if(listen(s, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    struct sockaddr their_addr; // connector's address information
    socklen_t sin_size;

    char buf[buffer_size];

    while(true)
    {
        int new_sockfd;
        sin_size = sizeof their_addr;
        new_sockfd = accept(s, (struct sockaddr *)&their_addr, &sin_size);
        if (new_sockfd == -1) {
            perror("accept");
            continue;
        }


        bool conn_shut = false;
        while(!conn_shut)
        {
            int numbytes;
            if ((numbytes = recv(new_fd, buf, buffer_size-1, 0)) == -1) {
                perror("recv");
                exit(1);
            }
            buf[numbytes] = '\0';

            struct packet pck_send;
            memset(pck_send.data, 0, sizeof buffer_size);
            pck_send.type = processIncomingPck(their_addr, buf, pck_send.data);
            strcpy((char*) pck_send.source, "server"); 
            pck_send.size = strlen((char*) pck_send.data);


                if(pck_send.type != 16){
                sendMessage(new_sockfd, pck_send);
                printf("ACK back to client.\n");
            }

            // if drop connection actively, exit or leave session
            if(pck_send.type == 3 || pck_send.type == 14){
                conn_shut = true;
            }
            if(pck_send.type == 3){
                printf("Authentication Faliure or Client already logged in. Refuse connection.\n");
            }else if(pck_send.type == 14){
                printf("Client Logout. Close connection.\n");
            }

        }

        printf("Connection Closed\n");
        close(new_sockfd);
        return 0;

    }


    return 0;
}


// numerical order of the commands in the handout, 14 is exit success, 3 is login fail
int processIncomingPck(struct sockaddr socketID, char* incomingPck, unsigned char* ack_data, unsigned char* source)
{
    struct packet docodedPacket = readPck(incomingPck)
}
// this function process the incoming packet to return a int value as the packet type Identifier