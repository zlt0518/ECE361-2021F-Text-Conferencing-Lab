#define MAX_USER 6

struct user_info{
    char* username;
    char* password;
    char* sessionID;
    int sockfd;
    bool isLogin;
    bool isInSession;
};

struct user_info database[MAX_USER];

