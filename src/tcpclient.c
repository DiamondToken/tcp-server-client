#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define PORT 3333
#define MAXLINE 4096


int main(int argc,char ** argv){
    int sockfd;
    char buff[MAXLINE];

    struct hostent *he;
    struct sockaddr_in server;

    if(argc != 2){
        printf("Usage: ./client <Address>\n");
        exit(1);
    }

    he = gethostbyname(argv[1]);
    if(he == NULL){
        fprintf(stderr, "Gethostbyname error: %s\n", hstrerror(h_errno));
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        fprintf(stderr, "Socket error %s\n", strerror(errno));
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr*)he->h_addr);

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        fprintf(stderr, "Connection error %s\n", strerror(errno));
        exit(1);
    }

    fgets(buff, MAXLINE, stdin);

    if (write(sockfd, buff, strlen(buff)) == -1){
        fprintf(stderr, "Send message error: %s\n", strerror(errno));
        exit(1);
    }

    close(sockfd);

    return 0;
}
