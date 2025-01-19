#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <ctype.h>

#define BACKLOG 5
#define S_PATH "/tmp/socket"



/**
* caeser_cipher_encrpyt - Encrypts by incrementing according to shift
*
* This ceaser cipher takes into account negatives, numbers exceeding 26.
* @buffer: The string to be encrypted
* @shift: Number of alphabetical shifts for each character
* Return: Encrpyted string.
*/
char* caeser_cipher_encrypt(char buffer[], int shift) {
    for (int i = 0; i < strlen(buffer); i++) {
        if (isalpha(buffer[i])) {
            if (islower(buffer[i])) {
                buffer[i] = ((buffer[i] - 'a' + shift) % 26 + 26) % 26 + 'a';
            } else if (isupper(buffer[i])) {
                buffer[i] = ((buffer[i] - 'A' + shift) % 26 + 26) % 26 + 'A';
            }
        }
        // Non-alphabetic characters remain unchanged
    }
    return buffer;
}

/**
main - Entry point for the server socket

@ argc Number of command line arguments
@ argv an Array of Strings that represent the command line arguments.
        i.e, arg[1] is used as an indicator of how many letters to shift for encryption.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USE: %s <shift> \n", argv[0]);
        return 1;
    }
    int shift = atoi(argv[1]);

    // SET UP SOCKET
    struct sockaddr_un addr;
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket Creation Error:");
        return -1;
    }

    // BINDING
    // Setting addr parameters, IPC type and file location.
    memset(&addr, 0, sizeof(addr)); // Clears the memory by replacing all bytes to 0
    addr.sun_family = AF_UNIX; // Specifies that this will be used for IPC on same machine
    strcpy(addr.sun_path, "/tmp/socket"); // Dictates where the socket will be in the file system

    // Remove the socket file if it exists
    if (access(S_PATH, F_OK) != -1) {
        remove(S_PATH);
    }

    // Associates the server socket with the addrs created above
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Binding Error");
        exit(EXIT_FAILURE);
        return -1;
    }

    // LISTENING
    printf("Listening...\n");
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }

    printf("Server Socket is now accepting new connections\n");

    // ACCEPT CONNECTION
    int c_socket = accept(server_socket, NULL, NULL);
    if (c_socket == -1) {
        perror("Accept Error");
        // continue; // Keep the server running even if one connection fails
    }

    // RECIEVE MESSAGE
    printf("Accepted a new client connection\n");
    char buffer[128];
    ssize_t bytes = recv(c_socket, buffer, sizeof(buffer) - 1,0);
    if (bytes > 0) {
        // null terminator to signify end of string
        buffer[bytes] = '\0';
        printf("Recieved: %s\n", buffer);
    }

    // SEND (RESPOND) WITH ENCRPYTED MESSAGE
    char* encrypytedMessage = caeser_cipher_encrypt(buffer, shift);
    if (send(c_socket, buffer, strlen(encrypytedMessage), 0) == -1) {
        perror("Server Response Error");
        exit(EXIT_FAILURE);
    }
    // CLEAN UP
    close(c_socket);
    close(server_socket);
    if (unlink(S_PATH) == -1) {
        perror("Unlink Err");
        exit(EXIT_FAILURE);
    }


    return 0;
}
