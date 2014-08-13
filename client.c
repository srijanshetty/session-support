#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

// Constants
#define PORT 8000
#define SERVER "127.0.0.1"

int main(int argc, char *argv[]) {
    // Create a local socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 1;
    }

    // Create a connection to the server
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Now we send a connect message
    if ( connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        perror("[FAIL] Connection Error");
        return 1;
    }
    printf("[OKAY] Connected to %s at %d", SERVER, PORT);

    // Now we send a message to the server
    char *msg = "HELO\n";
    if ( write(socket_desc, msg, strlen(msg)) < 0 ) {
        perror("[FAIL] Message delivery failed");
        return 1;
    }
    puts("[OKAY] HELO");

    char reply[2000];
    if ( read(socket_desc, reply, 2000) < 0 ) {
        puts("[FAIL] No Reply");
        return 1;
    }
    puts("[OKAY] Reply Recieved");
    puts(reply);

    close(socket_desc);

    return 0;
}
