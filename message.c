#include "message.h"

#include "client.h"
#include "server.h"
#include "user.h"

#define MAX_COMMAND_LEN 1000

void sendMsg(int s, struct message encodedM) {
    char data_send[1050];
    const int length = sprintf(data_send, "%d:%d:%s:%s", encodedM.type,encodedM.size, encodedM.source, encodedM.data);
    int sendInfo = send(s, data_send, length, 0);
    if (sendInfo < 0) {
        printf("send");
    }
}

struct message readMsg(char* incomingM) {
    struct message decodedM;
    unsigned char source[1000];
    unsigned char data[1000];

    sscanf(incomingM, "%d:%d:%s:%[^\n]s", &decodedM.type, &decodedM.size,source,data);
    strcpy((char*)decodedM.source, (char*)source);
    strcpy((char*)decodedM.data, (char*)data);

    return decodedM;
}

// maybe just password in the content
struct message createLoginPackage(char* user, char* password) {
    // valid input and encoding the data
    unsigned char* encodedData =
        (unsigned char*)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN);

    strcpy(encodedData, user);
    encodedData[strlen((char*)encodedData)] = ':';
    strcat(encodedData, password);

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
    // valid input
    struct message package;
    package.type = 4;
    strcpy((char*)package.data, (char*)"logout");
    package.size = strlen((char*)"logout");
    strcpy((char*)package.source, user);
    return package;
}

struct message createJoinSessionPackage(char* user, char* sessionID) {
    // valid input
    struct message package;
    package.type = 5;
    strcpy((char*)package.data, sessionID);
    package.size = strlen(sessionID);
    strcpy((char*)package.source, user);
    return package;
}

struct message createLeaveSessionPackage(char* user) {
    // valid input
    struct message package;
    package.type = 8;
    strcpy((char*)package.data, (char*)"leavesession");
    package.size = strlen((char*)"leavesession");
    strcpy((char*)package.source, user);
    return package;
}

struct message createCreateSessionPackage(char* user, char* sessionID) {
    // valid input
    struct message package;
    package.type = 9;
    strcpy((char*)package.data, sessionID);
    package.size = strlen(sessionID);
    strcpy((char*)package.source, user);
    return package;
}

struct message createListPackage(char* user) {
    // valid input
    struct message package;
    package.type = 12;
    strcpy((char*)package.data, (char*)"list");
    package.size = strlen((char*)"list");
    strcpy((char*)package.source, user);
    return package;
}


struct message createtextPackage(char* user,char*text){





    
}
