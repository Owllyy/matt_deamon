#pragma once
#include "Tintin_reporter.hpp"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <csignal>
#include <cerrno>

#define PORT 4242
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

extern volatile sig_atomic_t g_signal;

class Matt
{
    public:
        Tintin_reporter logger;
    private:
        int server_fd;
        int client_sockets[MAX_CLIENTS];

        void shutdown() {
            logger.log(Tintin_reporter::INFO, "Quitting.");
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] > 0) close(client_sockets[i]);
            }
            close(server_fd);
            unlock();
        }

        void handle_new_connection() {
            struct sockaddr_in address;
            int addrlen = sizeof(address);
            int cur_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            
            if (cur_socket == -1) {
                shutdown();
                throw std::runtime_error("Failed accepting incoming client connection.");
            }
            
            bool connection_accepted = false;
            for (int i = 0; i < MAX_CLIENTS; i++) {  
                if (client_sockets[i] == 0) {  
                    client_sockets[i] = cur_socket;
                    connection_accepted = true;
                    break;  
                }  
            }

            if (!connection_accepted) {
                const char *msg = "Matt daemon: no available connection slot to the daemon.\n";
                send(cur_socket, msg, strlen(msg), 0);
                close(cur_socket);
            }
        }

        bool handle_client_messages(fd_set &client_set) {
            char buffer[BUFFER_SIZE];
            for (int i = 0; i < MAX_CLIENTS; i++) {  
                int cur_socket = client_sockets[i];

                if (FD_ISSET(cur_socket , &client_set)) {  
                    int valread = read(cur_socket , buffer, BUFFER_SIZE - 1);
                    
                    if (valread <= 0) {  
                        close(cur_socket);  
                        client_sockets[i] = 0;  
                    } else {  
                        buffer[valread] = '\0';
                        
                        std::string msg(buffer);

                        while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r')) {
                            msg.pop_back();
                        }

                        if (msg == "quit") {
                            logger.log(Tintin_reporter::INFO, "Request quit.");
                            return false;
                        } else {
                            logger.log(Tintin_reporter::LOG, msg);
                        }
                    }  
                }  
            }
            return true;
        }

    public:
        Matt() : server_fd(socket(AF_INET, SOCK_STREAM, 0)) {
            for (int i = 0; i < MAX_CLIENTS; i++) {
                client_sockets[i] = 0;
            }

            int opt = 1;
            if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                throw std::runtime_error("Failed to set socket option of REUSEADDR to true.");
            }
            communicationSetup();
        }

        void communicationSetup(void) {
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(PORT);
            address.sin_addr.s_addr = htonl(INADDR_ANY);

            if (bind(this->server_fd, (struct sockaddr *)&address, sizeof address) == -1) {
                throw std::runtime_error("Failed to bind a name to our socket.");
            } if (listen(this->server_fd, 1) == -1) { 
                throw std::runtime_error("Failed to start listening on the daemon socket.");
            }

            fd_set client_set;
            while(true)
            {
                if (g_signal != 0) {
                    logger.log(Tintin_reporter::INFO, "Signal handler.");
                    shutdown();
                    return;
                }

                FD_ZERO(&client_set);
                FD_SET(this->server_fd, &client_set);
                int max_socket = this->server_fd;

                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (client_sockets[i] > 0) {
                        FD_SET(client_sockets[i], &client_set);
                        if (client_sockets[i] > max_socket) {
                            max_socket = client_sockets[i];
                        }
                    }
                }
                if (select(max_socket + 1, &client_set, NULL, NULL, NULL) == -1)
                {
                    if (errno == EINTR) {
                        continue; // Loop resets, hits the signal check, and shuts down cleanly
                    }
                    shutdown();
                    throw std::runtime_error("Select function failed.");
                }

                if (FD_ISSET(this->server_fd, &client_set))
                {
                    handle_new_connection();
                }
                else
                {
                    if (!handle_client_messages(client_set)) {
                        shutdown();
                        return;
                    }
                }
            }
        }
};
