#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

int main(int argc, char *argv[]) {
    // Create a local socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 1;
    }

    // Create a connection to the server
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);

    // Now we send a connect message
    if ( connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        puts("connection error");
        return 1;
    }
    puts("Connected");

    // Now we send a message to the server
    char *msg = "GET / HTTP/1.1\r\n\n\r\n";
    if ( write(socket_desc, msg, strlen(msg)) < 0 ) {
        puts("Message not sent");
        return 1;
    }
    puts("Message sent");

    char reply[2000];
    if ( read(socket_desc, reply, 2000) < 0 ) {
        puts("No Reply");
        return 1;
    }
    puts("Reply Recieved");
    puts(reply);

    close(socket_desc);

    return 0;
}
