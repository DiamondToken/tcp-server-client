CC=gcc

SRC=src/tcpclient.c src/tcpserver.c
SERVER=src/tcpserver.c
CLIENT=src/tcpclient.c

objects=${SRC:.c=.o}


all: server client

server: ${SERVER:.c=.o}
	$(CC) -o server $<

client: ${CLIENT:.c=.o}
	$(CC) -o client $<

server.o: $(SERVER)
	$(CC) -c $(CLIENT)

client.o: $(CLIENT)
	$(CC) -c $(CLIENT)

clean:
	-rm server client $(objects)
