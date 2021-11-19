#include "message.h"
#include "server.h"
#include "user.h"

#define buffer_size 1000

//part of the code is derived from Beej's guide

int processIncomingM(struct sockaddr their_addr, int s, unsigned char* buffer, unsigned char* data_fill, unsigned char* source);

int main(int argc, char** argv){

    init_database();

    fd_set master; // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    int fdmax;
    int i, j, listener, newfd;
    char buffer[buffer_size];

    //get the port number
    if (argc !=2) {
        printf("Usage: server <port number>\n");
        return(1);
    }

    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);

    // socket()
    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    int rv = getaddrinfo(NULL, argv[1], &hints, &res);

    listener = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //check if the socket is legel
    if (listener < 0){
        printf("Error Socket in Server");
        return 1;
    }

    // bind()
    int bindinfo = bind(listener, res->ai_addr, res->ai_addrlen);

    //check if the binding is success else return the failure message
    if (bindinfo == -1) {
        printf("Binding failure");
        exit (1);
    }

    // start listening to the socket
    if(listen(listener, 10) == -1){
        printf("listen Error");
        exit(1);
    }

    struct sockaddr their_addr; // connector's address information
    socklen_t addrlen;

    FD_SET(listener, &master);
    fdmax = listener;

    while(true) 
    {
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            printf("select");
            exit(4);
        }

        for(i = 0; i <= fdmax; i++) 
        {
            if (FD_ISSET(i, &read_fds)) 
            { // we got one!!
                if (i == listener) 
                {
                    // handle new connections
                    addrlen = sizeof their_addr;
                    newfd = accept(listener,
                    (struct sockaddr *)&their_addr,
                    &addrlen);

                    if (newfd == -1) 
                    {
                        printf("accept");
                    } 
                    else 
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) 
                        { // keep track of the max
                            fdmax = newfd;
                        }
                    }
                } 
                else 
                {
                    // we hear something from connected user
                    int nbytes;
                    if ((nbytes = recv(i, buffer, buffer_size-1, 0)) == -1) {
                        perror("recv\n");
                        continue;
                    }
                    buffer[nbytes] = '\0';

                    // create reply msg
                    struct message sendM;
                    memset(sendM.data, 0, sizeof sendM.data);
                    sendM.type = processIncomingM(their_addr,i, buffer, sendM.data, sendM.source);
                    sendM.size = strlen((char*) sendM.data);

                    printf("%d:%d:%s:%s\n", sendM.type, sendM.size, sendM.source, sendM.data);

                    // send reply
                    if(sendM.type != 11 && sendM.type != 15){
                        if(sendMsg(i, sendM) == -1)
                        {
                            printf("send error\n");
                            break;
                        }
                        printf("Replied client request.\n");
                    }
                    if(sendM.type == 3 || sendM.type == 14){
                        close(i);
                        FD_CLR(i, &master);
                    }
                    if(sendM.type == 3){
                        printf("Client is logged in or wrong ID/PW.\n");
                    }else if(sendM.type == 14){
                        printf("Client Logout.\n");
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
        
    } // END while(true)--and you thought it would never end!

    return 0;
}

int processIncomingM(struct sockaddr their_addr, int s, unsigned char* buffer, unsigned char* data_fill, unsigned char* source)
{
    // read and fill a message struct and deserialize
    struct message recvM = readMsg(buffer);
    strcpy((char*) source, "server");

    printf("%d   %d   %s  %s\n", recvM.type, recvM.size, recvM.source, recvM.data);

    // according to the message type, call different processing functions
    // numbers are in accordance with lab handout chart (14 is logout ack, 15 is leavesession ack, for 
    // server internal use, not actually sent out to client)
    switch(recvM.type)
    {
        case 1:
        {
            if(login(their_addr,s,recvM.source,recvM.data,data_fill))
            {
                return 2;
            }
            else
            {
                return 3;
            }
            break;
        }
        case 4:
            leaveSession(recvM.source);
            logout(recvM.source);
            return 14;
            break;
        case 5:
            if(joinSession(recvM.source,recvM.data,data_fill))
            {
                return 6;
            }
            else
            {
                return 7;
            }
            break;
        case 8:
            leaveSession(recvM.source);
            return 15;
            break;
        case 9:
            createSession(recvM.source, recvM.data, data_fill);
            return 10;
            break;
        case 11:
            send_txt(recvM.source, recvM.data);
            return 11;
            break;
        case 12:
            listUserSession(data_fill);
            return 13;
            break;
        default:
            printf("no such message type\n");

    }

    return -1;
}