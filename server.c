#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <ctype.h>

#define BACKLOG 5
#define PORT 8080

// TODO PORT, IP_ADDRESS


/**
* vigenere cipher- Encrypts by incrementing according to shift
*
* This cipher takes into account negatives, numbers exceeding 26.
* @buffer   - The string to be encrypted
* @key      - String used to encrypt the buffer
* Return    - Encrpyted string.
*/
char* vigenere_cipher(char buffer[], char key[]) {
    int j = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (isalpha(buffer[i])) {
            char shift = key[j] - 'a';
            if (islower(buffer[i])) {
                buffer[i] = ((buffer[i] - 'a' + shift) % 26 + 26) % 26 + 'a';
            } else if (isupper(buffer[i])) {
                buffer[i] = ((buffer[i] - 'A' + shift) % 26 + 26) % 26 + 'A';
            }
            j = (j+1) % strlen(key); // Make sure it loops around
        }
        // Non-alphabetic characters remain unchanged
    }
    return buffer;
}

/**
    Checks to see if the msg contains only 1 delimiter. If not, then bad.

    @param msg - msg to check.
*/
int is_valid_input(char* msg) {
    printf("%s\n From Valid input", msg);
    int delims = 0;
    for (int i=0; i<strlen(msg); i++ ) {
        if (msg[i] == '&') {
            delims++;
        }
        if (delims > 1) {
            return 0;
        }
    }
    return 1;
}
/*
    Used for sending failure/success messages

    @param c_socket     - socket that recieves
    @param msg          - msg to be sent
    @param msg_len      - msg length
*/
int send_message(int c_socket,char* msg, size_t msg_len){
    if (send(c_socket, msg, msg_len, 0) == -1) {
        perror("Server Response Error");
        return -1;
    }
    return 1;
}

/**
Validates Address and Port Numberes

@param addr         - IP address to be parsed
@param is_port      - PORT Number to be parsed
*/
int is_valid_address_port(char* addr, int is_port) {
    int dot_count = 0;
    for (int i = 0; i < strlen(addr); i++) {
        if (addr[i] == '.') {
            dot_count++;
        } else if (!isdigit(addr[i])) {
            printf("Invalid digit");
            return 0;
        }
    }
    if (is_port) {
        return 1;
    } else if (dot_count != 3) {
        printf("Not a valid Ip address");
        return 1;
    }
    return 1;
}


/**
main - Entry point for the server socket

@ argc Number of command line arguments
@ argv an Array of Strings that represent the command line arguments.
        i.e, arg[1] is used to indicate the IP address
        i.e, arg2[2] is used to indicate a port number to bind the server
*/
int main(int argc, char *argv[]) {
    char* ip_addr;
    char* port;
    if (argc > 3) {
        printf("Too many arguments");
        fprintf(stderr, "USE: %s <addr> <port> \n", argv[0]);
        return -1;
    }
    if (argc < 3) {
        printf("Too few arguments");
        fprintf(stderr, "USE: %s <addr> <port> \n", argv[0]);
        return -1;
    }

    // Checking if input is valid.
    if (is_valid_address_port(argv[1], 0)|| is_valid_address_port(argv[2], 1)) {
        ip_addr = argv[1];
        port = argv[2];
    }

    // SET UP SOCKET NETWORK SOCKET = AF_INET
    struct sockaddr_in addr;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket Creation Error:");
        return -1;
    }

    // SET UP PORT
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }


    // BINDING
    // Setting addr parameters, IPC type and file location.
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_addr);
    addr.sin_port = htons(strtol(port, NULL, 10));



    // Associates the server socket with the addrs created above
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Binding Error");
        return -1;
    }


    // LISTENING
    printf("Listening...\n");
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Listen Error");
        return -1;
    }

    printf("Server Socket is now accepting new connections\n");

    // ACCEPT CONNECTION
    int c_socket = accept(server_socket, NULL, NULL);
    if (c_socket == -1) {
        perror("Accept Error");
        return -1;
    }

    // RECIEVE MESSAGE
    printf("Accepted a new client connection\n");
    char buffer[128];
    ssize_t bytes = recv(c_socket, buffer, sizeof(buffer) - 1,0);
    if (bytes > 0) {
        // null terminator to signify end of string
        buffer[bytes] = '\0';
        if (bytes >= 128) {
            char* msg = "Content size too large - Server will only return up to 128 bytes.";
            printf("%s", msg);
            send_message(c_socket, msg, strlen(msg));
        }
        printf("Recieved: %s\n", buffer);

    }
    if (is_valid_input(buffer) == 0) {
        char* msg = "Invalid input, there must ONLY be one '&' character";
        printf("%s", msg);
        send_message(c_socket, msg, strlen(msg));
        close(server_socket);
        exit(EXIT_FAILURE);
    };

    char *msg = strtok(buffer, "&");
    char *key = strtok(NULL, "&");

    printf("Encrypting...");
    // SEND (RESPOND) WITH ENCRPYTED MESSAGE
    char* encrypytedMessage = vigenere_cipher(msg, key);
    if (send_message(c_socket, encrypytedMessage, strlen(encrypytedMessage)) == -1) {
        close(server_socket);
        perror("Server Response Error");
        return -1;
    }
    printf("Sending back encrypted content: %s",buffer);

    // CLEAN UP
    close(c_socket);
    close(server_socket);


    return 0;
}
