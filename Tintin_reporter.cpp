#include "Tintin_reporter.hpp"
#include "file.hpp"
#include <iostream>
#include <ctime>
#include <stdexcept>

Tintin_reporter::Tintin_reporter() : logfile(open_log(LOG_FILE)) {}

Tintin_reporter::~Tintin_reporter() {
    logfile.flush();
    logfile.close();
}

std::ofstream Tintin_reporter::open_log(const std::string& path) {
    excl_build_dir_path(path);
    std::ofstream file(path, std::ios_base::app);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file " + path);
    }

    return file;
}

std::ostream & Tintin_reporter::print_time_stamp(std::ostream & file) {
    char time_buffer[80];

    std::time_t now = std::time(nullptr);
    if (std::strftime(time_buffer, 80, "[%d/%m/%Y-%H:%M:%S]", std::localtime(&now)) <= 0)
        std::cerr << "Probleme time strftime" << std::endl;

    return file << time_buffer;
}

void Tintin_reporter::log(logTag tag, std::string_view msg) {
    print_time_stamp(logfile);
    switch (tag)
    {
        case ERROR:
            logfile << " [ ERROR ] - Matt_daemon: ";
            break;
        case INFO:
            logfile << " [ INFO ] - Matt_daemon: ";
            break;
        case LOG:
            logfile << " [ LOG ] - Matt_daemon: User input: ";
            break;
    }
    logfile << msg << std::endl;
}