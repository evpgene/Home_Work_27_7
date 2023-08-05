
#include "Chats.h"

// получет пользователя из базёнки по указанному логину
User_t Chats::getUserByLogin(const std::string &login) {
  return db_queries_dml.select_User_By_Login_fc(login);
}

// формирует имя чата. В случае неудачи возвращает пустую строку.
std::string Chats::getChatNameByUsers(const User_t user,
                                      const User_t companion) {
  // проверяем, в порядке ли исходные данные
  if (!(user && companion)) {
    return std::string();
  }
  // формируем имя чата
  if (companion->getLogin() == std::string("Общий")) {
    return std::string("Общий");
  }
  if (user->getLogin() <= companion->getLogin()) {
    return std::string(user->getLogin() + companion->getLogin());
  } else {
    return std::string(companion->getLogin() + user->getLogin());
  }
}

// Возвращает указатель на чат вынутый из базёнки. В случае неудачи возвращает
// nullptr.
Chat_t Chats::getChatByUsers(const User_t user, const User_t companion) {
  // проверяем, в порядке ли исходные данные
  if (!(user && companion)) {
    return nullptr;
  }
  // формируем имя чата
  std::string chat_name{getChatNameByUsers(user, companion)};
  #if _DEBUG
  std::cout << "getChatNameByUsers(user, companion): ";
  std::cout << chat_name;
  std::cout << std::endl;
  #endif 
  if(chat_name.empty() || chat_name == ""){
    std::cout << "chatname  empty ";
    std::cout << std::endl;
    return nullptr;}
  // и проверяем,
  Chat_t chat = db_queries_dml.select_Chat_By_Name_fc(chat_name);
  if(!bool(chat)) {
    #if _DEBUG
      std::cout <<  " getChatByUsers() returned nullptr ";
      std::cout << std::endl;
    #endif
    return nullptr;
    }
  // существует ли чат с заданным именeм
  return chat;
}

// Создаёт чат в базёнке с указанными пользователями.  случае неудачи возвращает
// nullptr.
Chat_t Chats::createChatByUsers(const User_t user, const User_t companion) {

  #if _DEBUG
    std::cout << "Inserted un SQL state createChatByUsers() ";
    std::cout << std::endl;
  #endif

  // проверяем, в порядке ли исходные данные
  if (!(user && companion)) {
    return nullptr;
  }
  if (!(user->getId() && companion->getId())) {
    return nullptr;
  }

  no_errors no_errors{true};
  // по идее эти операции надо проводить транзакцией!!!
  size_t chat_id =
      db_queries_dml.insert_Chat_fc(getChatNameByUsers(user, companion));
  unsigned int user_user_no{2}, companion_user_no{1};
  if (user->getLogin() < companion->getLogin()) {
    user_user_no = 1, companion_user_no = 2;
  }
  no_errors &= bool(
      db_queries_dml.insert_Chat_User_fc(chat_id, user->getId(), user_user_no));
  // if(*user != *companion)
  no_errors &= bool(db_queries_dml.insert_Chat_User_fc(
      chat_id, companion->getId(), companion_user_no));
  // либо тут должно быть удалить созданный чат если есть ошибки!!!

  return Chat_t(db_queries_dml.select_Chat_By_Id_fc(chat_id));
}

// отправляет сообщение в базёнку и возвращает
// вновь сконструированное сообщение из базы
// в том числе и id сообщения
Message_t Chats::addMessage(const Chat_t chat, const Message_t message) {
  no_errors no_errors{true};
  User_t user = db_queries_dml.select_User_By_Login_fc(message->getUserLogin());
  if (!(no_errors &= bool(user))) return nullptr;
#if _DEBUG
  std::cout << "Пользователь, кто отправляет сообщение: ";
  user->printUser();
  std::cout << std::endl;
#endif
  std::size_t chat_user =
      db_queries_dml.select_Chat_User_fc(chat->getId(), user->getId());
  if (!(no_errors &= bool(chat_user))) return nullptr;
#if _DEBUG
  std::cout << "Чатюзер, кто отправляет сообщение: ";
  std::cout << chat_user;
  std::cout << std::endl;
#endif
#if _DEBUG
  std::cout << "Сообщение: ";
  std::cout << message->getMessageText();
  std::cout << std::endl;
#endif
  size_t message_id =
      db_queries_dml.insert_Message_fc(chat_user, message->getMessageText());
  if (!(no_errors &= bool(message_id))) return nullptr;
  return db_queries_dml.select_Message_fc(chat->getId(), message_id);
}

