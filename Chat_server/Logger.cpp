#include "Logger.h"

Logger::Logger() {
  if (fs::exists(filename)) {
    s.open(filename, std::fstream::binary | std::fstream::app |
                         std::fstream::out | std::fstream::in);
    std::cout << "file: " << filename << " is open" << std::endl;
  } else {
    s = std::fstream{filename, std::fstream::binary | std::fstream::app |
                                   std::fstream::out | std::fstream::in};
    std::cout << "file: " << filename << " created" << std::endl;
  };
}

Logger::~Logger() { s.close(); }

no_error Logger::writeLine(const Chat_t chat, const User_t user,
                           const Message_t message) {
  if (!(chat && user && message)) no_error(false);
  // // write

  std::lock_guard<std::shared_mutex> lg(mutex);  // lock all
  if (!(s.is_open())) {
    // std::cout << "s IS NOT OPEN" << std::endl;
    return no_error(false);
  } else {
    // s.end;
    //  текущие дата/время основываясь на текущей системе
    time_t now = time(0);
    char dt[26];
    ctime_r(&now, dt);
    dt[24] = ' ';
    s << std::string(dt) << sep << "Чат:" << sep << chat->getChatName() << sep
      << "Пользователь:" << sep << user->getLogin() << sep << "Писал:" << sep
      << message->getMessageText() << '\n';
    s.sync();
  }
  return no_error(true);
}

std::string Logger::readLine(void) {
  // read
  std::shared_lock<std::shared_mutex> mutex; // lock write
  if (!s.is_open())
  	return std::string("");
  else
  {
  	s.seekg(s.beg);
  	std::string line; std::getline(s, line);
  	return line;
  }
}

