#include "helper.hpp"

int fillAddress(struct sockaddr_in& address, const char* ipaddress, int port){
    address.sin_port = htons(port);
    address.sin_family = AF_INET;
    if(ipaddress == NULL){
        address.sin_addr.s_addr = INADDR_ANY;
    }
    else{
        int ret = inet_pton(AF_INET, ipaddress, &address.sin_addr);
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
