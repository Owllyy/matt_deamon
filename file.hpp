#pragma once

#include <string>

#define LOCK_PATH "/var/lock/matt_daemon.lock"

int openCreat(const std::string& path);
bool fileExists(const std::string& path);
bool create_dir(const std::string& path);
void ensure_dir(const std::string& path);
void build_dir_path(const std::string& dir_path);
void lock();
void unlock();