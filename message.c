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

#define MAX_COMMAND_LEN 1000

int sendMsg(int s, struct message encodedMessage) {
    char data_send[1000];
    const int length = sprintf(data_send, "%d:%d:%s:%s", encodedMessage.type,encodedMessage.size, encodedMessage.source, encodedMessage.data);
    int sendInfo = send(s, data_send, length, 0);
    if (sendInfo < 0) {
        printf("send Error");
        return -1;
    }
    return 1;

}


struct message readMsg(char* incomingM) {
    struct message decodedM;
    unsigned char readstring[1000];

    sscanf(incomingM, "%d:%d:%[^\n]s", &decodedM.type, &decodedM.size,readstring);

    char* colon = strchr(readstring, ':');
    *colon = '\0';
    colon += sizeof(unsigned char);

    strcpy((char*)decodedM.source, (char*)readstring);
    strncpy((char*)decodedM.data, (char*)colon, decodedM.size+1);

    return decodedM;
}

// maybe just password in the content
struct message createLoginPackage(char* user, char* password) {
    char* encodedData = (char*)malloc(sizeof(char) * MAX_COMMAND_LEN);

    strcpy(encodedData, password);
    //encodedData[strlen((char*)encodedData)] = ':';
    //strcat(encodedData, password);

    struct message package;
    package.type = 1;
    strcpy((char*)package.data, (char*) encodedData);
    package.size = strlen((char*)encodedData);
    strcpy((char*)package.source, user);

    // free the pointer
    free(encodedData);

    return package;
}

struct message createLogoutPackage(char* user) {

    struct message package;
    package.type = 4;
    strcpy((char*)package.data, (char*)"logout");
    package.size = strlen((char*)"logout");
    strcpy((char*)package.source, user);
    return package;
}

struct message createJoinSessionPackage(char* user, char* sessionID) {

    struct message package;
    package.type = 5;
    strcpy((char*)package.data, sessionID);
    package.size = strlen(sessionID);
    strcpy((char*)package.source, user);
    return package;
}

struct message createLeaveSessionPackage(char* user) {

    struct message package;
    package.type = 8;
    strcpy((char*)package.data, (char*)"leavesession");
    package.size = strlen((char*)"leavesession");
    strcpy((char*)package.source, user);
    return package;
}

struct message createCreateSessionPackage(char* user, char* sessionID) {

    struct message package;
    package.type = 9;
    strcpy((char*)package.data, sessionID);
    package.size = strlen(sessionID);
    strcpy((char*)package.source, user);
    return package;
}

struct message createListPackage(char* user) {
    struct message package;
    package.type = 12;
    strcpy((char*)package.data, (char*)"list");
    package.size = strlen((char*)"list");
    strcpy((char*)package.source, user);
    return package;
}


struct message createtextPackage(char* user,char*text){
    struct message package;
    package.type = 11;
    strcpy((char*)package.data, text);
    package.size = strlen(text);
    strcpy((char*)package.source, user);
    return package;
    
}

struct message createPMPackage(char* user, char* target, char* text)
{
    struct message package;
    package.type = 17;
    strcpy((char*)package.data, "Private message-> User ");
    strcat((char*)package.data,(char*) user);
    strcat((char*)package.data,"----=");
    strcat((char*)package.data,(char*) text);
    
    package.size = strlen(package.data);
    strcpy((char*)package.source, target); // source is actually target client ID, server only need to deliver to  
    return package;                         // client, dont really need to know who sent the PM
}

struct message createInvitePackage(char* user, char* target)
{
    struct message package;
    package.type = 18;
    strcpy((char*)package.data, target);
    
    package.size = strlen(package.data);
    strcpy((char*)package.source, user);  
    return package;
}