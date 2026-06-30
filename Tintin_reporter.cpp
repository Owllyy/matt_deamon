#include "Tintin_reporter.hpp"
#include "file.hpp"
#include <iostream>
#include <ctime>
#include <ostream>
#include <stdexcept>

Tintin_reporter& Tintin_reporter::getInstance() {
    static Tintin_reporter instance;
    return instance;
}

Tintin_reporter::Tintin_reporter() : is_valid(true) {
    try {
        build_dir_path(LOG_FILE);
        logfile.open(LOG_FILE, std::ios_base::app);
        if (!logfile.is_open()) {
            is_valid = false;
        }
    } catch (const std::exception& e) {
        is_valid = false;
    }
}

Tintin_reporter::~Tintin_reporter() {
    if (logfile.is_open()) {
        logfile.flush();
        logfile.close();
    }
}

std::ostream & Tintin_reporter::print_time_stamp(std::ostream & file) {
    char time_buffer[80];

    std::time_t now = std::time(nullptr);
    if (std::strftime(time_buffer, 80, "[%d/%m/%Y-%H:%M:%S]", std::localtime(&now)) <= 0)
        std::cerr << "Probleme time strftime" << std::endl;

    return file << time_buffer;
}

void Tintin_reporter::log(logTag tag, std::string_view msg) {
    Tintin_reporter& instance = getInstance();
    if (!instance.is_valid) {
        std::cerr << msg << std::endl;
        return;
    }

    print_time_stamp(instance.logfile);
    switch (tag)
    {
        case ERROR:
            instance.logfile << " [ ERROR ] - Matt_daemon: ";
            break;
        case INFO:
            instance.logfile << " [ INFO ] - Matt_daemon: ";
            break;
        case LOG:
            instance.logfile << " [ LOG ] - Matt_daemon: User input: ";
            break;
    }
    instance.logfile << msg << std::endl;
}