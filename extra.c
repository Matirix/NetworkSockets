// /**
// * server_socket_init - Sets up logic for UNIX domain socket.
// *
// * This inclides creation, binding, and preparation for incoming client connections
// *
// * @addr: Pointer to the sockaddr_un sturcutre for severs address.
// *
// * Return: The server socket file descriptor or -1 if there's an error.
// */
// int server_socket_init(struct sockaddr_un* addr) {
//     // Creation
//     int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
//     if (server_socket == -1) {
//         perror("Socket Creation Error:");
//         return -1;
//     }

//     // Binding
//     // Setting addr parameters, IPC type and file location.
//     memset(&addr, 0, sizeof(addr)); // Clears the memory by replacing all bytes to 0
//     addr.sun_family = AF_UNIX; // Specifies that this will be used for IPC on same machine
//     strcpy(addr.sun_path, "/tmp/socket"); // Dictates where the socket will be in the file system

//     // Remove the socket file if it exists
//     if (access(S_PATH, F_OK) != -1) {
//         remove(S_PATH);
//     }

//     // Associates the server socket with the addrs created above
//     if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
//         perror("Binding Error");
//         exit(EXIT_FAILURE);
//         return -1;
//     }

//     // Begins listening mode
//     printf("Listening...\n");
//     if (listen(server_socket, BACKLOG) == -1) {
//         perror("Listen Error");
//         exit(EXIT_FAILURE);
//     }

//     printf("Server Socket is now accepting new connections\n");
//     return server_socket;
// }
