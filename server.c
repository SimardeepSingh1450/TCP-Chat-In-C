#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>

void error(char* msg){
    printf(msg);
    exit(1);
}

int main(int argc,char* argv[]){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd<0) error("Error opening socket at server-side");

    struct sockaddr_in server_addr,client_addr;
    socklen_t clientLen;

    bzero((char*)&server_addr,sizeof(server_addr));

    int portno = atoi(argv[1]);

    //assigning values
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //binding
    if(bind(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr))<0){
        error("Error binding...");
    }

    //listen
    listen(sockfd,5);
    clientLen = sizeof(client_addr);

    //accept
    int newSockFd = accept(sockfd,(struct sockaddr*)&client_addr,&clientLen);
    if(newSockFd<0) error("There was an error accepting the connection...");

    int n;
    char buffer[255];
    while(1){
        bzero(buffer,255);
        n = read(newSockFd,buffer,255);
        if(n<0) error("Error in reading message from client-side");
        printf("Message from client-side: %s\n",buffer);

        bzero(buffer,255);
        fgets(buffer,255,stdin);

        n = write(newSockFd,buffer,sizeof(buffer));
        if(n<0) error("Error in writing message to client-side");

        int i = strncmp("exit",buffer,4);
        if(i==0){
            printf("Exit message was received from the client-side");
            break;
        }
    }

    close(newSockFd);
    close(sockfd);
    return 0;
}
