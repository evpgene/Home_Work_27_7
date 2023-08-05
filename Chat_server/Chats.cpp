
#include "Chats_functions.cpp"

void Chats::prepare(void) {
  //DB_Queries_DDL db_queries_ddl;
  MYSQL_Config mysqlconfig_ddl;
  mysqlconfig_ddl.passwd = (char*)("Root123_Root123");
  db_queries_ddl.connectToMySQLserver_open(mysqlconfig_ddl);
  db_queries_ddl.createDataBase();
  db_queries_ddl.connectToMySQLserver_close();

  //DB_Queries_DML db_queries_dml;
   MYSQL_Config mysqlconfig_dml = mysqlconfig_ddl;
   mysqlconfig_dml.db = "chat_db";
  db_queries_dml.connectDB_open(mysqlconfig_dml);
  db_queries_dml.prepareAll();
}

void Chats::shutdown(void) {
  db_queries_dml.closeAll();
  //db_queries_ddl.connectToMySQLserver_close();
}

void Chats::mainmenu() {


  // если пользователь общий не найден
  if (!(db_queries_dml.select_User_By_Login_fc("Общий") != nullptr)) {
    size_t user_id{0};
    // создаём пользователя для общего чата
    if (bool(user_id = db_queries_dml.insert_User_fc(
             std::make_shared<User>(User(user_id, "Общий", "Общий"))))) {
      currentUserPtr = db_queries_dml.select_User_By_Id_fc(user_id);
    };
    // создаём чат общий и делаем его активным
    currentChatPtr = createChatByUsers(currentUserPtr, currentUserPtr);
  };
  localCycle();
};

void Chats::userinfo() {
  // распечатываем активного пользоваателя
  if (currentUserPtr) {
      std::cout << "Активный пользователь: ";
      currentUserPtr->printUser();
      std::cout << endl;
  }
  // распечатываем активный чат
  if (currentChatPtr) {
      std::cout << "Активный чат: ";
      currentChatPtr->printChatName();
      std::cout << endl;
  }

  // распечатываем пречень всех зарегистрированных пользователей
  printAllUsers();
}

// logon для терминала - ищет пользователя в базёнке
void Chats::logon() {
  std::string login, pass;
  std::cout << "Введите ваш логин " << std::endl;  // login
  std::cin >> login;
  std::cout << "Введите ваш пароль " << std::endl;
  std::cin >> pass;
  // пусть нуль будет id для временных пользователей
  User_t user(std::make_shared<User>(0, login, pass));
  if (!bool(currentUserPtr =
                db_queries_dml.select_User_By_Login_fc(login))) {
      std::cout << "Такого пользователя нет. " << std::endl;
      return;
  }
  if(*currentUserPtr != *user) {
    currentUserPtr = nullptr;
    std::cout << "Неверные учётные данные. " << std::endl;
  }
}

// logon для сетевого интерфейса - ищет пользователя в базёнке
User_t Chats::logon(const User_t user) {
  if (!user) {return nullptr;} // проверка на нулевой указатель

  User_t restored_user{db_queries_dml.select_User_By_Login_fc(user->getLogin())};
  if (!bool(restored_user)) {return nullptr;} // проверка на нулевой указатель
  if(*user != *restored_user) {
    std::cout << "Неверные учётные данные. " << std::endl;
    return nullptr;}
    return restored_user;
  }

// регистрация пользователя в базёнке для терминала
no_errors Chats::userRegistration() {
  std::string login, pass;
  std::cout << "Введите ваш логин" << std::endl;
  std::cin >> login;
  std::cout << "Введите ваш пароль " << std::endl;
  std::cin >> pass;
  // пусть нуль будет id для временных пользователей
  User_t user(std::make_shared<User>(0, login, pass));
  if (bool(db_queries_dml.select_User_By_Login_fc(user->getLogin()))) {
      std::cout << "Такой пользователь уже существует. " << std::endl;
      return false;
  }
  if (db_queries_dml.insert_User_fc(user)) {
      std::cout << "Регистрация прошла успешно. " << std::endl;
      return true;
  }
  return false;
}

// регистрация пользователя в базёнке для сетевого интерфейса
User_t Chats::userRegistration(const User_t user) {
  if (!user) return nullptr; // проверка на нулевой указатель

  // ищем пользователя с заданным логином
  User_t restored_user{db_queries_dml.select_User_By_Login_fc(user->getLogin())};
  if (bool(restored_user)) {
    std::cout << "Такой пользователь уже есть. " << std::endl;
    return nullptr;} // пользователь с  таким-же логином уже есть

  // вносим нового пользователя в базёнку и находим его по id
  return db_queries_dml.select_User_By_Id_fc(db_queries_dml.insert_User_fc(user));
}

