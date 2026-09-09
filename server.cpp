#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int fillAddress(struct sockaddr_in& serverAddress, char* ipaddress, int port){
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;
    if(ipaddress == NULL){
        serverAddress.sin_addr.s_addr = INADDR_ANY;
    }
    else{
        int ret = inet_pton(AF_INET, ipaddress, &serverAddress.sin_addr);
        if(ret == 1){
            std::cout << "Address set successfully" << std::endl; 
        }
        else if(ret == 0){
            std::cerr << "Invalid IPv4 Address" << std::endl;
            return 1;
        }
        else{
            std::cerr << "Address not supported by OS" << std::endl;
            return 1;
        }
    }
    return 0;
}

int main(){
    int serverFd = socket(AF_INET,SOCK_STREAM,0);
    if(serverFd < 0){
        std::cerr << "Socket Creation failed" << std::endl;
        return 1;
    }
    struct sockaddr_in serverAddress;
    if(fillAddress(serverAddress,NULL,6767) == 1) {
        return 1;
    }
    if(bind(serverFd,(struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        std::cerr << "Socket binding failed" << std::endl;
        return 1;
    }
    std::cout << "Socket bound successfully" << std::endl;
    listen(serverFd,10);
    std::cout << "Socket is listening for 10 connections" << std::endl;
    struct sockaddr_in clientAddr;
    int connectionFd = connect(serverFd,(struct sockaddr*) &clientAddr, sizeof(clientAddr));
    std::string greeting = "Yo wsg gng";
    send(connectionFd, greeting.c_str(), greeting.size(), 0);
    close(connectionFd);
    close(serverFd);
}