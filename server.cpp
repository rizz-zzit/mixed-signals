#include "helper.hpp"
#include <thread>
#include <map>
#include <mutex>
std::unordered_set<int> sockets;
std::shared_mutex mtx;
uint32_t sizeOfIncomingMessage(int conncetionFd){
    uint32_t temp;
    recv(conncetionFd,reinterpret_cast<char*> (&temp), sizeof(uint32_t), 0);
    return temp;
}
bool recv_all(int connectionFd, char* buffer, uint32_t expectedSize){
    uint32_t receivedBytes = 0;
    while(receivedBytes < expectedSize){
        uint32_t bytesRemaining = expectedSize - receivedBytes;
        uint32_t currentBytes = recv(connectionFd, buffer + receivedBytes, bytesRemaining, 0);
        if(currentBytes < 0){
            std::cerr << "Failed to read client data " << std::endl;
            return false;
        } else{
            receivedBytes += currentBytes;
        }
    }
    return true;
}
bool send_all(int connectionFd, char* buffer, uint32_t expectedSize){
    uint32_t sentBytes = 0;
    while(sentBytes < expectedSize){
        uint32_t remainingBytes = expectedSize - sentBytes;
        uint32_t currrentBytes = send(socket,buffer + sentBytes,remainingBytes,0);
        if(currrentBytes < 0){
            std::cerr << "Failed to send message" << std::endl;
            return false;
        } else {
            sentBytes += currrentBytes;
        }
    }
    return true;
}
void sendToEveryoneElse(std::string& message, int hostFd){
    std::shared_lock lock(mtx);
    for(int socket : sockets){
        if(socket != hostFd){
            send_all(socket,(char*) (&message[0]),message.size())
        }
    }

}
void closeGracefully(std::unique_lock& lock, int& socket){
    close(connectionFd);
    lock.lock();
    sockets.erase(connectionFd);
    lock.unlock();
}
void handleAcceptedConnection(int connectionFd, struct sockaddr_in clientAddress){
    std::unique_lock lock(mtx);
    sockets.insert(connectionFd);
    lock.unlock();
    std::string greeting = "Yo wsg gng, name pls: ";
    send_all(connectionFd,(char*) (&greeting[0]),greeting.size());
    int usernameSize = sizeOfIncomingMessage(connectionFd);
    std::string username(usernameSize,'\0');
    if(recv_all(connectionFd, (char*) (&username[0]), usernameSize)){
        std::string initialMessage = "User: " + username + "has joined";
        std::cout << initialMessage << std::endl;
        sendToEveryoneElse(initialMessage,connectionFd)
    } else {
        std::cerr << "Could not read username" << std::endl;
        closeGracefully(lock,connectionFd);
        return;
    }
    while(true){
        int messageSize = sizeOfIncomingMessage(connectionFd);
        std::string message(messageSize,'\0');
        if(recv_all(connectionFd, (char*) (&message[0]), messageSize)){
            if(message != "exit"){
                std::string sendMessage = "[" + username + "] :" + message;
                sendToEveryoneElse(sendMessage,connectionFd);
            } else {
                std::string sendMessage = "[" + username + "] :" + "has exited";
                sendToEveryoneElse(sendMessage,connectionFd);
                closeGracefully(lock,connectionFd);
                return;
            }
        } else {
            std::cerr << "Could not read message" << std::endl;
            closeGracefully(lock,connectionFd);
            return;
        }
    }
    closeGracefully(lock,connectionFd);
}
char loopback[] = "127.0.0.1";

int main(){
    int serverFd = socket(AF_INET,SOCK_STREAM,0);
    if(serverFd < 0){
        std::cerr << "Socket Creation failed" << std::endl;
        return 1;
    }
    struct sockaddr_in serverAddress;
    if(fillAddress(serverAddress,loopback,6767) == 1) {
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
    //listening for connections continuously
    while(true){
        int connectionFd = accept(serverFd,(struct sockaddr*) &clientAddr, &clientAddrSize);
        if(connectionFd > 0){
            std::thread clientThread(handleAcceptedConnection,connectionFd,clientAddr);
            clientThread.detach();
            clients
        }
    }
    
    close(serverFd);
}