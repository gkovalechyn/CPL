/* 
 * File:   WinUDPSocket.h
 * Author: gkovalechyn
 *
 * Created on June 18, 2016, 8:04 PM
 */

#ifndef WINUDPSOCKET_H
#define	WINUDPSOCKET_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

class WinUDPSocket {
public:
    WinUDPSocket();
    WinUDPSocket(const WinUDPSocket& orig);
    virtual ~WinUDPSocket();
    
    bool connectTo(const char* addr, int port);
    
    void write(char* buff, int len);
    void write(char* buff, int offset, int len);
    
    void closeConnection();
    
    int read(char* buff, int len);
    void setLog(std::ofstream* log);
    
private:
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in other;
    int sockaddrInLen;
    char* buffer;
    std::ofstream* logStream;
};

#endif	/* WINUDPSOCKET_H */

