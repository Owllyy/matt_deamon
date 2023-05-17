#pragma once
#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#define _XOPEN_SOURCE
#define _POSIX_SOURCE
#include <fcntl.h>
#include <ctime>

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool creatDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
}

std::ofstream concurrencyOpen(const std::string& path) {
    int fileDescriptor = open(path.c_str(), O_CREAT | O_WRONLY | O_EXCL, 0666);
    if (fileDescriptor == -1) {
        throw std::runtime_error("Impossible d'ouvrir le fichier " + path + " en mode exclusif.");
    }

    std::FILE* file = fdopen(fileDescriptor, "w");
    if (!file) {
        close(fileDescriptor);
        throw std::runtime_error("Impossible de convertir le descripteur de fichier en flux de fichier.");
    }

    std::ofstream fileStream;

    if (!fileStream.is_open()) {
        fclose(file);
        throw std::runtime_error("Impossible d'ouvrir le flux de sortie pour le fichier " + path);
    }

    return fileStream;
}

class Logger
{
    private:
        std::ofstream logfile;
    public:
        Logger() : logfile(open_log("/var/log/matt_deamon/matt_deamon.log")) {}
        ~Logger() { logfile.close(); }

        std::ofstream static open_log(const std::string& path) {
            if (!fileExists("/var/log/matt_deamon")) {
                if (!creatDirectory("/var/log/matt_deamon")) {
                    std::cerr << "Can't creat directory : " << path << std::endl;
                }
            }
            std::ofstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file " + path);
            }
            return file;
        }

        static std::ostream & get_time_stamp(std::ofstream & file)
        {
            std::time_t now = std::time(nullptr);
            char buffer[80];
            std::strftime(buffer, 80, "[%d / %m / %Y - %H : %M : %S]", std::localtime(&now));
            file << buffer;
            return file;
        }

        void log() {
            logfile << &get_time_stamp << " TEST "<< std::endl;
        }

        void log(std::ofstream file) {
            file << &get_time_stamp << " TEST "<< std::endl;
        }
};