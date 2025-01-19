#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>


int main(int argc, char*argv[]) {
    struct sockaddr_un addr;
    int client_socket = socket(AF_UNIX, SOCK_STREAM,0);
    if (client_socket == -1) {
        perror("Socket Creation Error: ");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/socket");

    // Attempts to connect
    if (connect(client_socket,(struct sockaddr *) &addr, sizeof(addr)) == -1 ) {
        perror("Connection Error");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to Server\n");

    const char *msg = "Hello, world!";
    if (send(client_socket, msg, strlen(msg),0) == -1) {
        perror("Sending Error");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    ssize_t bytes = recv(client_socket, buffer, sizeof(buffer) - 1,0);
    if (bytes > 0) {
        // null terminator to signify end of string
        buffer[bytes] = '\0';
        printf("Recieved: %s\n", buffer);
    }



    close(client_socket);



}
