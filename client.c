#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>

#define S_PATH "/tmp/socket"


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

    // Allocate a buffer to store the files
    char* buffer = (char *) malloc(file_size + 1);
    if (buffer) {
        fread(buffer, 1, file_size, file);
    }
    fclose(file);
    return buffer;
}

/**
main - Entry point for the server socket

@ argc Number of command line arguments
@ argv an Array of Strings that represent the command line arguments.
        i.e, arg[1] is used for a file name.
*/
int main(int argc, char*argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USE: %s <filename> \n", argv[0]);
        return -1;
    }
    const char *msg = read_file(argv[1]);
    if (strlen(msg) == 0) {
        printf("File is empty!");
        return -1;
    }

    struct sockaddr_un addr;
    // CREATE SOCKET UNIX DOMAIN SOCKET
    int client_socket = socket(AF_UNIX, SOCK_STREAM,0);
    if (client_socket == -1) {
        perror("Socket Creation Error: ");
        return -1;
    }

    // SET UP ADDRESS
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, S_PATH);

    // CONNECT TO SOCKET
    if (connect(client_socket,(struct sockaddr *) &addr, sizeof(addr)) == -1 ) {
        perror("Connection Error");
        close(client_socket);
        return -1;
    }

    printf("Connected to Server\n Sending message...");


    if (send(client_socket, msg, strlen(msg),0) == -1) {
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
        printf("Recieved Encrypted Message: %s\n", buffer);
    }

    close(client_socket);
    return 0;

}
