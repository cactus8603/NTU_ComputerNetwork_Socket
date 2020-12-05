all:
	gcc -o server server.c
	gcc -o client client.c
clean:
	rm server
	rm client
