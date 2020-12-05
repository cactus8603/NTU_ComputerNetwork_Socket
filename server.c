#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 

// create a socket
short SocketCreate()
{
    short socketfd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketfd == -1)
    {
        perror("Server socket create failed\n");
        return -1;
    }
    else return socketfd;
}

// bind socket
int BindCreatedSocket(int socketfd)
{
    int bind_socket;
    int ClientPort = 8080;

    // address setting
    struct sockaddr_in  client = {0};
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(ClientPort);

    // bind the socket with client 
    bind_socket = bind(socketfd, (struct sockaddr *)&client, sizeof (struct sockaddr_in));
    if(bind_socket == -1)
    {
        perror("Server socket bind failed\n");
        return -1;
    }
    else return bind_socket;
}

int main(int argc, char *argv[])
{
    int socket_desc, sock, clientLen;
    struct sockaddr_in server, client;
    
    // initial sockaddr_in
    bzero(&server, sizeof server);
    bzero(&client, sizeof client);

    char client_message[200] = {0};
    char message[200] = {0};

    // create socket
    socket_desc = SocketCreate();
    
    // bind
    BindCreatedSocket(socket_desc);

    // listen with at most 3
    listen(socket_desc, 3);

    while(1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);
        
        // accept connection from client
        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);

        if (sock < 0 )
        {
            perror("accept failed");
            return 1;
        }
        printf("Connection accepted\n");

        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);

        // receive a message from the client
        if( recv(sock, client_message, 200, 0) < 0)
        {
            perror("recv failed");
            break;
        }
        
        printf("\nMessage from client : %s\n", client_message);

        strcpy(message, "\nServer received the message: ");
        strcat(message, client_message);
        strcat(message, "\n");

        // send data
        if( send(sock, message, strlen(message), 0) < 0)
        {
            perror("Send failed");
            return 1;
        }

        close(sock);
        sleep(1);
    }
    return 0;
}