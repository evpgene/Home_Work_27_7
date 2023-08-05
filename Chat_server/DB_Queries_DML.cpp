
#include "DB_Queries_DML_utilities.cpp"


DB_Queries_DML::DB_Queries_DML() {}

DB_Queries_DML::~DB_Queries_DML() {}

no_errors DB_Queries_DML::connectDB_open(const MYSQL_Config& config) {
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

no_errors DB_Queries_DML::connectDB_close(void) {
  // Закрываем соединение с сервером базы данных
  mysql_close(mysql);
  return true;
}

// V
insert_id_t DB_Queries_DML::insert_User_fc(const User_t user) {
  Insert_User& arg_struct = Insert_User_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  if (!user) return 0;

  /* Prepare data for execution */
  strncpy(query.login.data, user->getLogin().c_str(), string_size);
  query.login.length = strlen(query.login.data);

  strncpy(query.pass.data, user->getPass().c_str(), string_size);
  query.pass.length = strlen(query.pass.data);

  /* Execute statement */
  no_errors &= execute<Insert_User&>(arg_struct);

  /* Verify affected rows */
  no_errors &= verify_affected_rows(arg_struct.stmt, 1, arg_struct.headline);

  if (!no_errors) return 0;
  return mysql_stmt_insert_id(arg_struct.stmt);
}
// extracts the user with certain id. If some errors or not such id exists -
// returning nullptr
User_t DB_Queries_DML::select_User_By_Id_fc(const size_t id) {
  Select_User_By_Id& arg_struct = Select_User_By_Id_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  // Prepare data for execution
  query.id.data = id;

  /* Execute statement */
  no_errors &= execute<Select_User_By_Id&>(arg_struct);


  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) return nullptr;
  return std::make_shared<User>(
      User{result.id.data, result.login.data, result.pass.data});
}

// Returns a pointer to the found user. If the user is not found - returns a
// nullpointer
User_t DB_Queries_DML::select_User_By_Login_fc(const std::string& login) {
  Select_User_By_Login& arg_struct = Select_User_By_Login_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  strncpy(query.login.data, login.c_str(), string_size);
  query.login.length = strlen(query.login.data);

  /* Execute statement */
  no_errors &= execute<Select_User_By_Login&>(arg_struct);


  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) return nullptr;
  return make_shared<User>(User{result.id.data, result.login.data, result.pass.data});
}

queue_user_t DB_Queries_DML::select_Users_All_fc(void) {
  Select_Users_All& arg_struct = Select_Users_All_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.dummy.data = (unsigned int)(1);

  /* Execute statement */
  no_errors &= execute<Select_Users_All&>(arg_struct);


  // Prepare result storage
  queue_user_t queue(std::make_shared<std::queue<User>>(std::queue<User>()));

  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;

    queue->emplace(User(result.id.data, result.login.data, result.pass.data));
  }
  no_errors &= (status != 1);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

#if _DEBUG
  std::cout << "queue size: " << queue->size() << std::endl;
#endif
  if (!no_errors) return nullptr;
  return queue;
}

// V
insert_id_t DB_Queries_DML::insert_Chat_fc(const std::string chatname) {
  Insert_Chat& arg_struct = Insert_Chat_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* check source data */
  if (chatname.empty()) return 0;

  /* Prepare data for execution */
  strncpy(query.name.data, chatname.c_str(), string_size);
  query.name.length = strlen(query.name.data);

  /* Execute statement */
  no_errors &= execute<Insert_Chat&>(arg_struct);


  /* Verify affected rows */
  no_errors &= verify_affected_rows(arg_struct.stmt, 1, arg_struct.headline);

  if (!no_errors) return 0;
  return mysql_stmt_insert_id(arg_struct.stmt);
}

// V
Chat_t DB_Queries_DML::select_Chat_By_Id_fc(const size_t id) {
  Select_Chat_By_Id& arg_struct = Select_Chat_By_Id_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  // Prepare data for execution
  query.id.data = id;

  /* Execute statement */
  no_errors &= execute<Select_Chat_By_Id&>(arg_struct);


  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) return nullptr;
  return std::make_shared<Chat>(Chat{result.id.data, result.name.data});
}
// V
Chat_t DB_Queries_DML::select_Chat_By_Name_fc(const std::string& name) {
  Select_Chat_By_Name& arg_struct = Select_Chat_By_Name_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  strncpy(query.name.data, name.c_str(), string_size);
  query.name.length = strlen(query.name.data);

  /* Execute statement */
  no_errors &= execute<Select_Chat_By_Name&>(arg_struct);


  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) {
   
    #if _DEBUG
      std::cout <<  arg_struct.headline << "returned nullptr ";
      std::cout << std::endl;
    #endif
    return nullptr;
    }
  return make_shared<Chat>(Chat{result.id.data, result.name.data});
  ;
}

