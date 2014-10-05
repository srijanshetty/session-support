#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Related to sockets
#include<arpa/inet.h>
#include<sys/socket.h>

// Related to threading
#include<pthread.h>

// Constants
#define PORT 8000

void *connection_handler(void *);

int main(int argc, char *argv[]) {

    // Create a socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_desc == -1 ) {
        perror("[FAIL] Socket creation failed");
        return 1;
    }

    // Bind the socket to the port
    struct sockaddr_in server;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if ( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0 ) {
        perror("[FAIL] Bind failed");
    }

    // Listen for connections
    listen(socket_desc, 3);
    printf("[OKAY] Listening for connections on port %d\n", PORT);

    // Now we accept connections
    struct sockaddr_in client;
    char *message;
    int new_socket, *new_sock, c;

    puts("[OKAY] Waiting for connections");
    c = sizeof(struct sockaddr_in);
    while ( (new_socket = accept( socket_desc, (struct sockaddr *) & client, &c)) ) {
        puts("\n[OKAY] Connection Accepted");
        message = "HELO\n";
        write(new_socket, message, strlen(message));

        // Handler
        pthread_t handler;
        new_sock = malloc(sizeof(int));
        *new_sock = new_socket;

        if ( pthread_create( &handler, NULL, connection_handler, (void *)new_sock )  < 0 ) {
            perror("[FAIL] could not create thread");
            return 1;
        }

        // Join the thread
        // pthread_join( handler, NULL)
        puts("[OKAY] Handler Assigned");
    }

    if ( new_socket < 0 ) {
        perror("[FAIL] accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler( void *sock ) {
    // Copy the socket
    int socket_desc = *(int *)sock;
    char *message;

    // Send message to the client
    message = "FIN\n";

    write(socket_desc, message, strlen(message));

    // Free the space which was allocated to the socket
    free(sock);

    puts("[OKAY] Closing connection");

    return 0;
}

