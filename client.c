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


     while(true){


       //the state of not login
        while(islogin==0){
            printf("You are not log in! Please input the login information!\n");
 

            unsigned char* command[5];
            for(int i =0;i<5;i++){
                command[i] = (unsigned char*)malloc(sizeof(unsigned char) * MAX_COMMAND_LEN)
            }

            processLogInCommand(*command)





            //check if the instruction is to login or quit else return 0





            // establish connection and configure the socket
            struct addrinfo hints;
            struct addrinfo* res;
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET;  // use IPv4
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;     // fill in my IP for me


            //conduct the connection


            //check if the connection succeeded else promot for other triel



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
    return 1 for get the info
    return 0 for quit
    */


    unsigned char incomingMsg[MAXDATASIZE];
    memset (incomingMsg, '\0', sizeof(unsigned char) * MAXDATASIZE);
    sscanf((char*) incomingMsg, "%s", (char*)command[0]);

    if(strcmp(commandLine[0], "/login")){

        sscanf((char*) incomingMsg, "%s %s %s %s %s", (char*)command[0],(char*)command[1],(char*)command[2],(char*)command[3],(char*)command[4]);



    }else if(strcmp(commandLine[0], "/quit"){
       return 1; 
    }else{

       return 0;
    }







}