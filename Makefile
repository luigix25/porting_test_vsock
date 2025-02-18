all: server client

server: server.c
	$(CC) $(CFLAGS) -o server $^

client: client.c
	$(CC) $(CFLAGS) -o client $^

clean:
	$(RM) client.o server.o client server
