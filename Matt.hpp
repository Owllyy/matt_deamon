#pragma once
#include "Logger.hpp"
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>

#define PORT 4242
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

class Matt
{
private:
    Logger logger;
    int server_fd;
    // struct sockaddr_in address;
    // fd_set client_set;
public:
    Matt() : server_fd(socket(AF_INET, SOCK_STREAM, 0)) {

        bool REUSEADDR_value = true;
        if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &REUSEADDR_value, 1) < 0) {
            throw std::runtime_error("Failed to set socket option of REUSEADDR to true.");
        }
        communicationSetup();
    }

    void communicationSetup(void) {
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(PORT);
        address.sin_addr.s_addr = htonl(INADDR_ANY);

        fd_set client_set;
        int client_sockets[3] = {};
        int max_socket, cur_socket;
        char buffer[BUFFER_SIZE];

        if (bind(this->server_fd, (struct sockaddr *)&address, sizeof address) == -1) {
            throw std::runtime_error("Failed to bind a name to our socket.");
        } if (listen(this->server_fd, 1) == -1) { 
            throw std::runtime_error("Failed to start listening on the daemon socket.");
        }

        int addrlen = sizeof(address);
        while(true)
        {
            FD_ZERO(&client_set);
            FD_SET(this->server_fd, &client_set);
            max_socket = this->server_fd;

            // Adding client socket descriptors to the client fd_set
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                cur_socket = client_sockets[i];
                if (cur_socket > 0)
                    FD_SET(cur_socket, &client_set);
                if (cur_socket > max_socket)
                    max_socket = cur_socket;
            }
            // Waiting on some activity from the sockets
            if (select(max_socket + 1, &client_set, NULL, NULL, NULL) == -1)
            {
                // need proper shutdown
                throw std::runtime_error("Select function failed.");
            }
            // If there is activity on daemon socket_fd it means it's an incomming connection
            if (FD_ISSET(this->server_fd, &client_set))
            {
                if ((cur_socket = accept(this->server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1)
                {
                    // need proper shutdown
                    throw std::runtime_error("Failed accepting incoming client connection.");
                }
                for (int i = 0; i < MAX_CLIENTS; i++)  
                {  
                    // if there is an empty connection slot, you can connect to the daemon
                    if (client_sockets[i] == 0)  
                    {  
                        client_sockets[i] = cur_socket;
                        break;  
                    }  
                }
                send(cur_socket, "Matt daemon: no available connection slot to the daemon.", 57, 0);
            }
            else
            {
                for (int i = 0; i < MAX_CLIENTS; i++)  
                {  
                    cur_socket = client_sockets[i];

                    if (FD_ISSET(cur_socket , &client_set))  
                    {  
                        // if connection is closed
                        if (read(cur_socket , buffer, BUFFER_SIZE - 1) == 0)  
                        {  
                            close(cur_socket);  
                            client_sockets[i] = 0;  
                        }  
                        else 
                        {  
                            // client message received
                            buffer[BUFFER_SIZE - 1] = 0;  
                            // TODO: A call to the execution function appears here
                        }  
                    }  
                }
            }
        }
    }
};

