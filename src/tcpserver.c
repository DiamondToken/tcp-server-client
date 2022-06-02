#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 3333
#define BACK_LOG 10
#define BUFFER_SIZE 4096


int main(int argc, char ** argv){
    if (argc != 1){
        printf("Usage: ./server\n");
        exit(0);
    }

    int listenfd, connectfd;
    struct sockaddr_in server, client;

    pid_t childpid;
    socklen_t addrlen;
    char buff[BUFFER_SIZE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1){
        fprintf(stderr, "Socket creation failed %s\n", strerror(errno));
        exit(1);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(listenfd, (struct sockaddr *)&server, sizeof(server))) != 0){
        fprintf(stderr, "Bind error! %s\n", strerror(errno));
        exit(1);
    }

    if (listen(listenfd, BACK_LOG) != 0){
        fprintf(stderr, "Listen error: %s\n", strerror(errno));
        exit(1);
    }

    printf("Waiting for client's request.....\n");
    while(1){
        int n; // read
        addrlen = sizeof(client);
        connectfd = accept(listenfd, (struct sockaddr*)&client, &addrlen);

        if(connectfd == -1){
            fprintf(stderr, "Accept error %s\n", strerror(errno));
            exit(0);
        }else{
            printf("Client connected\n");
        }
        if ((childpid = fork()) == 0){
            close(listenfd);
            printf("Client from %s\n", inet_ntoa(client.sin_addr));
            //memset(buff,'\0',sizeof(buff));
            printf("Ready to read\n");

            while((n = read(connectfd, buff, BUFFER_SIZE)) > 0){
                buff[n] = '\0';
                printf("Requested message from client: %s\n", buff);
            }

            printf("End read\n");
            exit(0);
        }
        else if(childpid < 0)
            printf("Fork error: %s\n", strerror(errno));

        close(connectfd);
	}

    return 0;
}
