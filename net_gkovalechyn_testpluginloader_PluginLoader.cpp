#include "net_gkovalechyn_testpluginloader_PluginLoader.h"
#include "CPL.h"
#include "WinUDPSocket.h"
#include "aes.h"

jboolean Java_net_gkovalechyn_testpluginloader_PluginLoader_initialize(JNIEnv* env, jobject obj){
    mbedtls_aes_self_test(true);
    
    if (!CPL::getInstance()->connect()){
        return false;
    }
    
    if (!CPL::getInstance()->doHandshake()){
        return false;
    }
    
    CPL::getInstance()->getDecryptionKey();
    CPL::getInstance()->initializeDecryptor();
    
    return true;
}

jbyteArray Java_net_gkovalechyn_testpluginloader_PluginLoader_translateClass(JNIEnv* env, jobject obj, jbyteArray encryptedClass){
    int length = env->GetArrayLength(encryptedClass);
    unsigned char* decrypted = new unsigned char[length];
    unsigned char* encrypted = new unsigned char[length];
    jbyteArray result;
    
    *CPL::getInstance()->getOutputStream() << "Received decrypt request." << std::endl;
    
    env->GetByteArrayRegion(encryptedClass, 0, length, reinterpret_cast<jbyte*>(encrypted));
    
    if (!CPL::getInstance()->decrypt(encrypted, length, decrypted)){
        *(CPL::getInstance()->getOutputStream()) << "Could not decrypt data." << std::endl;
        delete[] encrypted;
        delete[] decrypted;
        return NULL;
    }
    
    result = env->NewByteArray(length);
    
    env->SetByteArrayRegion(result, 0, length, reinterpret_cast<jbyte*>(decrypted));
    //env->DeleteLocalRef(result);
    delete[] encrypted;
    delete[] decrypted;
    
    return result;
}
