#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


#define PORT 3333
#define BACK_LOG 10
#define BUFFER_SIZE 4096


void sig_handler(int signum){
    if (signum == SIGTERM){
        printf("Need to exit safely\n");
        /* signal(SIGTERM, sig_handler); */
    }else if (signum == SIGHUP){
        printf("Connection lost");
    }
}

int main(int argc, char ** argv){
    if (argc != 2){
        printf("Usage: ./server <log_file>\n");
        exit(0);
    }

    int listenfd, connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    pid_t childpid;
    socklen_t addrlen;
    char buff[BUFFER_SIZE];

    FILE *fp;
    const char *out_file = "output.txt";
    fp = fopen(out_file, "a");
    if (fp == NULL){
        printf("Could not open file %s:  %s\n", out_file, strerror(errno));
    }

    errno = 0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1){
        perror("Socket creation failed");
        exit(0);
    }

    int option;
    option = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, option, &option, sizeof(option));
    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1){
        perror("Bind error!");
        exit(1);
    }

    if(listen(listenfd, BACK_LOG) == -1){
        perror("Listend error");
        exit(1);
    }

    printf("Waiting for client's request.....\n");
    while(1){
        signal(SIGTERM, sig_handler);
        signal(SIGHUP, sig_handler);

        int n;
        addrlen = sizeof(client);
        connectfd = accept(listenfd, (struct sockaddr*)&client, &addrlen);
        if(connectfd == -1){
            perror("Accept error");
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
                /* fputs(buff, fp); */
                assert(fp != NULL);
                fprintf(fp, "%s", buff);
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
