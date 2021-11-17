#include "client.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "packet.h"
#include "server.h"
#include "user.h"

#define MAXDATASIZE 100

int processLogInCommand(unsigned char *commandLine[5]);
int processNotInSessionCommand(unsigned char *command[2]);
int processInSessionCommand(unsigned char *command);

// part of the code is refered from Beej's guide

int main(int argc, char **argv) {
    // declare the flag and socket
    bool isLogin = 0;
    bool isinsession = 0;
    int socket;

    while (true) {
        // the state of not login
        while (islogin == 0) {
            printf("You are not log in! Please input the login information!\n");

            unsigned char *logInCommandInput[5];
            for (int i = 0; i < 5; i++) {
                logInCommandInput[i] = (unsigned char *)malloc(
                    sizeof(unsigned char) * MAX_COMMAND_LEN)
            }

            int logincommand = processLogInCommand(logInCommandInput);

            // check the login command by the user

            // if the user asked for quit
            if (logincommand == 0) return 0;

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
                int returnAddress =
                    getaddrinfo((char *)logInCommandInput[3],
                                (char *)logInCommandInput[4], &hints, &res);
                if (returnAddress < 0) {
                    printf(
                        "Invalid IP Address or Port Number. Please try "
                        "again!\n");
                    continue;
                }

                socket = socket(res->ai_family, SOCK_STREAM, res->ai_protocol);

                if (socket < 0) {
                    printf("Socket Error!\n");
                    continue;
                }

                int connnectToServer =
                    connect(socket, res->ai_addr, res->ai_addrlen);

                if (connnectToServer < 0) {
                    printf(
                        "Fail to connect to the server! Please try again!\n");
                    continue;
                }
                // create the package for sending the log in info package

                // receive the package of acknowledge of yes or no.
            } else {
                // other log in command
            }
        }
        // in the state of log in but not in session
        while ((islogin == 1) && (isinsession == 0)) {
            printf("You have logged in! Please input the instruction!\n");
            unsigned char *notInSessionCommandInput[2];

            for (int i = 0; i < 2; i++) {
                notInSessionCommandInput[i] = (unsigned char *)malloc(
                    sizeof(unsigned char) * MAX_COMMAND_LEN)
            }
            // process the command input
            int notInSessioncommand =
                processLogInCommand(notInSessionCommandInput);

            // command for quit the client
            if (notInSessioncommand == 0) {
                printf("Client terminaton\n");
                // send out the package for quit

                // need to free the pointer
                return 0;
            } else if (notInSessioncommand == 1) {
                // command for join session
                // create the join session package and send

                // change the flag
                isinsession = 1;
            } else if (notInSessioncommand == 2) {
                // command for create session
                // create the create session package and send

                // change the flag
                isinsession = 1;

            } else if (notInSessioncommand == 3) {
                printf("Requested to log out!\n");
                // command for log out
                // create package of logout and send

                // change the flag
                isLogin = 0;
                printf("Log out succeeded!\n");

            } else if (notInSessioncommand == 4) {
                printf("Requesting for the current list of sessions and users!\n");

                // command for list
                // create package of list and send

                //listen for the package of list 



            } else {
                printf("Ilegal Command, Please input an valid command!");

            }

            // free the command pointer
        }

        // in the state of in session;
        while ((islogin == 1) && (isinsession == 0)) {
            printf("You are in the chat session Now! Please type in word to send or commands!\n")
            unsigned char *inSessionCommandInput;
            inSessionCommandInput = (unsigned char *)malloc(
                sizeof(unsigned char) * MAX_COMMAND_LEN);

            // process the command input
            int inSessioncommand = processLogInCommand(inSessionCommandInput);

            if(inSessioncommand==0){
                printf("Client terminaton\n");
                // command for quit session

                //free pointers
                return 0;

            }else if(inSessioncommand ==1){
                // command for leave session
                // create the leave session package and send
                


                // change the flag
                isinsession = 0;



            }else if(inSessioncommand ==2){
                // command for list
                // create package of list and send



            }else if(insessioncommand == 3){
                // command for log out
                // create package of leave session and logout and send
                
                // change the flag
                isinsession = 0;
                isLogin = 0;






            }else if(insessioncommand == 4){
                 //create the message to sent
            }

            //free pointers
        }





    }

    return 0;
}

int processLogInCommand(unsigned char *command[5]) {
    // process the login command

    /*
    return 2 for illegal command
    return 1 for get the info for the next step
    return 0 for quit
    */
    unsigned char incomingMsg[MAXDATASIZE];
    memset(incomingMsg, '\0', sizeof(unsigned char) * MAXDATASIZE);
    scanf("%[^\n]s", incomingMsg);

    sscanf((char *)incomingMsg, "%s", (char *)command[0]);

    if (strcmp(command[0], "/login") == 0) {
        // possible issue of less or more argument
        sscanf((char *)incomingMsg, "%s %s %s %s %s", (char *)command[0],
               (char *)command[1], (char *)command[2], (char *)command[3],
               (char *)command[4]);

        return 1;
    } else if (strcmp(command[0], "/quit") == 0) {
        return 0;
    } else {
        return 2;
    }

    return 2;
}

int processNotInSessionCommand(unsigned char *command[2]) {
    /*
    return 1 for join the session
    return 2 for create session
    return 3 for log out
    return 4 for list
    return 5 for illegal command
    return 0 for quit
    */

    unsigned char incomingMsg[MAXDATASIZE];
    memset(incomingMsg, '\0', sizeof(unsigned char) * MAXDATASIZE);
    scanf("%[^\n]s", incomingMsg);

    sscanf((char *)incomingMsg, "%s", (char *)command[0]);

    if (strcmp(command[0], "/joinsession") == 0) {
        sscanf((char *)incomingMsg, "%s %s", (char *)command[0],
               (char *)command[1]);
        return 1;
    } else if (strcmp(command[0], "/createsession") == 0) {
        sscanf((char *)incomingMsg, "%s %s", (char *)command[0],
               (char *)command[1]);
        return 2;
    } else if (strcmp(command[0], "/logout") == 0) {
        return 3;
    } else if (strcmp(command[0], "/list") == 0) {
        return 4;
    } else if (strcmp(command[0], "/quit") == 0) {
        return 0;
    } else {
        return 5;
    }
}



int processInSessionCommand(unsigned char *command) {
    /*
    return 1 for leave the session
    return 2 for list
    return 3 for logout
    return 4 for senetence sent
    return 0 for quit
    */

    unsigned char incomingMsg[MAXDATASIZE];
    memset(incomingMsg, '\0', sizeof(unsigned char) * MAXDATASIZE);
    scanf("%[^\n]s", incomingMsg);

    sscanf((char *)incomingMsg, "%s", (char *)command);

    if (strcmp(command, "/leavesession") == 0) {
        return 1;

    } else if (strcmp(command, "/logout") == 0) {
        return 3;

    } else if (strcmp(command, "/list") == 0) {
        return 2;

    } else if (strcmp(command, "/quit") == 0) {
        return 0;

    } else {
        return 4
    }
}