// V
insert_id_t DB_Queries_DML::insert_Chat_User_fc(const size_t chat_id,
                                              const size_t user_id,
                                              const size_t user_no) {
  Insert_Chat_User& arg_struct = Insert_Chat_User_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_id.data = chat_id;
  query.user_id.data = user_id;
  query.user_no.data = user_no;

  /* Execute statement */
  no_errors &= execute<Insert_Chat_User&>(arg_struct);


  /* Verify affected rows */
  no_errors &= verify_affected_rows(arg_struct.stmt, 1, arg_struct.headline);

  // Construct return
  if (no_errors) return mysql_stmt_insert_id(arg_struct.stmt);
  return 0;
}

size_t DB_Queries_DML::select_Chat_User_fc(const size_t chat_id,
                                           const size_t user_id) {
  Select_Chat_User& arg_struct = Select_Chat_User_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_id.data = chat_id;
  query.user_id.data = user_id;

  /* Execute statement */
  no_errors &= execute<Select_Chat_User&>(arg_struct);

#if _DEBUG
  std::cout << "Returned after execution. ";
  std::cout << std::endl;
#endif

  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) return 0;
  return result.chat_user_id.data;
}

// V
insert_id_t DB_Queries_DML::insert_Message_fc(const size_t chat_user_id,
                                            std::string message) {
  Insert_Message& arg_struct = Insert_Message_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_user_id.data = chat_user_id;
  strncpy(query.message.data, message.c_str(), string_size);
  query.message.length = strlen(query.message.data);

  /* Execute statement */
  no_errors &= execute<Insert_Message>(arg_struct);


  /* Verify affected rows */
  no_errors &= verify_affected_rows(arg_struct.stmt, 1, arg_struct.headline);

  if (!no_errors) return 0;
  return mysql_stmt_insert_id(arg_struct.stmt);
}

Message_t DB_Queries_DML::select_Message_fc(const size_t chat_id,
                                            const size_t message_id) {
  Select_Message& arg_struct = Select_Message_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  auto& timesend = arg_struct.Result_struct.timesend.data;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_id.data = chat_id;
  query.message_id.data = message_id;

  /* Execute statement */
  no_errors &= execute<Select_Message&>(arg_struct);


  // Fetch all rows
  result.row_count = 0;
#if _DEBUG
  std::cout << arg_struct.headline << "Fetching results ... " << std::endl;
#endif
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;
  }
  no_errors &= (status != 1);

  // Check row count
  no_errors &= verify_fetched_rows(result.row_count, 1, arg_struct.headline);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  std::string date_string =
      (std::to_string(timesend.year) + '-' + std::to_string(timesend.month) +
       '-' + std::to_string(timesend.day) + " " +
       std::to_string(timesend.hour) + ':' + std::to_string(timesend.minute) +
       ':' + std::to_string(timesend.second));

  if (!no_errors) return 0;
  return make_shared<Message>(
      Message(date_string, result.login.data, result.message.data));
}

queue_message_t DB_Queries_DML::select_Messages_Mult_fc(
    const size_t chat_id, const size_t message_status) {
  Select_Messages_Mult& arg_struct = Select_Messages_Mult_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  auto& timesend = arg_struct.Result_struct.timesend.data;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_id.data = chat_id;
  query.status.data = message_status;

  /* Execute statement */
  no_errors &= execute<Select_Messages_Mult&>(arg_struct);


  // Prepare result storage
  queue_message_t queue =
      std::make_shared<std::queue<Message>>(std::queue<Message>());

  // Fetch all rows
  result.row_count = 0;
  std::cout << arg_struct.headline << " Fetching results ... " << std::endl;
  int status;
  while (true) {
    status = mysql_stmt_fetch(arg_struct.stmt);
    if (status == 1 || status == MYSQL_NO_DATA) break;
    result.row_count++;

   // std::string date_string = "и тут пусто";
    std::string date_string = (std::to_string(timesend.year) + '-'+ std::to_string(timesend.month) + '-' +
         std::to_string(timesend.day) + " " + std::to_string(timesend.hour) + ':' + std::to_string(timesend.minute) +
         ':' + std::to_string(timesend.second));

         

    queue->emplace<Message>(Message( result.message_id.data, date_string,
        result.user_login.data, result.message.data ));
  }
  no_errors &= (status != 1);

  // Free result
  no_errors &= free_result_stmt(arg_struct.stmt, arg_struct.headline);

  if (!no_errors) return nullptr;
  return queue;
}

