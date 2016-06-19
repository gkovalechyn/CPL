/* 
 * File:   Main.cpp
 * Author: gkovalechyn
 *
 * Created on June 19, 2016, 2:28 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "CPL.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    std::ofstream hashFile("Hashes.txt");
    char buff[10];
    
    //CPL::getInstance()->connect();
    //CPL::getInstance()->doHandshake();
    
    for(int i = 1; i < argc; i++){
        std::string cppString(argv[i]);
        std::ifstream file(cppString, std::ios::binary | std::ios::ate);
        
        if (!file.is_open()){
            std::cout << "Could not open file: " << argv[i] << std::endl;
        }else{
            SHA256 sha256 = SHA256();
            unsigned char hash[SHA256::DIGEST_SIZE];
            std::streamsize size = file.tellg();
            unsigned char* buffer;
            
            memset(hash, 0, SHA256::DIGEST_SIZE);
            
            std::cout << "Generating hash for: " << argv[i] << ", File size: " << size << std::endl;
            hashFile << argv[i] << ": ";
            file.seekg(0, std::ios::beg);

            buffer = new unsigned char[size];
            memset(buffer, 0 , size);
            
            file.read((char*) buffer, size);
            
            sha256.update(buffer, size);
            sha256.final(hash);
            
            delete[] buffer;
            
            for(int i = 0; i < SHA256::DIGEST_SIZE; i++){
                snprintf(buff, 10, "%02X", hash[i]);
                hashFile << "(byte) 0x" << buff << ", ";
            }
            
            hashFile << std::endl;
        }
    }
    return 0;
}

