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

    strcpy((char*) database[0].password, "1" );
    strcpy((char*) database[1].password, "2" );
    strcpy((char*) database[2].password, "3" );
    strcpy((char*) database[3].password, "4" );
    
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

bool login(int sfd, unsigned char* un, unsigned char* pw, unsigned char* reply, unsigned char* source )
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



