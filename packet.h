#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

struct packet{
	unsigned int type;
	unsigned int size;
	unsigned char source[1050];
	unsigned char data[1050];
};

struct connection {
    bool isConnected;
    bool isInSession;
    unsigned char source[1050];
    unsigned char destAddr[1050];
    unsigned char destPort[1050];
    unsigned char portNum[10];
};

void sendPck(int s, struct message encodedPck);
struct message readPck(char* incomingPck);

// message types
/*
    1: [client -> server] login
        <clientID, clientPW>
    2: [server -> client] acknowledge successful login
    3: [server -> client] negative acknowledgement successful login
        <failureMsg>
    4: [client -> server] exit from the server
    5: [client -> server] join a session
        <sessionID>
    6: [server -> client] acknowledge successful join
        <sessionID>
    7: [server -> client] acknowledge unsuccessful join
        <sessionID, failureMsg>
    8: [client -> server] leave session
    9: [client -> server] create new session and join
        <sessionID>
    10:  [server -> client] acknowledge new session
    11:  [client -> server] send message in joined session
        <msg>
    12:  [client -> server] query for list of online users and avaliable sessions
    13:  [server -> client] reply query in message type 12
        <users and sessions list>

	14: [server -> client] ack for exit

*/
#endif