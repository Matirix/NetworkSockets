#include <ctype.h>
#include <malloc/_malloc_type.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define PORT 8080
// TODO Implement Socket, Port, Filename

/**
read_file - Reads the file and returns the contents

@param file_name The file name
Returns the contents
*/
char* read_file(char* file_name) {
    FILE *file = fopen(file_name,"r");
    if (file == NULL) {
        perror("File Error");
        exit(EXIT_FAILURE);
    }
    // GET SIZE
    fseek(file, 0, SEEK_END); // Go to end
    long file_size = ftell(file); // Get to the position to figure out size
    fseek(file, 0, SEEK_SET); // Go back to the beginning

    // Allocate a buffer to store the file content
    char* buffer = (char *) malloc(file_size + 1);
    if (buffer) {
        fread(buffer, 1, file_size, file);
    }
    fclose(file);

    // Remove the new line and replace it with a delimiter instead which used for the vigenere function.
    if (buffer[file_size - 1] == '\n') {
        buffer[file_size - 1] = '&';
    }

    if (strlen(buffer) == 0) {
        printf("File is empty!");
        exit(-1);
    }
    return buffer;
}

/**
Checks for numbers in the key. If there are numbers this is an invalid key.
@param key - char* used to verify if there are no numbers in the key.
*/
int isValidString(char* key ) {
    for (int i=0; i < strlen(key); i++) {
        if (!isalpha(key[i])) {
            return 0;
        }
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
Combines two the message and the key into one server-readable format.

@param msg - first portion of the combined string
@param key - second portion of the combined string
Returns one string
*/
char* concat_strings(const char* msg, const char* key) {
    size_t len_msg = strlen(msg);
    size_t len_key = strlen(key);

    // Allows for space with +1 to signify end of string
    char* result = malloc(len_msg + len_key + 1);
    if (result == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // Copying and extending.
    strcpy(result, msg);
    strcat(result, key);

    return result;
}

/**
main - Entry point for the server socket

@ argc Number of command line arguments
@ argv an Array of Strings that represent the command line arguments.
        i.e, arg[1] is used for a file name.
*/
int main(int argc, char*argv[]) {
    if (argc < 5) {
        printf("Too few arguments");
        fprintf(stderr, "USE: %s <ip_addr> <port> <filename> <key> \n", argv[0]);
        return -1;
    }
    if (argc > 5) {
        printf("Too many arguments");
        fprintf(stderr, "USE: %s <ip_addr> <port> <filename> <key> \n", argv[0]);
        return -1;
    }
    char *ip_addr;
    char *port;
    char *msg = read_file(argv[3]);
    char *key = argv[4];
    char *combined;
    // Checking if input is valid.
    if (is_valid_address_port(argv[1], 0)|| is_valid_address_port(argv[2], 1)) {
        ip_addr = argv[1];
        port = argv[2];
    }
    if (isValidString(key) == 0) {
        return -1;
    }

    combined = concat_strings(msg, key);



    struct sockaddr_in serv_addr;
    // CREATE SOCKET UNIX DOMAIN SOCKET
    int client_socket = socket(AF_INET, SOCK_STREAM,0);
    if (client_socket == -1) {
        perror("Socket Creation Error: ");
        return -1;
    }

    // SET UP ADDRESS
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converts string representation to binary represenation and stores inside serv_addr
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }


    // CONNECT TO SOCKET
    if (connect(client_socket,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1 ) {
        perror("Connection Error");
        close(client_socket);
        return -1;
    }

    printf("Connected to Server\n Sending message...");


    if (send(client_socket, combined, strlen(combined),0) == -1) {
        perror("Sending Error");
        close(client_socket);
        return -1;
    }
    printf("Connected to Server\n ");

    char buffer[128];
    ssize_t bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        // null terminator to signify end of string
        buffer[bytes] = '\0';
        printf("MESSAGE FROM SERVER: %s\n", buffer);
    }

    close(client_socket);
    return 0;

}
