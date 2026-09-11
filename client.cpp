#include "helper.hpp"

int main(){
    struct sockaddr_in sockaddress;
    if(fillAddress(sockaddress,"140.238.240.25",6767) == 1){
        return 1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }
    if(connect(sockfd, (struct sockaddr*) &sockaddress, sizeof(sockaddress)) < 0){
        std::cerr << "Connection connection failed" << std::endl;
        return 1;
    }
    char resfromserver[1024];
    int bytesRead = read(sockfd,resfromserver,sizeof(resfromserver) - 1);
    if (bytesRead > 0) {
        resfromserver[bytesRead] = '\0'; // Null-terminate safely
        std::cout << "Server says: " << resfromserver << std::endl;
    }
    close(sockfd);
}