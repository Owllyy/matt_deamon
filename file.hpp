#pragma once

#include <string>

#define LOCK_PATH "/var/lock/matt_daemon.lock"

int exclusiveOpenCreat(const std::string& path);
bool fileExists(const std::string& path);
bool create_dir(const std::string& path);
void excl_dir(const std::string& path);
void excl_build_dir_path(const std::string& dir_path);
void lock();
void unlock();