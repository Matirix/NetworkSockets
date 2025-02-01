# Network Sockets
## Purpose:
This client/server application demonstrates Network communication using Network Sockets. The server component accepts a parameter that acts as a key to encrypting content sent from the client. Upon encryption, the server sends the client the encrypted content back.

## Installing:
### Obtaining
  ```git clone https://github.com/Matirix/NetworkSockets.git```
### Building
  ```./build.sh```
### Running
  ```./server <ip_addr> <port>```
  ```./client <ip_addr> <port> <filename> <key>```


## Examples

./server 127.0.0.1 8080
./client 127.0.0.1 8080 wraparound.txt abc
