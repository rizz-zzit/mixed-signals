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
