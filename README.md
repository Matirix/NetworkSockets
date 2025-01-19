# DomainSocketsUnix
## Purpose:
This client/server application demonstrates inter-process communication using UNIX Domain Sockets. The server component accepts a parameter that acts as a key to encrypting content sent from the client. Upon encryption, the server sends the client the encrypted content back.

## Installing:
### Obtaining
  ```git clone https://github.com/Matirix/DomainSocketsUnix.git```
### Building
  ```gcc -o server server.c```
  ```gcc -o server client.c```
### Running
  ```./server <shift>```
  ```./client <filename>```


## Examples

./server 1		      	// Shifts by 1;
./client hello.txt		// name of the file



./server -21			   // Shifts by -21;
./client hello.txt		// name of the file



./server 100	     		// Shifts by 100;
./client hello.txt		// name of the file


./server 1		      	// Shifts by 1;
./client README.md		// Different File Type
