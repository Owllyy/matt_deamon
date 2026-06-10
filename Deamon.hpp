#pragma once
#include "Tintin_reporter.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
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
            lock(lock_path);

            Tintin_reporter logger;
            logger.log(Tintin_reporter::INFO, "Started.");
            logger.log(Tintin_reporter::INFO, "Creating server.");
            logger.log(Tintin_reporter::INFO, "Server created.");
            logger.log(Tintin_reporter::INFO, "Entering Daemon mode.");

            pid_t pid = fork();
            
            if (pid > 0) {
                exit(EXIT_SUCCESS);
            } else if (pid < 0) {
                exit(EXIT_FAILURE);
            }

            if (setsid() < 0) {
                exit(EXIT_FAILURE);
            }

            pid = fork();
            if (pid > 0) {
                exit(EXIT_SUCCESS);
            } else if (pid < 0) {
                exit(EXIT_FAILURE);
            }

            umask(mask);

            if (chdir(working_directory) == -1) {
                throw std::runtime_error("Failed to change daemon working directory.");
            }

            int fd = open("/dev/null", O_RDWR);
            if (fd >= 0) {
                dup2(fd, STDIN_FILENO);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                if (fd > STDERR_FILENO) {
                    close(fd);
                }
            }

            logger.log(Tintin_reporter::INFO, "started. PID: " + std::to_string(getpid()) + ".");

            return std::move(*this);
        }
};
