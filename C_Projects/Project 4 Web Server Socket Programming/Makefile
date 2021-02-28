CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread -pthread

web_server: server.c util.c
	${CC} -Wall -o web_server server.c util.c ${LDFLAGS}

clean:
	rm web_server
#10 10 1 100 5
#./web_server 8500 "/home/runner/G28P4" 1 1 1 10 1
#wget http://127.0.0.1:8500/image/jpg/29.jpg