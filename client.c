
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "message.h"
#include "server.h"
#include "user.h"
#include "client.h"
#include "command.h"

#define MAXDATASIZE 1000
#define MAX_NAME 1000
#define MAX_COMMAND_LEN 1000



// part of the code is refered from Beej's guide

int main(int argc, char **argv) {
    // declare the flag and socket
    bool isLogin = 0;
    bool isinsession = 0;
    int soc;
    char buffer[MAXDATASIZE];
    unsigned char userID[MAX_NAME];

    fd_set read_fds;
    fd_set master;    // master file descriptor list
    FD_ZERO(&read_fds);
    FD_ZERO(&master);    // clear the master and temp sets

    while (true) {
        // the state of not login
        while (isLogin == 0) {
            printf("You are not log in! Please input the login information!\n");

            unsigned char *logInCommandInput[5];
            for (int i = 0; i < 5; i++) {
                logInCommandInput[i] = (unsigned char *)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN);

            }

            int logincommand = processLogInCommand(logInCommandInput);

            // check the login command by the user

            // if the user asked for quit
            if (logincommand == 0){
                //free pointers
                for (int i = 0; i < 5; i++) free(logInCommandInput[i]);

                return 0;

            }

            // command for legal login command
            else if (logincommand == 1) {
                // establish connection and configure the socket
                struct addrinfo hints;
                struct addrinfo *res;
                memset(&hints, 0, sizeof hints);
                hints.ai_family = AF_INET;  // use IPv4
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_flags = AI_PASSIVE;  // fill in my IP for me

                // get the address and port number from the user
                int returnAddress =getaddrinfo((char *)logInCommandInput[3],(char *)logInCommandInput[4], &hints, &res);
                if (returnAddress < 0) {
                    printf("Invalid IP Address or Port Number. Please try again!\n");
                    for (int i = 0; i < 5; i++) free(logInCommandInput[i]);
                    continue;
                }

                soc = socket(res->ai_family, SOCK_STREAM, res->ai_protocol);

                if (soc < 0) {
                    printf("Socket Error!\n");
                    for (int i = 0; i < 5; i++) free(logInCommandInput[i]);
                    continue;
                }

                int connnectToServer = connect(soc, res->ai_addr, res->ai_addrlen);

                if (connnectToServer < 0) {
                    printf("Fail to connect to the server! Please try again!\n");
                    for (int i = 0; i < 5; i++) free(logInCommandInput[i]);
                    continue;
                }

                strcpy(userID,logInCommandInput[1]);

                // create the package for sending the log in info package
                struct message unLoginPackage = createLoginPackage(userID, logInCommandInput[2]);
                //printf("%d  %d  %s  %s\n", unLoginPackage.type, unLoginPackage.size, unLoginPackage.source, unLoginPackage.data);
                int sendByte = sendMsg(soc,unLoginPackage);

                if(sendByte<0){

                    for (int i = 0; i < 5; i++) free(logInCommandInput[i]);
                    continue;

                }


                // receive the package of acknowledge of yes or no
                // clear buffer before receiving new content
                for(int m = 0; m < MAXDATASIZE;m++)
                {
                    buffer[m] = '\0';
                }

                int unLoginByte = recv(soc, buffer, MAXDATASIZE-1, 0);

                if(unLoginByte<0){
                    printf("Error receiving\n");
                    //free pointers
                    for (int i = 0; i < 5; i++) free(logInCommandInput[i]);
                    continue;
                }
                struct message decodedMsg = readMsg(buffer);  

                if(decodedMsg.type == 2){
                    printf("Successfully login!\n");
                    isLogin = 1;
                }else {
                    printf("Failed to login!\n%s\n", decodedMsg.data);
                }


            } else{
                printf("Invalid Command!\n");
            }

            for (int i = 0; i < 5; i++) free(logInCommandInput[i]);


        }


        if((isLogin == 1) && (isinsession == 0))
        {
            FD_SET(soc, &master);
            FD_SET(0,&master);
            // add the socket to server and standard input socket to master fd_set
            printf("You are logged in Now! Please type in commands!\n");

        }

        // in the state of log in but not in session
        while ((isLogin == 1) && (isinsession == 0)) {

            read_fds = master;

            int selectVal = select(soc+1, &read_fds, NULL, NULL, NULL);

            if(selectVal<0){
                printf("Fail to do the select!\n");
                continue;
            }

            if (FD_ISSET(soc, &read_fds)){
                
                for(int m = 0; m < MAXDATASIZE;m++)
                {
                    buffer[m] = '\0';
                }

                int loginByte = recv(soc, buffer, MAXDATASIZE-1, 0);     

                if(loginByte<0){
                    printf("Error receiving\n");
                    continue;
                }

                struct message decodedMsg = readMsg(buffer);

                if(decodedMsg.type == 6){

                    printf("Successfully joined the session \n");
                    isinsession = 1;

                }
                else if(decodedMsg.type == 7){
                    printf("Failed to join the session %s\n", decodedMsg.data);

                }
                else if(decodedMsg.type == 10){

                    printf("Successfully created the session \n");
                    //change the flag
                    isinsession = 1;

                }
                else if(decodedMsg.type == 13){
                    printf("Successfully get the List of user and session\n");
                    printf("%s\n", decodedMsg.data);
                    //print out the list

                }
                else if(decodedMsg.type == 16){
                    printf("Failed to create the session, %s\n", decodedMsg.data);
                }
                else if(decodedMsg.type == 17){
                    printf("%s\n", decodedMsg.data);
                }
                else{
                    printf("Failed to query the list\n");
                }


            }
            else if (FD_ISSET(0, &read_fds)){

                unsigned char *notInSessionCommandInput[3];
                struct message loginPackage;

                for (int i = 0; i < 3; i++) {
                    notInSessionCommandInput[i] = (unsigned char *)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN);
                }

                // process the command input
                int notInSessioncommand = processNotInSessionCommand(notInSessionCommandInput);

                //printf("%d\n", &notInSessioncommand);
                // command for quit the client
                if (notInSessioncommand == 0) {
                    printf("Client terminaton\n");

                    // send out the package for log out
                    loginPackage = createLogoutPackage(userID);
                    int sendByte = sendMsg(soc,loginPackage);
                    if(sendByte == -1)
                    {
                        printf("send error\n");
                    }

                    // need to free the pointer
                    for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                    return 0;

                } else if (notInSessioncommand == 1) {
                    // command for join session
                    // create the join session package and send
                    loginPackage = createJoinSessionPackage(userID,notInSessionCommandInput[1]);
                    int sendByte = sendMsg(soc,loginPackage);


                    if(sendByte<0){

                        for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                        continue;

                    }

                    /*
                    //wait for ACK
                    // clear buffer before receiving new content
                    for(int m = 0; m < MAXDATASIZE;m++)
                    {
                        buffer[m] = '\0';
                    }

                    int loginByte = recv(soc, buffer, MAXDATASIZE-1, 0);

                    if(loginByte<0){
                        printf("Error receiving\n");
                        //free pointers
                        for (int i = 0; i < 2; i++) free(notInSessionCommandInput[i]);
                        continue;
                    }

                    struct message decodedMsg = readMsg(buffer); 

                    if(decodedMsg.type == 6){

                        printf("Successfully joined the session %s\n",notInSessionCommandInput[1]);
                        isinsession = 1;

                    }else{
                        printf("Failed to join the session %s\n", decodedMsg.data);

                    }
                    */
                    
                } else if (notInSessioncommand == 2) {
                    // command for create session
                    // create the create session package and send
                    loginPackage = createCreateSessionPackage(userID,notInSessionCommandInput[1]);
                    int sendByte = sendMsg(soc,loginPackage);

                    if(sendByte<0){

                        for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                        continue;

                    }

                    /*
                    // wait for ack
                    // clear buffer before receiving new content
                    for(int m = 0; m < MAXDATASIZE;m++)
                    {
                        buffer[m] = '\0';
                    }

                    int loginByte = recv(soc, buffer, MAXDATASIZE-1, 0);     

                    if(loginByte<0){
                        printf("Error receiving\n");
                        //free pointers
                        for (int i = 0; i < 2; i++) free(notInSessionCommandInput[i]);
                        continue;
                    }

                    struct message decodedMsg = readMsg(buffer);

                    if(decodedMsg.type == 10){

                        printf("Successfully created the session %s\n",notInSessionCommandInput[1]);
                        //change the flag
                        isinsession = 1;

                    }else{
                        printf("Failed to create the session, %s\n", decodedMsg.data);
                    }
                    */


                } else if (notInSessioncommand == 3) {
                    printf("Requested to log out!\n");

                    // command for log out
                    // send out the package for log out
                    loginPackage = createLogoutPackage(userID);
                    int sendByte = sendMsg(soc,loginPackage);

                    if(sendByte<0){

                        for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                        continue;

                    }


                    // change the flag
                    isLogin = 0;
                    printf("Log out succeeded!\n");

                } else if (notInSessioncommand == 4) {
                    printf("Requesting for the current list of sessions and users!\n");

                    // command for list
                    // create package of list and send
                    loginPackage = createListPackage(userID);
                    if(sendMsg(soc,loginPackage) == -1)
                    {
                        printf("Send error\n");
                        for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                        continue;
                    }


                    /*
                    //listen for the package of list 
                    //wait for ACK
                    // clear buffer before receiving new content
                    for(int m = 0; m < MAXDATASIZE;m++)
                    {
                        buffer[m] = '\0';
                    }

                    int loginByte = recv(soc, buffer, MAXDATASIZE-1, 0);

                    if(loginByte<0){
                        printf("Error receiving\n");
                        //free pointers
                        for (int i = 0; i < 2; i++) free(notInSessionCommandInput[i]);
                        continue;
                    }

                    struct message decodedMsg = readMsg(buffer);

                    if(decodedMsg.type == 13){
                        printf("Successfully get the List of user and session\n");
                        printf("%s\n", decodedMsg.data);
                        //print out the list

                    }else{
                        printf("Failed to query the list\n");

                    }

                    */


                } else if (notInSessioncommand == 6) {
                    printf("private messaging!\n");

                    // command for private messaging
                    // create package PM and send
                    loginPackage = createPMPackage(userID,notInSessionCommandInput[1],
                                                        notInSessionCommandInput[2]);
                    if(sendMsg(soc,loginPackage) == -1)
                    {
                        printf("Send error\n");
                        for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
                        continue;
                    }
                } else {
                    printf("Ilegal Command, Please input an valid command!\n");

                }

                // free the command pointer
                for (int i = 0; i < 3; i++) free(notInSessionCommandInput[i]);
            }
        }






        // in the state of in session
        //need to add function select for multiplexing

        if((isLogin == 1) && (isinsession == 1))
        {
            printf("You are in the chat session Now! Please type in senetences to send or commands!\n");

        }



        while ((isLogin == 1) && (isinsession == 1)) {

            read_fds = master;

            int selectVal = select(soc+1, &read_fds, NULL, NULL, NULL);

            if(selectVal<0){
                printf("Fail to do the select!\n");
                continue;
            }

            if (FD_ISSET(soc, &read_fds)){
                //receive the message
                // clear buffer before receiving new content
                for(int m = 0; m < MAXDATASIZE;m++)
                {
                    buffer[m] = '\0';
                }

                int insessionByte = recv(soc, buffer, MAXDATASIZE, 0);
                //deal with the message phrasing and print out the message
                if(insessionByte == -1)
                {
                    printf("Error receiving\n");
                    continue;
                }
                struct message decodedMsg = readMsg(buffer);

                if(decodedMsg.type == 11)
                {
                    printf("%s\n", decodedMsg.data);
                }
                else if(decodedMsg.type == 17)
                {
                    printf("%s\n", decodedMsg.data);
                }
                


            }else if (FD_ISSET(0, &read_fds)){
            //send message
            unsigned char *inSessionCommandInput[3];
            for (int i = 0; i < 3; i++) {
                inSessionCommandInput[i] = (unsigned char *)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN);
            }
            struct message inSesssionPackage;

            // process the command input
            int inSessioncommand = processInSessionCommand(inSessionCommandInput);
                if(inSessioncommand==0){
                    printf("Client terminaton\n");
                    // command for quit session
                    //create package of complete log out and send

                    inSesssionPackage = createLogoutPackage(userID);
                    if(sendMsg(soc,inSesssionPackage) == -1)
                    {
                        printf("send error\n");
                        for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                        continue;
                    }

                    close(soc);
                    FD_CLR(soc,&master);
                    FD_CLR(soc,&read_fds);
                    
                    
                    //free pointers
                    for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                    return 0;

                }else if(inSessioncommand ==1){
                    // command for leave session
                    // create the leave session package and send
                    inSesssionPackage = createLeaveSessionPackage(userID);
                    if(sendMsg(soc,inSesssionPackage) == -1)
                    {
                        printf("send error\n");
                        for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                        continue;
                    }

                   // change the flag
                   isinsession = 0;

                }else if(inSessioncommand == 3){
                // command for log out
                // create package of leave session and logout and send
                //inSesssionPackage = createLeaveSessionPackage(userID);

                inSesssionPackage = createLogoutPackage(userID);
                if(sendMsg(soc,inSesssionPackage) == -1)
                {
                    printf("send error\n");
                    for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                    continue;
                }
                
                close(soc);
                FD_CLR(soc,&master);
                FD_CLR(soc,&read_fds);
                // change the flag
                isinsession = 0;
                isLogin = 0;

                }else if(inSessioncommand == 2){

                    printf("You can't ask for list when you are in session\n");
                
                
                }else if(inSessioncommand == 5){
                //create the private message and send
                inSesssionPackage = createPMPackage(userID,(char*) inSessionCommandInput[1],
                                                            (char*) inSessionCommandInput[2]);
                if(sendMsg(soc,inSesssionPackage) == -1)
                {
                    printf("send error\n");
                    for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                    continue;
                }

                }else{
                //create the message and send
                inSesssionPackage = createtextPackage(userID,(char*) inSessionCommandInput[0]);
                if(sendMsg(soc,inSesssionPackage) == -1)
                {
                    printf("send error\n");
                    for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}
                    continue;
                }

                }

                for (int i = 0; i < 3; i++) { free(inSessionCommandInput[i]);}

            }

        }





    }

    return 0;
}



