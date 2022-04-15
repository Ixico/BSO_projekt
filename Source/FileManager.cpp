#include "../Headers/FileManager.h"
#include "iostream"
#include <fstream>
#include <filesystem>
#include "../Headers/ConfigurationVariables.h"
#include <sys/statfs.h>
using std::string;
using std::filesystem::recursive_directory_iterator;
using std::vector;
const int DIGEST_LENGTH = 32;
using std::filesystem::path;


//TODO manage file types and symlinks
std::unordered_set <string> readDatabaseRecords(path file_path){
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

void saveQuarantineRecords(vector<QuarantineRecord> records,path quarantine_list_path){
    std::cout << "hi!" << std::endl;
    std::ofstream file;
    file.open(quarantine_list_path);
    if (!file.is_open()) throw std::invalid_argument("File not found!");
    for (const auto &record : records){
        file << record.file_path << ' ' << record.digest << std::endl;
    }
    file.close();
}

//TODO: std::out_of_range exc
vector<QuarantineRecord> readQuarantineRecords(path quarantine_list_path){
    std::ifstream file;
    file.open(quarantine_list_path);
    if (!file.is_open()) throw std::invalid_argument("File not found listq");

    vector<QuarantineRecord> records;

    string line;
    QuarantineRecord temp;
    while (file >> temp.file_path >> temp.digest) {
        records.push_back(temp);
    }
    file.close();
    return records;
}

//todo file permission denied, symlinks to folder
vector<path> findFilesInDirectory(path directory_path){
    vector<path> files;
    for (const auto& dirEntry : recursive_directory_iterator(directory_path, std::filesystem::directory_options::skip_permission_denied))
        if (!std::filesystem::is_directory(dirEntry)) files.push_back(dirEntry.path());
    return files;
}

//TODO: path for relative symlink
path followSymlinks(path file_path){
    path target;
    if (std::filesystem::is_symlink(file_path)){
        target = std::filesystem::read_symlink(file_path);
    } else {
        return file_path;
    }
    return std::filesystem::canonical(file_path.parent_path().append(target.string()));
}

bool inRightFilesystem(path file_path){
    struct statfs buf;

    if (statfs(file_path.string().c_str(),&buf) == -1) throw std::invalid_argument("Cannot check file's filesystem!");
    return buf.f_type == 61267;
}
