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
#include <filesystem>
#include <ctime>

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool creatDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
}

bool concurrencyOpen(const std::string& path) {
    return std::filesystem::create_directories(path);
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