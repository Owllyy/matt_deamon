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

void init_signals() {
    for (int i = 0; i < NSIG; ++i) {
        // Unix kernel does not allow to capture SIGKILL and SIGSTOP
        if (i == SIGKILL || i == SIGSTOP) continue;
        signal(i, signal_handler);
    }
}

int main(void)
{
    Tintin_reporter logger;

    if (getuid() != 0) {
        std::cerr << "Matt_daemon must be run as root." << std::endl;
        return EXIT_FAILURE;
    }

    try {
        logger.log(Tintin_reporter::INFO, "Started.");
        init_signals();
        logger.log(Tintin_reporter::INFO, "Entering Daemon mode.");
        Daemon();
        logger.log(Tintin_reporter::INFO, "Creating server.");
        logger.log(Tintin_reporter::INFO, "started. PID: " + std::to_string(getpid()) + ".");

        Matt matt;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
