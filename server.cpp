#include "helper.hpp"


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
    socklen_t clientAddrSize = sizeof(sockaddr_in);
    int connectionFd = accept(serverFd,(struct sockaddr*) &clientAddr, &clientAddrSize);
    std::string greeting = "Yo wsg gng";
    send(connectionFd, greeting.c_str(), greeting.size(), 0);
    close(connectionFd);
    close(serverFd);
}