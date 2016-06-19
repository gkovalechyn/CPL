/* 
 * File:   CPL.h
 * Author: gkovalechyn
 *
 * Created on June 18, 2016, 7:50 PM
 */

#ifndef CPL_H
#define	CPL_H

#include <cstdlib>
#include <iomanip>
#include <fstream>

#include "WinUDPSocket.h"
#include "SHA256.h"
#include "aes.h"
class CPL {
    public:
        virtual ~CPL();
        
        static CPL* getInstance();
        
        bool connect();
        WinUDPSocket* getSocket();
        
        bool doHandshake();
        
        void getDecryptionKey();
        void initializeDecryptor();
        
        bool decrypt(unsigned char* src, int length, unsigned char* dst);
        
        std::ofstream* getOutputStream();
    private:
        CPL();
        
        bool getHashOfFile(std::string path, unsigned char* output);
        static void initialize();
        static CPL* instance;
        
        WinUDPSocket* socket = NULL;
        
        std::ofstream* logFile;
        
        mbedtls_aes_context context;
        unsigned char key[16];
        unsigned char iv[16];
};

#endif	/* CPL_H */

