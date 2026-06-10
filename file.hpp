#pragma once

#include <iostream>
#include <filesystem>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/file.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <unistd.h>

#define LOCK_PATH "/var/lock/matt_daemon.lock"

using namespace std::filesystem;

int exclusiveOpenCreat(const std::string& path) {

    int fd = open(path.c_str(), O_CREAT | O_RDWR, 0666);

    if (fd < 0) {
        throw(std::runtime_error("Can't open :" + path));
    } else if (flock( fd, LOCK_EX | LOCK_NB )) { 
        close(fd);
        throw(std::runtime_error("Can't open :" + path));
    }

    return fd;
}

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool create_dir(const std::string& path) {
    return std::filesystem::create_directories(path);
}

void excl_dir(const std::string& path) {
    if (!fileExists(path)) {
        if (!create_dir(path)) {
            throw std::runtime_error("Can't creat directory : " + path);
        }
    }
}

void excl_build_dir_path(const std::string& dir_path) {
	path full_path(dir_path);
    full_path.remove_filename();
    path ancestors;

	for (path::iterator it = full_path.begin(); it != full_path.end(); it++) {
        ancestors /= path(*it);
        excl_dir(ancestors);
	}
}

static int lock_fd = -1;

/*
    Create a new lockfile at LOCK_PATH.
    It also build the whole path before creating the file.

    It atomicly ensure that the lockfile is created, if it already exist
    the function throw a std::runtime_error.
*/
void lock() {
    excl_build_dir_path(LOCK_PATH);
    lock_fd = exclusiveOpenCreat(LOCK_PATH);
}

/*
    Create a new lockfile at lock_path.
    It also build the whole path before creating the file.

    It atomicly ensure that the lockfile is created, if it already exist
    the function throw a std::runtime_error.
*/
void lock(const char *lock_path) {
    excl_build_dir_path(lock_path);
    lock_fd = exclusiveOpenCreat(lock_path);
}

/*
    Remove the lockfile at LOCK_PATH.

    It ensure that the lockfile is deleted, else the function throw a std::runtime_error.
*/
void unlock() {
    if (lock_fd != -1) {
        close(lock_fd);
        lock_fd = -1;
    }
    if (remove(LOCK_PATH)) { 
        throw(std::runtime_error("Failed to unlock file: " + std::string(strerror( errno ))));
    }
}