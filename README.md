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

./server 1
./client hello.txt
