#pragma once
#include <time.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "Message.h"
#include "User.h"

class Chat;
class User;
class Message;

namespace fs = std::filesystem;
using no_error = bool;
using User_t = std::shared_ptr<User>;  // указатель на юзера
using Chat_t = std::shared_ptr<Chat>;  // указатель на чат
using Message_t = std::shared_ptr<Message>;  // указатель на чат

class Logger {
 private:
  std::shared_mutex mutex;
  std::string filename{"log.txt"};
  std::fstream s;
  const std::string sep{" "};  // separator

 public:
  Logger();
  ~Logger();
  void createPath(const fs::path &Path, const fs::path &savePath);
  no_error writeLine(const Chat_t chat, const User_t user,
                     const Message_t message);
  std::string readLine(void);
};
