#include "packet.h"

void sendPck(int s, struct packet encodedPck)
{
    char data_send[1050];
    const int length = sprintf(data, "%d:%d:%s:%s", encodedPck.type, encodedPck.size, encodedPck.source
    , encodedPck.data);
    int sendInfo = send(s, data_send, length, 0);
    if(sendInfo == -1)
    {
        perror("send");
    }
}

struct packet readPck(char* incomingPck)
{
    struct packet decodedPck;
    unsigned char sourceData[1050];

    sscanf(incomingPck, "%d:%d:%[^\n]s", &decodedPck.type, &decodedPck.size, sourceData);
    // split source and data
    char* colon;
    colon = strchr ((char*) sourceData, ':');
    *colon = '\0';
    colon += sizeof(unsigned char);
    strcpy((char*) decodedPck.source, (char*) sourceData);

    strcpy((char*) decodedPck.data, (char*) colon);

    return decodedPck;
}