// распечатывает перечень всех пользователей вынутых из базёнки,
// в случае удачи возвращает true.
no_errors Chats::printAllUsers(void) {
  queue_user_t all_users = db_queries_dml.select_Users_All_fc();
  if (!all_users) {
    std::cout << "Нет зарегистрированных пользователей! " << std::endl;
    return no_errors(false);
  }
  // выводим имена всех пользователей
  std::cout << "Зарегистрированные пользователи: " << std::endl;
  while (!all_users->empty()) {
    all_users->front().printUser();
    std::cout << endl;
    all_users->pop();
  }
  return no_errors(true);
}

// проверить необходимость этой  функции и при ненадобности - заменить
//  тут необходимо учесть id
queue_string_t Chats::getUserNames() {
  using queue_str = std::queue<std::string>;
  queue_user_t all_users(db_queries_dml.select_Users_All_fc());
  queue_string_t usernames(std::make_shared<queue_str>(queue_str()));
  if (!all_users) {
    return nullptr;
  }
  while (!all_users->empty()) {
    usernames->push(all_users->front().getLogin());
    all_users->pop();
  }
  return usernames;
}

// распечатывает все сообщения указанного чата
// вынутые из базёнки
// в случае удачи возвращает true.
// разобраться со статусом
no_errors Chats::printAllMessages(const Chat_t chat) {
  if (!chat) return no_errors(false);
  size_t chat_id =
      db_queries_dml.select_Chat_By_Name_fc(chat->getChatName())->getId();

#if _DEBUG
  std::cout << " chat id - " << chat_id << std::endl;
#endif

  queue_message_t messages = db_queries_dml.select_Messages_Mult_fc(
      chat_id, 1);
  if (!messages) {
    std::cout << "Нет сообщений! " << std::endl;
    return no_errors(false);
  }
  // выводим все сообщения
  std::cout << "Сообщения: " << std::endl;
  while (!(messages->empty())) {
    messages->front().printMessage();
    std::cout << endl;
    messages->pop();
  }
  return no_errors(true);
}

// Возвращает непрочитанные сообщения для сетевого интерфейса
queue_message_t Chats::getUnreadMessages(const Chat_t chat) {
  if (!chat) return nullptr;
  size_t chat_id =
      db_queries_dml.select_Chat_By_Name_fc(chat->getChatName())->getId();

#if _DEBUG
  std::cout << " chat id - " << chat_id << std::endl;
#endif

  queue_message_t messages = db_queries_dml.select_Messages_Mult_fc(
      chat_id, 1);
  if (!messages) {
    std::cout << "Нет сообщений! " << std::endl;
    return nullptr;
  }
  // // выводим все сообщения
  // std::cout << "Сообщения: " << std::endl;
  // while (!(messages->empty())) {
  //   messages->front().printMessage();
  //   std::cout << endl;
  //   messages->pop();
  // }
  return messages;
}

// отправляет сообщение в указанный чат в базёнку от указанного пользователя
insert_id_t Chats::sendMessage(const Chat_t chat, const User_t user,
                             std::string message_text) {
  // проверяем исходные дынные
  if (!(chat && user)) return 0;
  if (!(chat->getId() && user->getId())) return 0;
  #if _DEBUG
  std::cout << " ИД чата для отправки сообщения: ";
  std::cout << chat->getId();
  std::cout << std::endl;
  std::cout << " ИД пользователя для отправки сообщения: ";
  std::cout << user->getId();
  std::cout << std::endl;
  #endif

  std::size_t chat_user_id =
      db_queries_dml.select_Chat_User_fc(chat->getId(), user->getId());
#if _DEBUG
  std::cout << "chat_user_id: " << chat_user_id << std::endl;
#endif

  std::size_t message_id =
      db_queries_dml.insert_Message_fc(chat_user_id, message_text);
#if _DEBUG
  std::cout << "message_id: " << message_id << std::endl;
#endif
  if (bool(message_id)) {
    saveLastSendMessageId(message_id);
  }
  return insert_id_t(message_id);
}

// сохраняет id последнего отправленного в базёнку сообщения
void Chats::saveLastSendMessageId(size_t id) { lastSendMessageId = id; }

// запрашивает ввод с консоли и возвращает текст сообщения
std::string Chats::acquaireMessage(void) {
  std::string message_text;
  std::cout << "Введите ваше сообщение: ";
  cin.ignore();
  std::getline(std::cin, message_text);
  return std::string(message_text);
}
