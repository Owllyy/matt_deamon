#include "Deamon.hpp"
#include "file.hpp"
#include "Tintin_reporter.hpp"
#include "Matt.hpp"

#define PORT 4242
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

int main(void)
{

	Daemon().lock_file("/var/lock/matt_deamon/matt_deamon.lock")
			.set_umask(7777)
			.work_directory("/")
			.start();

	Matt matt;
	// try {
	// 	lock();

	// 	Tintin_reporter logger;
	// 	logger.log(Tintin_reporter::INFO, "Creating server.");
	// 	logger.log(Tintin_reporter::LOG, "Salut !");
	// 	logger.log(Tintin_reporter::ERROR, "Error file locked.");

	// 	unlock();
	// } catch (std::exception &e) {
	// 	std::cerr << e.what() << std::endl;
	// }
}
