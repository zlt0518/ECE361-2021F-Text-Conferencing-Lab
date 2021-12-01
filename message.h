#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

struct message{
	unsigned int type;
	unsigned int size;
	unsigned char source[1000];
	unsigned char data[1000];
};


int sendMsg(int s, struct message encodedM);
struct message readMsg(char* incomingM);

//function to create message function
struct message createListPackage(char* user);
struct message createLoginPackage(char* user,char*password);
struct message createLogoutPackage(char* user);
struct message createtextPackage(char* user,char*text);
struct message createJoinSessionPackage(char* user,char*sessionID);
struct message createCreateSessionPackage(char* user,char* sessionID);
struct message createLeaveSessionPackage(char* user);
struct message createPMPackage(char* user, char* target, char* text);
struct message createInvitePackage(char* user, char* target);


// message types
/*
    1: login <clientID, clientPW>
    2:  acknowledge successful login
    3:  negative acknowledgement successful login
        <failure>
    4:  exit from the server
    5:  join a session
        <sessionID>
    6:  acknowledge successful join
        <sessionID>
    7:  acknowledge unsuccessful join
        <sessionID, failure>
    8: leave session
    9: create new session and join
        <sessionID>
    10:  acknowledge new session
    11:  send text in joined session
        <text>
    12:  query for list of online users and avaliable sessions
    13:  reply query in message type 12
        <users and sessions list>

	14:ack for exit

    17:private message

    18: session invite

*/
#endif