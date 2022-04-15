//
// Created by ixico on 17.03.2022.
//

#include "../Headers/FileHasher.h"
#include "../Headers/FileHasher.h"
#include <iostream>
#include <openssl/md5.h>
#include <fstream>
#include "../Headers/FileManager.h"

using std::string;


//TODO: consider other options for digest
string hashToHexString(unsigned char *digest){
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i=0; i < MD5_LENGTH; i++){
        ss << std::setw(2) << static_cast<unsigned>(digest[i]);
    }
    return ss.str();
}


string calculateFileHash(const string& file_name){
    MD5_CTX ctx;
    MD5_Init(&ctx);

    std::ifstream is(file_name);
    char buf[4096];
    do {
        is.read(buf, sizeof(buf));
        MD5_Update(&ctx, buf, is.gcount());
    } while(is);
    unsigned char digest[MD5_LENGTH];
    MD5_Final(digest, &ctx);
    return hashToHexString(digest);
}

