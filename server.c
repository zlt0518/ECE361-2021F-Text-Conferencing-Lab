#include "message.h"
#include "server.h"
#include "user.h"

#define buffer_size 1050

//part of the code is derived from Beej's guide

int processIncomingM(int s, unsigned char* buffer, unsigned char* data_fill, unsigned char* source);

int main(int argc, char** argv){

    init_database();

    fd_set master; // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    int fdmax;
    int i, j, listener, newfd;
    char buffer[buffer_size];

    //get the port number
    if (argc !=2) {
        printf("Invalid number of arguments!");
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

    int listener = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //check if the socket is legel
    if (listener < 0){
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
    if(listen(listener, 10) == -1){
        perror("listen Error");
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
            perror("select");
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
                        perror("accept");
                    } 
                    else 
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) 
                        { // keep track of the max
                            fdmax = newfd;
                        }
                        struct message sendM;
                        memset(sendM.data, 0, sizeof sendM.data);
                        sendM.size = strlen((char*) sendM.data);
                        sendM.type = processIncomingM(newfd, buffer, sendM.data, sendM.source);
                        

                        // send reply
                        if(sendM.type != 15){
                            sendMsg(newfd, sendM);
                            printf("[INFO] ACK back to client.\n");
                        }
                    }
                } 
                else 
                {
                   int nbytes;
                    if ((nbytes = recv(i, buffer, buffer_size-1, 0)) == -1) {
                        perror("recv");
                        exit(1);
                    }
                    buffer[nbytes] = '\0';

                    // create reply msg
                    struct message sendM;
                    memset(sendM.data, 0, sizeof sendM.data);
                    sendM.size = strlen((char*) sendM.data);
                    sendM.type = processIncomingM(i, buffer, sendM.data, sendM.source);
                    

                    // send reply
                    if(sendM.type != 11 && sendM.type != 15){
                        sendMsg(newfd, sendM);
                        printf("[INFO] ACK back to client.\n");
                    }
                    if(sendM.type == 3 || sendM.type == 14){
                        close(i);
                        FD_CLR(i, &master);
                    }
                    if(sendM.type == 3){
                        printf("client is logged in or wrong ID/PW.\n");
                    }else if(sendM.type == 14){
                        printf("Client Logout.\n");
                    }
                } // END handle data from client
            } // END got new incoming connection
        } // END looping through file descriptors
        
    } // END while(true)--and you thought it would never end!

    return 0;
}

int processIncomingM(int s, unsigned char* buffer, unsigned char* data_fill, unsigned char* source)
{
    struct message recvM = readMsg(buffer);
    strcpy((char*) source, "server");

    switch(recvM.type)
    {
        case 1:
            unsigned char un[buffer_size];
            unsigned char pw[buffer_size];

            char* space;
            space = strchr((char*) recvM.data, ' ');
            *space = '\0';
            strcpy((char*) un, (char*) recvM.data);
            strcpy((char*) pw, space);

            if(login(s,un,pw,data_fill,source))
            {
                return 2;
            }
            else
            {
                return 3;
            }
            break;
        case 4:
            leaveSession(recvM.source);
            logout(source);
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
            break;

    }

    return -1;
}