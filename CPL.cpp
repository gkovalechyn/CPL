/* 
 * File:   CPL.cpp
 * Author: gkovalechyn
 * 
 * Created on June 18, 2016, 7:50 PM
 */
#include "CPL.h"

CPL* CPL::instance;

CPL* CPL::getInstance() {
    if (CPL::instance == NULL){
        CPL::initialize();
    }
    
    return CPL::instance;
}

void CPL::initialize() {
    if (CPL::instance == NULL){
        CPL::instance = new CPL();
    }
}

CPL::~CPL() {
}

CPL::CPL() {
    this->logFile = new std::ofstream("CPL.log", std::ios::out);
    this->socket = new WinUDPSocket();
    this->socket->setLog(this->logFile);
    //memset(this->key, 0, 32);
    //memset(this->iv, 0, 32);
}


bool CPL::connect() {
    return this->socket->connectTo("gkovalechyn.net", 6969);
}

WinUDPSocket* CPL::getSocket() {
    return this->socket;
}

bool CPL::getHashOfFile(std::string path, unsigned char* output){
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    char buff[10];
    
    if (file.is_open()){
        SHA256 sha256 = SHA256();
        std::streamsize size = file.tellg();
        unsigned char* buffer;
        
        file.seekg(0, std::ios::beg);
        
        buffer = new unsigned char[size];
        file.read((char*) buffer, size);
        
        sha256.update(buffer, size);
        sha256.final(output);
        
        delete[] buffer;
        
        *this->logFile << path << ": ";
        
        for(int i = 0; i < SHA256::DIGEST_SIZE; i++){
            snprintf(buff, 10, "%02X", output[i]);
            *this->logFile << "(byte) 0x" << buff << ", ";
        }
        *this->logFile << std::endl;
        return true;
    }else{
        *this->logFile << "File not open: " << path << std::endl;
        return false;
    }
}

bool CPL::doHandshake() {
    char buff[256];
    unsigned char mySHA256[SHA256::DIGEST_SIZE];
    unsigned char pluginLoaderSHA256[SHA256::DIGEST_SIZE];
    unsigned char pluginSHA256[SHA256::DIGEST_SIZE];
    int read = 0;
    
    *this->logFile << "sizeof unsigned char: " << sizeof(unsigned char) << std::endl;
    *this->logFile << "Digest size: " << SHA256::DIGEST_SIZE << std::endl;
#if defined(x86_64)
    char temp[] = "0064";
#else
    char temp[] = "0032";
#endif
    
    this->socket->write(temp, 4);
    
    //Discard the first read?
    //read = this->socket->read(buff, 256);
    //
    if ((read = this->socket->read(buff, 256)) != 4){
        *this->logFile << "Did not receive 4 bytes as handshake, received " << read << " bytes." << std::endl;
        this->socket->closeConnection();
        return false;
    }
    
    for(int i = 0; i < 4; i++){
        if (buff[i] != temp[i]){
            *this->logFile << "Byte " << i << " differed from sent. [" << buff[i] << "] != [" << temp[i] << "]" << std::endl;
            this->socket->closeConnection();
            return false;
        }
    }
    
    //Get the hashes and send them
#if defined(x86_64)
    this->getHashOfFile("libCPL64.dll", mySHA256);
#else
    this->getHashOfFile("libCPL32.dll", mySHA256);
#endif
    this->getHashOfFile("Plugins/TestPluginLoader.jar", pluginLoaderSHA256);
    this->getHashOfFile("Plugins/TestPlugin/TestPlugin.jar", pluginSHA256);
    
    this->socket->write((char*) mySHA256, SHA256::DIGEST_SIZE);
    this->socket->write((char*) pluginLoaderSHA256, SHA256::DIGEST_SIZE);
    this->socket->write((char*) pluginSHA256, SHA256::DIGEST_SIZE);
    
    if (this->socket->read(buff, 256) > 4){
        *this->logFile << "Did not receive 4 bytes as response, received " << read << " bytes." << std::endl;
        this->socket->closeConnection();
        return false;
    }
    
    if (buff[3] <= 0 || buff[3] > 1){
        *this->logFile << "Third byte <= 0 or > 1 [" << buff[3] << "]." << std::endl;
        this->socket->closeConnection();
        return false;
    }else{
        *this->logFile << "Passed all tests." << std::endl;
    }
    
    return true;
}

bool CPL::decrypt(unsigned char* src, int length, unsigned char* dst) {
    unsigned char iv2[16];
    
    memcpy(this->iv, iv2, 16);
    
    return mbedtls_aes_crypt_cbc(&this->context, MBEDTLS_AES_DECRYPT, length, iv2, src, dst) == 0;
}


void CPL::getDecryptionKey() {
    char buff[16];
    
    this->socket->read(buff, 16);
    memcpy(buff, this->key, 16);
    *this->logFile << "Received key." << std::endl;
    
    this->socket->read(buff, 16);
    memcpy(buff, this->iv, 16);
    *this->logFile << "Received IV." << std::endl;
    
    this->socket->closeConnection();
    
    *this->logFile << "Connection closed." << std::endl;
}

void CPL::initializeDecryptor() {
    mbedtls_aes_setkey_dec(&context, key, 128);
}

std::ofstream* CPL::getOutputStream() {
    return this->logFile;
}







