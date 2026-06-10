#include "Deamon.hpp"
#include "file.hpp"
#include "Tintin_reporter.hpp"
#include "Matt.hpp"
#include <unistd.h>
#include <iostream>
#include <csignal>

volatile sig_atomic_t g_signal = 0;

void signal_handler(int signum) {
    g_signal = signum;
}

int main(void)
{
    if (getuid() != 0) {
        std::cerr << "Matt_daemon must be run as root." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        Daemon().lock_file("/var/lock/matt_daemon.lock")
                .set_umask(027)
                .work_directory("/")
                .start();

        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGQUIT, signal_handler);
        signal(SIGPIPE, SIG_IGN);

        Matt matt;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
