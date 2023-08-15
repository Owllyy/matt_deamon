#pragma once
#include "file.hpp"
#include <fstream>
#include <iostream>
#include <ctime>

#define LOG_PATH "/var/log/matt_daemon/matt_daemon.log"

/*
    A logger class that use the method log() to log into the file defined by LOG_PATH.
*/
class Logger
{
    private:
        std::ofstream logfile;
    public:

        enum logTag {
            LOG,
            ERROR,
            INFO
        };
        
        Logger() : logfile(open_log(LOG_PATH)) {}
        ~Logger() { logfile.flush(); logfile.close(); }

        std::ofstream static open_log(const std::string& path) {
            excl_build_dir_path(LOG_PATH);
            std::ofstream file(path);

            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file " + path);
            }

            return file;
        }

        static std::ostream & get_time_stamp(std::ostream & file)
        {
            char time_buffer[80];

            std::time_t now = std::time(nullptr);
            if (std::strftime(time_buffer, 80, "[%d/%m/%Y-%H:%M:%S]", std::localtime(&now)) <= 0)
                std::cerr << "Probleme time strftime" << std::endl;

            return file << time_buffer;
        }

        /*
            Log into a logfile your message in a format:
            [d/m/Y-H:M:S] [ TAG ] - Matt_daemon: MSG

            @param tag: Logger::ERROR, Logger::INFO or Logger::LOG
            @param msg: [char] or string
        */
        void log(logTag tag, std::string_view msg) {
            logfile << &get_time_stamp;
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
};