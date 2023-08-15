#pragma once
#include "Logger.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <filesystem>
#include "file.hpp"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

class Daemon
{
    private:
        const char * working_directory;
        mode_t mask;
        const char * lock_path;

    public:
        Daemon() {
            working_directory = "/";
            mask = 0270;
            lock_path = 0;
        };

        Daemon&& set_umask(mode_t new_mask) && {
            mask = new_mask;
            return std::move(*this);
        }

        Daemon&& lock_file(const char * path) && {
            lock_path = path;
            return std::move(*this);
        }

        Daemon&& work_directory(const char * path) && {
            working_directory = path;
            return std::move(*this);
        }

        Daemon&& start(void) && {
            int pid = fork();
            
            if (pid > 0) {
                exit(EXIT_SUCCESS);
            } else if (pid < 0) {
                exit(EXIT_FAILURE);
            }

            umask(mask);

            setsid();

            if (chdir(working_directory) == -1) {
                throw std::runtime_error("Failed to change daemon working directory.");
            }

            lock(lock_path);

            return std::move(*this);
        }
};
