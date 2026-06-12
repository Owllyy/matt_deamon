#pragma once
#include <fstream>
#include <string_view>
#include <string>

#define LOG_FILE "/var/log/matt_daemon/matt_daemon.log"

/*
    A logger class that use the method log() to log into the file defined by LOG_FILE.
*/
class Tintin_reporter
{
    private:
        std::ofstream logfile;
        Tintin_reporter();
        ~Tintin_reporter();
    public:

        enum logTag {
            LOG,
            ERROR,
            INFO
        };
        
        Tintin_reporter(const Tintin_reporter&) = delete;
        Tintin_reporter& operator=(const Tintin_reporter&) = delete;

        static Tintin_reporter& getInstance();

        static std::ofstream open_log(const std::string& path);
        static std::ostream & print_time_stamp(std::ostream & file);
        static void log(logTag tag, std::string_view msg);
};