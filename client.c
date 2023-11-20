#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>

void error(char* msg){
    perror(msg);
    exit(1);
}

int main(int argc,char* argv[]){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0) error("SOcket did not open at client-side");

    struct sockaddr_in server_addr;
    struct hostent* server;

    bzero((char*)&server_addr,sizeof(server_addr));

    int portno = atoi(argv[1]);
    server = gethostbyname(argv[2]);

    //assigning values
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portno);
    bcopy((char*) server->h_addr,(char*) &server_addr.sin_addr.s_addr,server->h_length);

    if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0) error("Error connecting at client-side");

    char buffer[255];
    int n;
    while(1){
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,sizeof(buffer));

        if(n<0) error("Error writing msg to server-side");

        bzero(buffer,255);
        n = read(sockfd,buffer,255);

        if(n<0) error("Error reading msg at client-side...");

        printf("Server Msg is: %s\n",buffer);

        int i = strncmp("exit",buffer,4);
        if(i==0){
            printf("Exit message was triggered...");
            break;
        }
    }

    close(sockfd);
    return 0;
}
