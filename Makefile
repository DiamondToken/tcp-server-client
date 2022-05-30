CC=gcc

#SRC=src/tcpclient.c src/tcpserver.c
# SERVER=src/tcpserver.c
# CLIENT=src/tcpclient.c
objects=server client

all: server client

server: $(SERVER)
	$(CC) -o server src/tcpserver.c

client: $(CLIENT)
	$(CC) -o client src/tcpclient.c

clean:
	-rm $(objects)
