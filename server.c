#include<stdio.h>
#include<string.h>

#include<arpa/inet.h>
#include<sys/socket.h>

int main(int argc, char *argv[]) {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_desc == -1 ) {
        puts("Socket creation failed");
        return 1;
    }
    puts("Socket creation succeded");

    struct sockaddr_in server;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);

    // Bind the socket to the port
    if ( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        puts("Bind failed");
    }
    puts("Bind succeded");

    // Listen for connections
    listen(socket_desc, 3);

    // Now we accept connections
    puts("Waiting for connections");
    struct sockaddr_in client;
    int c = sizeof(struct sockaddr_in);
    int new_socket;
    char *message;
    while ( (new_socket = accept( socket_desc, (struct sockaddr *) & client, (socklen_t *) &c)) ) {
        puts("Connection Accepted");
        message = "Hello World";
        write(new_socket, message, strlen(message));
    }

    if ( new_socket < 0 ) {
        perror("accept failed");
        return 1;
    }


    return 0;
}


