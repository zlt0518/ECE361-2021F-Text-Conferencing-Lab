#include "message.h"

void sendMsg(int s, struct message encodedM)
{
    char data_send[1050];
    const int length = sprintf(data_send, "%d:%d:%s:%s", encodedM.type, encodedM.size, encodedM.source
    , encodedM.data);
    int sendInfo = send(s, data_send, length, 0);
    if(sendInfo == -1)
    {
        perror("send");
    }
}

struct message readMsg(char* incomingM)
{
    struct message decodedM;
    unsigned char sourceData[1050];

    sscanf(incomingM, "%d:%d:%[^\n]s", &decodedM.type, &decodedM.size, sourceData);
    // split source and data
    char* colon;
    colon = strchr ((char*) sourceData, ':');
    *colon = '\0';
    colon += sizeof(unsigned char);
    strcpy((char*) decodedM.source, (char*) sourceData);

    strcpy((char*) decodedM.data, (char*) colon);

    return decodedM;
}