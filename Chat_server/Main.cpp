
#include <thread>
#include "Chats.h"
#include "DB_Queries_DDL.h"
#include "DB_Queries_DML.h"
using queue_user_t = std::shared_ptr<std::queue<User>>;

int main() {
//   // system("chcp 1251");

  TCP_server tcp_server;
  Chats chats;

  // chats.restordata();

  chats.prepare();

  std::vector<std::thread> clients_thread;

  tcp_server.configureConnection();
  tcp_server.listening();
  

  while(true) {
    tcp_server.openConnection();
    clients_thread.emplace_back(std::thread(&Chats::remoteCycle, std::ref(chats), tcp_server));
  }

for (auto& thread : clients_thread) {
  if(thread.joinable()) {
    thread.join();
  }
}

  // chats.savedata();
  chats.shutdown();

  return 0;
}