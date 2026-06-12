#pragma once
#include <sys/stat.h>
#include <sys/types.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

class Daemon
{
    private:
        static const mode_t DAEMON_UMASK = 077;

    public:
        Daemon();
};
