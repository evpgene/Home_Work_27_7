
#include "Chats.h"
#include "DB_Queries_DDL.h"
#include "DB_Queries_DML.h"
using queue_user_t = std::shared_ptr<std::queue<User>>;

int main() {
//   // system("chcp 1251");

  Chats chats;
  chats.prepare();
  // chats.restordata();
  chats.remoteCycle();

  // chats.savedata();
  chats.shutdown();

  return 0;
}