// Написать сообщение для терминала
void Chats::write() {
  // Выводим имя активного пользователя
  if (!currentUserPtr) {
    std::cout << "Вы не зарегистрированы! " << std::endl;
    return;
  }
  std::cout << "Активный пользователь: ";
  currentUserPtr->printUser();
  std::cout << std::endl;
  printAllUsers();
  std::cout << std::endl;

  std::string id_input;
  std::cout << "Выберите пользователя для чата (введите номер) " << std::endl;
  std::cin >> id_input;
  User_t companion;

  // проверяем корректность ввода
  try {
    companion = db_queries_dml.select_User_By_Id_fc(std::stol(id_input));
  } catch (const std::exception &e) {
    cout << endl << e.what() << endl;
  }
  #if _DEBUG
  std::cout << "Компаньон: ";
  companion->printUser();
  std::cout << std::endl;
  #endif

  // выбираем чат по именам пользователей
  currentChatPtr = getChatByUsers(currentUserPtr, companion);

  // Посмотри ниже внимательно!!! Так делать нельзя!
  // хотя строки компилируются условно -
  // разыменование указателя приводит к ошибке!!!

  // #if _DEBUG
  // std::cout << "Aктивный чат: ";
  // std::cout << "активный чат: " << currentChatPtr->getId() << " : " << currentChatPtr->getChatName();
  // std::cout << std::endl;
  // #endif

  // если чата нет - создаём его
  if (!(currentChatPtr)) {
    //  создаём чат - добавляем его по имени и тут же считывааем по id.
    currentChatPtr = createChatByUsers(currentUserPtr, companion);
    // currentChatPtr =
    //     db_queries_dml.select_Chat_By_Id_fc(db_queries_dml.insert_Chat_fc(
    //         getChatNameByUsers(currentUserPtr, companion)));
  }
  //printAllMessages(currentChatPtr); // может для диагностики
  sendMessage(currentChatPtr, currentUserPtr, acquaireMessage());
}

void Chats::logoff() {
  if (currentUserPtr) {
    std::cout << "Вы вышли из учётной записи - ";
    currentUserPtr->printUser();
    std::cout << std::endl;
    currentUserPtr = nullptr;
    currentChatPtr = nullptr;
  } else {
    std::cout << "Прежде чем выйти из учётной записи необходимо в неё войти. "
                 "Вы не вошли в учётую запись."
              << std::endl;
  }
}

void Chats::exit() { Q = true; }

void Chats::restordata() {
  SaveRestor restor;
  restor.createPath("/tmp", "/tmp/Chat_Yevgeniy");
  restor.createPath("/tmp", "/tmp/Chat_Yevgeniy/Chats");
  restor.restorUsers(users);
  restor.restorChats(chats);
}

void Chats::savedata() {
  SaveRestor save;
  save.createPath("/tmp", "/tmp/Chat_Yevgeniy");
  save.saveUsers(users);
  save.saveChats(chats);
}

void Chats::localCycle() {
  while (!Q)  // цикл
  {
    // если есть текущий пользователь - то распечатываем его
    if (currentUserPtr) {
      std::cout << "Активный пользователь: ";
      currentUserPtr->printUser();
    }

    if (currentChatPtr) {
      std::cout << "Активный чат: ";
      currentChatPtr->printChatName();
    }

    std::cout << "Введите комманду (0 - помощ)" << std::endl;
    std::cin >> cmd_input;

    // проверяем корректность ввода
    try {
      cmd = std::stoi(cmd_input);
    } catch (exception &except) {
      cout << endl << except.what() << endl;
      cmd = 0;
    }

    try  // обработка исключений
    {
      switch (cmd) {
        case 0:
          std::cout << "0 - помощь" << std::endl;
          std::cout << "1 - вывести данные текущего пользователя" << std::endl;
          std::cout << "2 - авторизоваться" << std::endl;
          std::cout << "3 - написать сообщение" << std::endl;
          std::cout << "4 - регистрация пользователя" << std::endl;
          std::cout << "5 - распечатать все сообщения чата" << std::endl;
          std::cout << "8 - выйти из учётной записи" << std::endl;
          std::cout << "9 - выйти из программы" << std::endl;
          std::cout << "Имя пользователя должно состоять из одного слова"
                    << std::endl;
          break;

        case 1:  // выводим  данные текущего пользователя
          userinfo();
          break;
        case 2:  // User logon
          logon();
          break;
        case 3:  // Написать пользователю
          write();
          break;
        case 4:  // User registration
          userRegistration();
          break;
        case 5:
          printAllMessages(currentChatPtr);
          break;
        case 8:  // logoff
          logoff();
          break;
        case 9:  // Выход из программы
          exit();
          break;
        default:
          std::cout << "Нет такой команды: " << cmd << std::endl;
          break;
      }
    } catch (exception &except) {
      cout << endl << except.what() << endl;
    }
  }
}

