#pragma once
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "DB_Queries_DDL.h"
#include "DB_Queries_DML.h"
#include "Message.h"
#include "SaveRestor.h"
#include "Server.h"
#include "TCP_server.h"
#include "User.h"

using User_t = std::shared_ptr<User>;  // указатель на юзера
using Chat_t = std::shared_ptr<Chat>;  // указатель на чат
using Message_t = std::shared_ptr<Message>;  // указатель на сообщение
using no_errors = bool;  // Execution OK. No errors occurred if = true;
using is_errors = bool;  // Execution Not OK. Errors occurred if = true;
using queue_string_t = std::shared_ptr<std::queue<std::string>>; 

class Chats {
  // переменные
  bool Q{false};
  int cmd{0};
  std::string cmd_input;
  std::vector<User_t> users;  // массив указателей на пользователей
  std::vector<Chat_t> chats;  // массив указателей на чаты
  User_t currentUserPtr{nullptr};  // указатель на текущего пользователя
  Chat_t currentChatPtr{nullptr};  // указатель на текущий чат
  DB_Queries_DDL db_queries_ddl;
  DB_Queries_DML db_queries_dml;  // DML запросы к MySQL
  size_t lastSendMessageId{0};

 public:
  Chats(){};
  ~Chats(){};

  friend void SaveRestor::saveUsers(std::vector<User_t>& users);

  void prepare(void);
  void shutdown(void);

  // Main menu
  void mainmenu();
  // выводим  данные текущего пользовател
  void userinfo();

  // тут можно сделать возвращаемое значение истина в случае успеха и передавать
  // в функцию готового юзера для входа чтобы унифицировать процедуру с
  // регистрацией и не вводить по десять раз одно и то же например сделать
  // функцию инсерт юзер дата, которая возвращает юзера

  // User logon
  User_t logon(const User_t user);
  void logon();
  // тут можно сделать возвращаемое значение истина в случае успеха
  User_t userRegistration(const User_t user);
  no_errors userRegistration();
  // Написать пользователю
  void write();
  // logoff
  void logoff();
  // Выход из программы
  void exit();

  // Restore from files
  void restordata();
  // Save to files
  void savedata();

  void localCycle(void);
  void remoteCycle();

  User_t getUserByLogin(const std::string& companion);
  std::string getChatNameByUsers(const User_t user, const User_t companion);
  Chat_t getChatByUsers(const User_t user, const User_t companion);
  Chat_t createChatByUsers(const User_t user, const User_t companion);
  Message_t addMessage(const Chat_t chat, const Message_t message);
  no_errors printAllUsers(void);
  std::shared_ptr<std::queue<std::string>> getUserNames();
  const std::string getMessageString(const Message_t message);
  no_errors printAllMessages(const Chat_t chat);
  queue_message_t getUnreadMessages(const Chat_t chat);

  insert_id_t sendMessage(const Chat_t chat, const User_t user,
                        std::string message_text);
  void saveLastSendMessageId(size_t id);
  std::string acquaireMessage(void);
};
