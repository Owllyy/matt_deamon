#include "Deamon.hpp"
#include "Tintin_reporter.hpp"
#include "file.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <utility>
#include <string>
#include <cstdlib>

Daemon::Daemon() {
    start();
}

void Daemon::start(void) {
    lock();
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

    umask(DAEMON_UMASK);

    if (chdir("/") == -1) {
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
}