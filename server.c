#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>

int BACKLOG = 5;

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket Creation Error:");
        return 1;
    }

    // Setting addr parameters, IPC type and file location.
    memset(&addr, 0, sizeof(addr)); // Clears the memory by replacing all bytes to 0
    addr.sun_family = AF_UNIX; // Specifies that this will be used for IPC on same machine
    strcpy(addr.sun_path, "/tmp/socket"); // Dictates where the socket will be in the file system

    // Remove the socket file if it exists
    if (access("/tmp/socket", F_OK) != -1) {
        remove("/tmp/socket");
    }

    // Associates the server socket with the addrs created above
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Binding Error");
        exit(EXIT_FAILURE);
    }

    // Begins listening mode
    printf("Listening...\n");
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }

    printf("Server Socket is now accepting new connections\n");

    // Client sockets
    int c_socket;
    struct sockaddr_un c_addr;
    socklen_t c_addr_len = sizeof(c_addr);

    while (1) {
        // Accepting connection
        c_socket = accept(server_socket, (struct sockaddr *)&c_addr, &c_addr_len);
        if (c_socket == -1) {
            perror("Accept Error");
            continue; // Keep the server running even if one connection fails
        }

        // Recieving  code
        printf("Accepted a new client connection\n");
        char buffer[128];
        ssize_t bytes = recv(c_socket, buffer, sizeof(buffer) - 1,0);
        if (bytes > 0) {
            // null terminator to signify end of string
            buffer[bytes] = '\0';
            printf("Recieved: %s\n", buffer);
        }

        // Response code here:
        const char* msg = "Hello from Server";
        if (send(c_socket, msg, strlen(msg), 0) == -1) {
            perror("Server Response Error");
            exit(EXIT_FAILURE);
        }




        // Close client socket after communication
        close(c_socket);
    }

    // Close the server socket when done
    close(server_socket);

    return 0;
}
