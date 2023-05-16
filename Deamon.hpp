#pragma once
#include "Logger.hpp"

class Deamon
{
    private:
        const static unsigned int port = 4242;
        Logger logger;
    public:
        Deamon();
        ~Deamon();
};

Deamon::Deamon()
{
    logger.log();
    //    pid_t pid, sid;

//     if ()
//    pid = fork();
//    if (pid > 0) {
//         std::ofstream outfile ("test.txt");
//         outfile << "my text here!" << std::endl;
//         outfile.close();
//       exit(EXIT_SUCCESS);
//    } else if (pid < 0) {
//       exit(EXIT_FAILURE);
//    }

//    umask(0);

//    // Open system logs for the child process
//    openlog("daemon-named", LOG_NOWAIT | LOG_PID, LOG_USER);
//    syslog(LOG_NOTICE, "Successfully started daemon-name");

// //    // Generate a session ID for the child process
// //    sid = setsid();
// //    // Ensure a valid SID for the child process
// //    if(sid < 0)
// //    {
// //       // Log failure and exit
// //       syslog(LOG_ERR, "Could not generate session ID for child process");

// //       // If a new session ID could not be generated, we must terminate the child process
// //       // or it will be orphaned
// //       exit(EXIT_FAILURE);
// //    }

// //    // Change the current working directory to a directory guaranteed to exist
// //    if((chdir("/")) < 0)
// //    {
// //       // Log failure and exit
// //       syslog(LOG_ERR, "Could not change working directory to /");

// //       // If our guaranteed directory does not exist, terminate the child process to ensure
// //       // the daemon has not been hijacked
// //       exit(EXIT_FAILURE);
// //    }

//    // A daemon cannot use the terminal, so close standard file descriptors for security reasons
//    close(STDIN_FILENO);
//    close(STDOUT_FILENO);
//    close(STDERR_FILENO);

//    // Daemon-specific intialization should go here
//    const int SLEEP_INTERVAL = 5;

//    // Enter daemon loop
//    while(1)
//    {
//       // Execute daemon heartbeat, where your recurring activity occurs
//       do_heartbeat();

//       // Sleep for a period of time
//       sleep(SLEEP_INTERVAL);
//    }

//    // Close system logs for the child process
//    syslog(LOG_NOTICE, "Stopping daemon-name");
//    closelog();

//    // Terminate the child process when the daemon completes
//    exit(EXIT_SUCCESS);
}

Deamon::~Deamon()
{
}