affected_rows_t DB_Queries_DML::update_Status_Delivered_fc(
    const size_t message_id) {
  Update_Status_Delivered& arg_struct = Update_Status_Delivered_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.id.data = message_id;

  /* Execute statement */
  no_errors &= execute<Update_Status_Delivered&>(arg_struct);


  if (!no_errors) return 0;
  return affected_rows(arg_struct.stmt, arg_struct.headline);
}

affected_rows_t DB_Queries_DML::update_Status_Read_fc(
    const size_t chat_user_id, const size_t message_id_begin,
    const size_t message_id_end) {
  Update_Status_Read& arg_struct = Update_Status_Read_struct;
  auto& query = arg_struct.Query_struct;
  auto& result = arg_struct.Result_struct;
  no_errors no_errors{true};

  /* Prepare data for execution */
  query.chat_user_id.data = chat_user_id;
  query.message_id_begin.data = message_id_begin;
  query.message_id_end.data = message_id_end;

  /* Execute statement */
  no_errors &= execute<Update_Status_Read&>(arg_struct);

  if (!no_errors) return 0;
  return affected_rows(arg_struct.stmt, arg_struct.headline);
}

no_errors DB_Queries_DML::prepareAll(void) {
  no_errors no_errors{true};
  no_errors &= prepare<Insert_User>(Insert_User_struct, mysql);
  no_errors &= prepare<Select_User_By_Id>(Select_User_By_Id_struct, mysql);
  no_errors &=
      prepare<Select_User_By_Login>(Select_User_By_Login_struct, mysql);
  no_errors &= prepare<Select_Users_All>(Select_Users_All_struct, mysql);
  no_errors &= prepare<Insert_Chat>(Insert_Chat_struct, mysql);
  no_errors &= prepare<Select_Chat_By_Name>(Select_Chat_By_Name_struct, mysql);
  no_errors &= prepare<Select_Chat_By_Id>(Select_Chat_By_Id_struct, mysql);
  no_errors &= prepare<Insert_Chat_User>(Insert_Chat_User_struct, mysql);
  no_errors &= prepare<Select_Chat_User>(Select_Chat_User_struct, mysql);
  no_errors &= prepare<Insert_Message>(Insert_Message_struct, mysql);
  no_errors &= prepare<Select_Message>(Select_Message_struct, mysql);
  no_errors &=
      prepare<Select_Messages_Mult>(Select_Messages_Mult_struct, mysql);
  no_errors &=
      prepare<Update_Status_Delivered>(Update_Status_Delivered_struct, mysql);
  no_errors &= prepare<Update_Status_Read>(Update_Status_Read_struct, mysql);

  return no_errors;
}

no_errors DB_Queries_DML::closeAll(void) {
  no_errors no_errors{true};
  no_errors &= close<Insert_User>(Insert_User_struct, mysql);
  no_errors &= close<Select_User_By_Id>(Select_User_By_Id_struct, mysql);
  no_errors &= close<Select_User_By_Login>(Select_User_By_Login_struct, mysql);
  no_errors &= close<Select_Users_All>(Select_Users_All_struct, mysql);
  no_errors &= close<Insert_Chat>(Insert_Chat_struct, mysql);
  no_errors &= close<Select_Chat_By_Name>(Select_Chat_By_Name_struct, mysql);
  no_errors &= close<Select_Chat_By_Id>(Select_Chat_By_Id_struct, mysql);
  no_errors &= close<Insert_Chat_User>(Insert_Chat_User_struct, mysql);
  no_errors &= close<Select_Chat_User>(Select_Chat_User_struct, mysql);
  no_errors &= close<Insert_Message>(Insert_Message_struct, mysql);
  no_errors &= close<Select_Message>(Select_Message_struct, mysql);
  no_errors &= close<Select_Messages_Mult>(Select_Messages_Mult_struct, mysql);
  no_errors &=
      close<Update_Status_Delivered>(Update_Status_Delivered_struct, mysql);
  no_errors &= close<Update_Status_Read>(Update_Status_Read_struct, mysql);

  no_errors &= connectDB_close();
  return no_errors;
}
