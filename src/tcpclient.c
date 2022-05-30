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
    int sockfd, num;
    char buff[MAXLINE];
    const char *file_to_send = argv[2];

    struct hostent *he;
    struct sockaddr_in server;

    if(argc != 3){
        printf("Usage: ./<IP Address> <File to send>\n");
        exit(1);
    }

    he = gethostbyname(argv[1]);
    if(he == NULL){
        printf("gethostbyname error\n");
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        printf("socket error\n");
        exit(1);
    }

    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr*)he->h_addr);

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))== -1){
        printf("connect error\n");
        exit(1);

    }

    FILE *fp;
    fp = fopen(file_to_send, "r");

    printf("Sending file <%s> to server at <%s>: \n", file_to_send, argv[1]);
    fgets(buff, MAXLINE, fp);

    if (write(sockfd, buff, strlen(buff)) == -1){
        printf("send msg error: %s \n",strerror(errno));
        exit(1);
    }

    close(sockfd);

    return 0;
}