void Chats::remoteCycle() {
  // тут добавил новые объявления/////////////////////////////
  Server server;
  TCP_server tcp_server;
  std::string string_to_send{"Привет от сервера!"};
  std::string string_for_receive;
  User_t user;
  User_t companion;
  Chat_t chat;
  queue_string_t usernames;
  queue_message_t lastMessages;
  // тут добавил новые объявления конец/////////////////////////////

  tcp_server.configureConnection();
  tcp_server.listening();
  tcp_server.openConnection();

  bool firstcycle{true};

  while (true) {
    if (!firstcycle) {
      tcp_server.send(string_to_send);
    }
    firstcycle = false;
    tcp_server.receive(string_for_receive);

    ReceivedData receivedData(server.interpretString(string_for_receive));
    switch (receivedData._type) {
      case REGISTRATION:
        user = userRegistration(
            server.retrieveUser(std::string(receivedData._str_view)));
        if (!user) {
          string_to_send = std::string("Ошибка регистрации");
          break;
        }
        string_to_send =
            std::string("Активный пользователь: ") + user->getLogin();
        break;

      case LOGON:
        user = logon(server.retrieveUser(std::string(receivedData._str_view)));
        if (!user) {
          string_to_send = std::string("Ошибка входа в учётную запись");
          break;
        }
        string_to_send = "Активный пользователь: " + user->getLogin();
        break;
      case COMPANION:
        if (!user) {
          string_to_send = "Вы не вошли в учётную запись";
          break;
        }
        companion = getUserByLogin(std::string(receivedData._str_view));
        if (!companion) {
          string_to_send = "Пользователь для переписки не найден";
          break;
        }
        chat = getChatByUsers(user, companion);
        if (!chat) {
          string_to_send = "чат не существует";
          break;
        };

        string_to_send = "Активный чат: " + chat->getChatName() +
                         " с пользователем: " + companion->getLogin();
        break;

      case MESSAGE:
        if (!user) {
          string_to_send = "Вы не вошли в учётную запись";
          break;
        };
        if (!chat) {
          string_to_send = "чат не существует";
          break;
        };
        {
          Message_t msg(
              server.retrieveMessage(std::string(receivedData._str_view)));
          if (!msg) {
            string_to_send = "Сообщение не доставлено или повреждено";
          }
          addMessage(chat, msg);
          string_to_send = "принято";
        }
        break;

      case GET_USERNAMES:
        std::cout << "getusernames step"
                  << std::endl;  // диагностическая информация на сервер
        usernames = getUserNames();
      case CONTINUE_USERNAMES:
        std::cout << "continueusernames step"
                  << std::endl;  // диагностическая информация на сервер
        if (!usernames->empty()) {
          string_to_send = server.getUsernamesString(usernames->front());
          usernames->pop();
        } else
          string_to_send = server.getUsernamesEnd();
        break;

      case GET_MESSAGES:
        std::cout << "get messages step"
                  << std::endl;  // диагностическая информация на сервер

        if (!chat) {
          string_to_send = "чат не существует";
          break;
        };
        //lastMessages = chat->getLastMessages();
        lastMessages = getUnreadMessages(chat);

        std::cout << "last messages from queue step get_messages start "
                  << std::endl;  // диагностическая информация на сервер
//!!! тут проверить указатель
        // while (!lastMessages->empty()) {
        //   std::cout << std::endl;
        //   lastMessages->front().printMessage();
        //   std::cout << std::endl;
        //   lastMessages->pop();
        // }
        std::cout << "last messages from queue step get_messages end "
                  << std::endl;  // диагностическая информация на сервер

      case CONTINUE_MESSAGES:
        std::cout << "continuemessages step"
                  << std::endl;  // диагностическая информация на сервер
        if (!lastMessages) {
          std::cout << "something wrong, try to read nothing "
                    << std::endl;  // диагностическая информация на сервер
          break;
        };
        if (!lastMessages->empty()) {
          {
            Message msg{lastMessages->front()}; // аккуратней, тут всё зависит только от конструктора!
            string_to_send = server.getMessageString(msg);

            db_queries_dml.update_Status_Delivered_fc(msg.getId());
            std::cout << "lastMessages->front().getId()" << msg.getId()
                      << std::endl;
          }
            lastMessages->pop();

          std::cout << "one message pop "
                    << std::endl;  // диагностическая информация на сервер
        } else {
          string_to_send = server.getMessagesEnd();
        }
        break;

      case LOGOUT:
        string_to_send = "Logout for respond";
        user = nullptr;
        companion = nullptr;
        chat = nullptr;
        if (!lastMessages) {
          break;
        };
        while (!lastMessages->empty()) lastMessages->pop();
        break;

      case EXIT:
        string_to_send = "Exit for respond";
        return;
        break;

      case NOTHING:
        string_to_send = string_for_receive;
        break;  // диагностическая информация на сервер
    }
  }
}

// void Chats::addMessage(const Chat_t chat, const std::shared_ptr<Message> msg) {
//   if (chat && msg) {
//     chat->addMessage(std::move(*msg));
//   }
// }



