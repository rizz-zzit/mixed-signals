#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int fillAddress(struct sockaddr_in& address, const char* ipaddress, int port);
uint32_t sizeOfIncomingMessage(int conncetionFd);
bool recv_all(int connectionFd, char* buffer, uint32_t expectedSize);
bool send_all(int connectionFd, char* buffer, uint32_t expectedSize);
void sendToEveryoneElse(std::string& message, int hostFd);
void closeGracefully(std::unique_lock& lock, int& socket);