#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

// create a Socket 
short SocketCreate()
{
    short socketfd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        perror("Create client socket failed\n");
    }
    return socketfd;
}


int SocketConnect(int socketfd)
{
    int bind_socket;
    int ServerPort = 8080;

    // address setting
    struct sockaddr_in server= {0};
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    server.sin_family = AF_INET;
    server.sin_port = htons(ServerPort);

    // bind socket
    bind_socket = connect(socketfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if(bind_socket == -1)
    {
        perror("Client socket bind failed\n");
        return -1;
    }
    else return bind_socket;
}


int SocketSend(int socketfd, char* message, short len)
{
    // set the time out of 15 secs 
    struct timeval limit;
    limit.tv_sec = 15;  
    limit.tv_usec = 0;
    
    // set time out
    if(setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&limit, sizeof(limit)) < 0)
    {
        perror("Time out\n");
        return -1;
    }

    // send message to server
    if(send(socketfd, message, len, 0) < 0)
    {
        perror("Client send socket failed\n");
        return -1;
    }
    else return 1;
}

// receive the data from server
int SocketReceive(int socketfd, char* message, short len)
{
    // set time out of 15 secs
    int recv_socket;
    struct timeval limit;
    limit.tv_sec = 15;  
    limit.tv_usec = 0;

    // set time out
    if(setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO,(char *)&limit, sizeof(limit)) < 0)
    {
        perror("Time out\n");
        return -1;
    }

    // receive message form server
    recv_socket = recv(socketfd, message, len, 0);
    if(recv_socket < 0)
    {
        perror("Client received socket failed\n");
    }
    return recv_socket;
}

int main(int argc, char *argv[])
{
    int socketfd, read_size;
    struct sockaddr_in server;
    
    char SendToServer[200] = {0};
    char server_reply[200] = {0};

    // initial sockaddr_in
    bzero(&server, sizeof server);

    // create socket
    socketfd = SocketCreate();

    // connect to server
    if (SocketConnect(socketfd) < 0)
    {
        perror("connect failed.\n");
        return 1;
    }

    printf("Sucessfully conected with server\n");
    printf("Enter the Message: ");

    // input message
    fgets(SendToServer, sizeof SendToServer, stdin);

    // send data to server
    if(SocketSend(socketfd, SendToServer, strlen(SendToServer)) < 0 )
    {
        perror("Send to server failed\n");
    }

    // received the message from the server
    read_size = SocketReceive(socketfd, server_reply, 200);
    printf("%s", server_reply);
    
    close(socketfd);
    return 0;
}