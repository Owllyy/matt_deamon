#pragma once
#include "Tintin_reporter.hpp"
#include <csignal>
#include <sys/select.h>

#define PORT 4242
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

extern volatile sig_atomic_t g_signal;

class Matt
{
    private:
        int server_fd;
        int client_sockets[MAX_CLIENTS];

        void shutdown();
        void handle_new_connection();
        bool handle_client_messages(fd_set &client_set);
        void communicationSetup(void);
        void server_loop(void);

    public:
        Matt();
};
