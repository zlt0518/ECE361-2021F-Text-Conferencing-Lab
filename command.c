#include "client.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "message.h"
#include "server.h"
#include "user.h"
#include "command.h"

#define MAXDATASIZE 1000
#define MAX_NAME 1000
#define MAX_COMMAND_LEN 1000

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
    setbuf(stdin,NULL);

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
    setbuf(stdin,NULL);

    printf("1");
    sscanf((char *)incomingMsg, "%s", (char *)command[0]);
    printf("%s\n", (char *)command[0]);
    printf("%s\n", (char *)incomingMsg);

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

    return 4;
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
    setbuf(stdin,NULL);

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
        return 4;
    }
}