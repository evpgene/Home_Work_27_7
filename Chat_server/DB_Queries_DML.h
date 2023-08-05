// Read the next referense for information about steps:
// https://dev.mysql.com/doc/c-api/8.0/en/c-api-prepared-statement-interface-usage.html

#pragma once
#include <mysql/mysql.h>
#include <string>
#include <cstring>
#include <iostream>
#include <utility>



#include "Chat.h"
#include "User.h"
#include "DB_Queries_DML_structures.h"
#include "DB_Queries_MYSQL_Config.h"


#define STRING_SIZE 255

using queue_message_t = std::shared_ptr<std::queue<Message>>;
using insert_id_t = size_t;  // id of the inserted element. No insertions if = 0;
using affected_rows_t = size_t;  // the number of affected rows. No affected rows if = 0;
using no_errors = bool;  // Execution OK. No errors occurred if = true;
using is_errors = bool;  // Execution Not OK. Errors occurred if = true;




class DB_Queries_DML {
 private:
  MYSQL* mysql = new MYSQL;
  // MYSQL_RES* res; // скорее нужно убрать, чем оставить
  // MYSQL_ROW row; // скорее нужно убрать, чем оставить
  std::size_t lastInsertMessageId{0};  // ??? проверить как использовать

  Insert_User Insert_User_struct;
  Select_User_By_Id Select_User_By_Id_struct;
  Select_User_By_Login Select_User_By_Login_struct;
  Select_Users_All Select_Users_All_struct;
  Insert_Chat Insert_Chat_struct;
  Select_Chat_By_Id Select_Chat_By_Id_struct;
  Select_Chat_By_Name Select_Chat_By_Name_struct;
  Insert_Chat_User Insert_Chat_User_struct;
  Select_Chat_User Select_Chat_User_struct;
  Insert_Message Insert_Message_struct;
  Select_Message Select_Message_struct;
  Select_Messages_Mult Select_Messages_Mult_struct;
  Update_Status_Delivered Update_Status_Delivered_struct;
  Update_Status_Read Update_Status_Read_struct;

 public:
  DB_Queries_DML();
  ~DB_Queries_DML();

  no_errors connectDB_open(const MYSQL_Config& config);
  no_errors connectDB_close(void);

  no_errors prepareAll(void);
  no_errors closeAll(void);

  insert_id_t insert_User_fc(const User_t user);
  User_t select_User_By_Id_fc(const size_t id);
  User_t select_User_By_Login_fc(const std::string& login);
  queue_user_t select_Users_All_fc(void);
  insert_id_t insert_Chat_fc(const std::string chatname);
  Chat_t select_Chat_By_Id_fc(const size_t id);
  Chat_t select_Chat_By_Name_fc(const std::string& name);
  insert_id_t insert_Chat_User_fc(const size_t chat_id, const size_t user_id,
                                const size_t user_no);
  size_t select_Chat_User_fc(const size_t chat_id, const size_t user_id);
  insert_id_t insert_Message_fc(const size_t chat_user_id, std::string message);
  Message_t select_Message_fc(const size_t chat_id, const size_t message_id);
  queue_message_t select_Messages_Mult_fc(const size_t chat_id,
                                          const size_t message_status);
  size_t update_Status_Delivered_fc(const size_t message_id);
  size_t update_Status_Read_fc(const size_t chat_user_id,
                               const size_t message_id_begin,
                               const size_t message_id_end);

};