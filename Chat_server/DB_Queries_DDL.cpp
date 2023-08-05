#include "DB_Queries_DDL.h"

DB_Queries_DDL::DB_Queries_DDL() {}

DB_Queries_DDL::~DB_Queries_DDL() {}

unsigned int DB_Queries_DDL::connectToMySQLserver_open(const MYSQL_Config& config) {
  unsigned int ret_val{0};
  // Получаем дескриптор соединения
  if(!(mysql = mysql_init(mysql))) {
    // Если дескриптор не получен — выводим сообщение об ошибке
    std::cout << "Error: can't create MySQL-descriptor" << std::endl;
  };

  // Подключаемся к серверу
  if (!(mysql = mysql_real_connect(mysql, config.host, config.user, config.passwd,
                          config.db, config.port, config.unix_socket,
                          config.clientflag))) {
    // Если нет возможности установить соединение с БД выводим сообщение об
    // ошибке
    std::cout << "Error: can't connect to database " << mysql_error(mysql)
              << std::endl;
    return mysql_errno(mysql);
  } else {
    // Если соединение успешно установлено выводим фразу — "Success!"
    std::cout << "Connect to database successfull" << std::endl;
  }

  mysql_set_character_set(mysql, "utf8mb4");
  // Смотрим изменилась ли кодировка на нужную, по умолчанию идёт latin1
  std::cout << "connection characterset: " << mysql_character_set_name(mysql)
            << std::endl;
  return mysql_errno(mysql);;
}

unsigned int DB_Queries_DDL::createDataBase(void) {

  unsigned int ret_val{0};
  // create database
  if(ret_val = mysql_query(mysql, CREATE_DATABASE)) {
	 std::cout << "CREATE_DATABASE error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };

  if(ret_val = mysql_select_db(mysql,"chat_db")){
	 std::cout << "mysql_select_db() error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };

  // create table "users"
  if(ret_val = mysql_query(mysql, CREATE_TABLE_USERS)) {
	 std::cout << "CREATE_TABLE_USERS error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };
  // create table "chats"
  if(ret_val = mysql_query(mysql, CREATE_TABLE_CHATS)) {
	 std::cout << "CREATE_TABLE_CHATS error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };
  // create table "chat_user"
  if(ret_val = mysql_query(mysql, CREATE_TABLE_CHAT_USER)) {
	 std::cout << "CREATE_TABLE_CHAT_USER error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };
  // create table "messages"
  if(ret_val = mysql_query(mysql, CREATE_TABLE_MESSAGES)) {
	 std::cout << "CREATE_TABLE_MESSAGES error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };
  // create view "message_view"
  if(ret_val = mysql_query(mysql, CREATE_VIEW_MESSAGE_VIEW)) {
	 std::cout << "CREATE_VIEW_MESSAGE_VIEW error: " << mysql_error(mysql) << std::endl;
	 return ret_val;
  };
  return ret_val;
}

void DB_Queries_DDL::connectToMySQLserver_close(void) {
  return mysql_close(mysql);
}
