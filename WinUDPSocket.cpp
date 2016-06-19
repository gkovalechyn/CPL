/* 
 * File:   WinUDPSocket.cpp
 * Author: gkovalechyn
 * 
 * Created on June 18, 2016, 8:04 PM
 */

#include <iostream>

#include "WinUDPSocket.h"

WinUDPSocket::WinUDPSocket() {
    this->sockaddrInLen = sizeof(this->other);
}

WinUDPSocket::WinUDPSocket(const WinUDPSocket& orig) {
}

WinUDPSocket::~WinUDPSocket() {
    WSACleanup();
    if (sock != INVALID_SOCKET){
        closesocket(this->sock);
        this->sock = INVALID_SOCKET;
    }
}

bool WinUDPSocket::connectTo(const char* addr, int port) {
    WSADATA wsaData;
    struct hostent* host;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        *this->logStream << "WSA startup failed, error: " << WSAGetLastError() << std::endl;
        return false;
    }
    
    host = gethostbyname(addr);
    
    if (host == NULL){
        *this->logStream << "Could not find host" << std::endl;
        WSACleanup();
        return false;
    }
    
    if ((this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET){
        *this->logStream << "Could not create socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    
    other.sin_family = AF_INET;
    other.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    other.sin_port = htons(port);
    
    if (connect(this->sock, (sockaddr*) &other, sizeof(other)) == SOCKET_ERROR){
        *this->logStream << "Could not create socket. Error: " << WSAGetLastError() << std::endl;
        closesocket(this->sock);
        this->sock = INVALID_SOCKET;
        WSACleanup();
        return false;
    }
    
    *this->logStream << "Connected." << std::endl;
    return true;
}

int WinUDPSocket::read(char* buff, int len) {
    return recvfrom(this->sock, buff, len, 0, (struct sockaddr*) &this->other, &this->sockaddrInLen);
}

void WinUDPSocket::write(char* buff, int len) {
    sendto(this->sock, buff, len, 0, (struct sockaddr*) &this->other, this->sockaddrInLen);
}

void WinUDPSocket::write(char* buff, int offset, int len) {
    this->write(buff + offset, len);
}

void WinUDPSocket::closeConnection() {
    if (this->sock != INVALID_SOCKET){
        closesocket(this->sock);
        WSACleanup();
        this->sock = INVALID_SOCKET;
    }
}

void WinUDPSocket::setLog(std::ofstream* log) {
    this->logStream = log;
}







