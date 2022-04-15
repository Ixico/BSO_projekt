//
// Created by ixico on 23.03.2022.
//

#include "../Headers/QuarantineController.h"
#include "openssl/aes.h"
#include <filesystem>
#include <cstring>
#include "../Headers/FileManager.h"
#include <openssl/sha.h>
#include "../Headers/FileHasher.h"

using std::filesystem::path;
using std::string;

void QuarantineController::imposeQuarantine(path file_path) {
    //File is already verified by FileController
    file_path = followSymlinks(file_path);
    std::string file_name = file_path.stem();//get file name
    std::filesystem::path destination_path = quarantine_path/file_name;//create new path
    quarantine_records.emplace_back(file_path, calculateFileHash(file_path));//save source path and hash
    useCipher(file_path,destination_path);//encrypt file
    std::filesystem::remove(file_path);//remove from current location
}

bool QuarantineController::removeQuarantine(const std::string& file_name) {
    int position = 0;
    bool found = false;
    for (const auto &item : quarantine_records){
        if (item.file_path.stem() == file_name){
            found = true;
            break;
        }
        position++;
    }
    if (!found) throw std::invalid_argument("File is not on quarantine!");
    QuarantineRecord record = quarantine_records.at(position);
    useCipher(quarantine_path/file_name,record.file_path);
    if (calculateFileHash(record.file_path) == record.digest) {
        std::filesystem::remove(quarantine_path/file_name);
        quarantine_records.erase(quarantine_records.begin() + position);
        return true;
    }
    std::filesystem::remove(record.file_path);
    return false;
}





void QuarantineController::useCipher(const path& file_path, const path& destination_path) {
    int bytes_read, bytes_written;
    unsigned char indata[AES_BLOCK_SIZE];
    unsigned char outdata[AES_BLOCK_SIZE];


    unsigned char hash[SHA256_DIGEST_LENGTH];
    unsigned char *prepared = new unsigned char[password.length()]; //dobra alokacja?
    strcpy(reinterpret_cast<char *>(prepared), password.c_str()); //czy to rzutowanie jest dobre?
    SHA256(prepared,password.length(),hash);
    unsigned char ckey[16];
    unsigned char ivec[16];
    std::copy(std::begin(hash),std::begin(hash)+16,ckey);
    std::copy(std::begin(hash)+16,std::end(hash),ivec);

    AES_KEY key;
    AES_set_encrypt_key(ckey, 128, &key);

    int num = 0;
    FILE *ifp = fopen(file_path.c_str(), "rb");
    FILE *ofp = fopen(destination_path.c_str(), "wb");

    while (1) {
        bytes_read = fread(indata, 1, AES_BLOCK_SIZE, ifp);
        AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num,
                           AES_ENCRYPT);

        bytes_written = fwrite(outdata, 1, bytes_read, ofp);
        if (bytes_read < AES_BLOCK_SIZE)
            break;

    }
    fclose(ifp);
    fclose(ofp);
}



void QuarantineController::saveQuarantineRecords() {
    ::saveQuarantineRecords(quarantine_records,quarantine_list_path);
}



void QuarantineController::setPassword(const string &pwd) {
    password = pwd;
}

QuarantineController::QuarantineController() = default;

void QuarantineController::init(const path& quarantine_directory_path) {
    this->quarantine_path = quarantine_directory_path;
    quarantine_list_path = quarantine_directory_path.parent_path() / "list";

    std::filesystem::create_directories(quarantine_directory_path);
    if (std::filesystem::exists(quarantine_list_path))
        quarantine_records = readQuarantineRecords(quarantine_list_path);
}


const std::vector<QuarantineRecord> &QuarantineController::getQuarantineRecords() const {
    return quarantine_records;
}
