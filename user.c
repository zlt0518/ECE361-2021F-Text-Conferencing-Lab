#include "message.h"
#include "user.h"
#include "server.h"

void init_database()
{
    for(int i = 0; i < MAX_USER; i++)
    {
        database[i].isInSession = false;
        database[i].isLogin = false;
        strcpy((char*) database[i].sessionID, "/0" );
    }
    strcpy((char*) database[0].username, "A" );
    strcpy((char*) database[1].username, "B" );
    strcpy((char*) database[2].username, "C" );
    strcpy((char*) database[3].username, "D" );

    strcpy((char*) database[0].password, "ihatenetworks" );
    strcpy((char*) database[1].password, "ece361damn" );
    strcpy((char*) database[2].password, "ece302sucks" );
    strcpy((char*) database[3].password, "ilovealogrithm" );
    
}

void listUserSession(unsigned char* reply)
{
    for(int i =0; i < MAX_USER; i++)
    {
        if(database[i].isLogin)
        {
            strcat((char*) reply, "User ");
            strcat((char*) reply, (char*) database[i].username);
            if(database[i].isInSession)
            {
                strcat((char*) reply, " is in session/");
                strcat((char*) reply, (char*) database[i].sessionID);
                strcat((char*) reply, " ");
            }
            strcat((char*) reply, " ---- ");
        }
    }
}

bool login(struct sockaddr their_addr,int sfd, unsigned char* un, unsigned char* pw, unsigned char* reply )
{
    for(int i = 0; i < MAX_USER; i++)
    {
        if(strcmp((char*) database[i].username, (char*) un) == 0)
        {
            if(database[i].isLogin)
            {
                strcpy((char*) reply, "user already logged in");
                return false;
            }
            else if(strcmp((char*) database[i].password, (char*) pw) == 0)
            {
                struct sockaddr_in *addr_in = (struct sockaddr_in*) &their_addr;
                char *IPAddr = inet_ntoa(addr_in->sin_addr);
                strcpy((char*) database[i].IP, IPAddr);

                database[i].port = addr_in->sin_port;
                database[i].isLogin = true;
                database[i].sockfd = sfd;
                strcpy((char*) reply, "login success");
                return true;
            }
            else
            {
                strcpy((char*) reply, "pw incorrect");
                return false;
            }
        }
    }

    strcpy((char*) reply, "user not found");
    return false;

}

void logout(unsigned char* un)
{
    for(int i = 0; i < MAX_USER; i++)
    {
        if(strcmp((char*) un, (char*) database[i].username) == 0)
        {
            database[i].isInSession = false;
            database[i].isLogin = false;
        }
    }
}

void leaveSession(unsigned char* un)
{
    for(int i = 0; i < MAX_USER; i++)
    {
        if(strcmp((char*) un, (char*) database[i].username) == 0)
        {
            database[i].isInSession = false;
        }
    }
}

void createSession(unsigned char* un, unsigned char* sessionID, unsigned char* reply)
{
    for(int i = 0; i < MAX_USER ; i++)
    {
        if(strcmp((char*) un, (char*) database[i].username) == 0)
        {
            database[i].isInSession = true;
            strcpy((char*) database[i].sessionID, (char*) sessionID);
            strcpy((char*) reply, "successfully created new session");
        }
    }
}

bool joinSession(unsigned char* un, unsigned char* sessionID, unsigned char* reply)
{
    bool sessionValid = false;
    for(int i =0; i < MAX_USER; i++)
    {
        if(database[i].isInSession)
        {
            if(strcmp((char*) database[i].sessionID, (char*) sessionID) == 0)
            sessionValid = true;
            break;
        }
    }

    if(!sessionValid)
    {
        strcpy((char*) reply, "session not found");
        return sessionValid;
    }
    else
    {
        for(int i =0; i < MAX_USER; i++)
        {
            if(strcmp((char*) database[i].username, (char*) un) == 0)
            {
                database[i].isInSession = true;
                strcpy((char*)database[i].sessionID, (char*)sessionID);
                strcpy((char*) reply, "successfully joined designated session");
                return sessionValid; 
            }
        }
    }

    return sessionValid;
}

void send_txt(unsigned char* un, unsigned char* txt)
{
    unsigned char crtSession[100];
    
    for(int i = 0; i < MAX_USER; i++)
    {
        if(strcmp((char*) un, (char*) database[i].username) == 0)
        {
            if(!database[i].isInSession)
            {
                printf("client not in session!");
                return;
            }
            else
            {
                strcpy((char*) crtSession, (char*) database[i].sessionID);
                break;
            }
        }

        if(i == MAX_USER -1)
        {
            printf("user not found!");
            return;
        }
    }

    struct message sendM;
    memset(sendM.data, 0, sizeof sendM.data);
    strcpy((char*) sendM.data, (char*) un);
    strcpy((char*) sendM.source, "server");
    strcat((char*) sendM.data, "----=");
    strcat((char*) sendM.data, (char*) txt);
    sendM.size = strlen((char*) sendM.data);
    sendM.type = 11;

    for(int i =0 ; i < MAX_USER; i++)
    {
        if(database[i].isInSession)
        {
            if(strcmp((char*) crtSession, (char*) database[i].sessionID) == 0
                && strcmp((char*) un, (char*) database[i].username) != 0)
            {
                if(sendMsg(database[i].sockfd, sendM) == -1)
                {
                    printf("send error\n");
                    return;
                }
            }
        }
    }

    printf("finished sending text in user designated session");
}




