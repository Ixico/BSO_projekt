#include "../Headers/FileManager.h"
#include "iostream"
#include <fstream>
#include <filesystem>
#include <sys/statfs.h>
using std::string;
using std::filesystem::recursive_directory_iterator;
using std::vector;
const int DIGEST_LENGTH = 32;
using std::filesystem::path;


std::unordered_set <string> readDatabaseRecords(const path& file_path){
    std::ifstream file;
    file.open(file_path);
    if (!file.is_open()) throw std::invalid_argument("File not found ex");
    string line;
    std::unordered_set<string> lines;
    for (int i=0; getline(file,line); i++){
        if(line.length() != DIGEST_LENGTH) continue; //omit incorrect records
        lines.insert(line);
    }
    file.close();
    return lines;
}

void saveQuarantineRecords(const vector<QuarantineRecord>& records,const path& quarantine_list_path){
    std::ofstream file;
    file.open(quarantine_list_path);
    if (!file.is_open()) throw std::invalid_argument("Quarantine configuration problem!");
    for (const auto &record : records){
        file << record.file_path << ' ' << record.digest << std::endl;
    }
    file.close();
}


vector<QuarantineRecord> readQuarantineRecords(const path& quarantine_list_path){
    std::ifstream file;
    file.open(quarantine_list_path);
    if (!file.is_open()) throw std::invalid_argument("Quarantine configuration problem!");

    vector<QuarantineRecord> records;

    string line;
    QuarantineRecord temp;
    while (file >> temp.file_path >> temp.digest) {
        records.push_back(temp);
    }
    file.close();
    return records;
}

vector<path> findFilesInDirectory(const path& directory_path){
    vector<path> files;
    for (const auto& dirEntry : recursive_directory_iterator(directory_path, std::filesystem::directory_options::skip_permission_denied))
        if (!std::filesystem::is_directory(dirEntry)) files.push_back(dirEntry.path());
    return files;
}


path followSymlinks(path file_path){
    path target;
    if (std::filesystem::is_symlink(file_path)){
        target = std::filesystem::read_symlink(file_path);
    } else {
        return file_path;
    }
    return std::filesystem::canonical(file_path.parent_path().append(target.string()));
}

bool inRightFilesystem(const path& file_path){
    struct statfs buf{};

    if (statfs(file_path.string().c_str(),&buf) == -1) throw std::invalid_argument("Cannot check file's filesystem!");
    return buf.f_type == 61267;
}
