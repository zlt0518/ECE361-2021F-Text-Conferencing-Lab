#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "server.h"
#include "client.h"
#include "packet.h"
#include "user.h"
#include <sys/types.h>
#include <sys/socket.h>


#define MAXDATASIZE 100


int processLogInCommand(unsigned char* commandLine[5]);



//part of the code is refered from Beej's guide

int main(int argc, char** argv){
     bool isLogin = 0;
     bool isinsession = 0;
     int socket;


     while(true){


       //the state of not login
        while(islogin==0){
            printf("You are not log in! Please input the login information!\n");
 

            unsigned char* command[5];
            for(int i =0;i<5;i++){
                command[i] = (unsigned char*)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN)
            }

            int logincommand = processLogInCommand(*command);
            
            //check the login command by the user

            //if the user asked for quit
            if(logincommand == 0) return 0;

            //command for legal login command
            else if(logincommand == 1){
                


                // establish connection and configure the socket
                struct addrinfo hints;
                struct addrinfo* res;
                memset(&hints, 0, sizeof hints);
                hints.ai_family = AF_INET;  // use IPv4
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

                //get the address and port number from the user
                int returnaddress = getaddrinfo((char *)command[3], (char *)command[4], &hints, &res);
                if(returnaddress < 0){
                    printf("Invalid IP Address or Port Number. Please try again!\n");
                    continue;
                }

                socket = socket(res->ai_family, SOCK_STREAM, res->ai_protocol);

                if(socket<0){
                    printf("Socket Error!\n");
                    continue;
                }

                int connnectToServer = connect(socket, res->ai_addr, res->ai_addrlen);

                if (connnectToServer<0){
                    printf("Fail to connect to the server! Please try again!\n");
                    continue;
                }
                //create the package for sending the log in info package



 
                //receive the package of acknowledge of yes or no.

                







            }else{
                //other log in comman
                continue;


            }





            //listen and receive the message of acknowledegement and check if the login info matched with the system 



            //if the message confirms the login go to other loop and set the islogin else promt for other trial












        }














     }














    return 0;
}




int processLogInCommand(unsigned char* command[5]){
    //process the login command

    /* 
    return 2 for illegal command
    return 1 for get the info for the next step
    return 0 for quit
    */
    unsigned char incomingMsg[MAXDATASIZE];
    memset (incomingMsg, '\0', sizeof(unsigned char) * MAXDATASIZE);
    sscanf((char*) incomingMsg, "%s", (char*)command[0]);

    if(strcmp(commandLine[0], "/login")==0){

        //possible issue of less or more argument
        sscanf((char*) incomingMsg, "%s %s %s %s %s", (char*)command[0],(char*)command[1],(char*)command[2],(char*)command[3],(char*)command[4]);

        return 1;

    }else if(strcmp(commandLine[0], "/quit")==0){
       return 0; 

    }else{

       return 2;

    }







}