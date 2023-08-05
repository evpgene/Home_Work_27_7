#pragma once
#include <mysql/mysql.h>

#include <string>

const int string_size{255};

struct ParamUint {
  unsigned long int data; // zdec' long blin!!! ne unsigned!!! bliiiinnnn!!! long a ne int! 3 dnya prosrano!
  unsigned long length;  // Current buffer length for output or input
                          // data (for character and binary C data)
  bool is_null{false};  // For input/output set inform that the data is NULL
  bool is_unsigned{true};   // Inform binding to signed or unsigned type - mean
                          // only for client side
  bool error{true};    // For output (received), set indicate Truncation
  ParamUint() = delete;
  ParamUint(MYSQL_BIND& bind) {
    bind.buffer_type = MYSQL_TYPE_LONG ;
    bind.buffer_length = sizeof(data);
    bind.is_unsigned = &is_unsigned;  // Inform binding to signed or unsigned type -
                              // mean only for client side
    bind.buffer = (char*)&data;
    bind.length = &length;
    bind.is_null = &is_null;
    bind.error = &error;
  };
  ~ParamUint(){};
};

struct ParamString {
  char data[string_size];
  unsigned long length;  // Current buffer length for output or input
                            // data (for character and binary C data)
  bool is_null{false};      // For input/output set inform that the data is NULL
  bool is_unsigned{true};   // Inform binding to signed or unsigned type - mean
                            // only for client side
  bool error{true};         // For output (received), set indicate Truncation
  ParamString() = delete;
  ParamString(MYSQL_BIND& bind) {
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.is_unsigned = false;  // Inform binding to signed or unsigned type -
                               // mean only for client side
    bind.buffer_length =
        string_size;  // Total buffer length (for character and binary C data)
    bind.buffer = (char*)&data;
    bind.length = &length;
    bind.is_null = &is_null;
    bind.error = &error;
  };
  ~ParamString(){};
};

struct ParamDatetime {
  MYSQL_TIME data;
  unsigned long length;  // Current buffer length for output or input
  //                           // data (for character and binary C data)
  bool is_null{false};  // For input/output set inform that the data is NULL
  bool is_unsigned{false};   // Inform binding to signed or unsigned type - mean
                          // only for client side
  bool error{false};     // For output (received), set indicate Truncation
  ParamDatetime() = delete;
  ParamDatetime(MYSQL_BIND& bind) {
    bind.buffer_type = MYSQL_TYPE_DATETIME; // zdec' MYSQL_TYPE_TIMESTAMP blin!!! bliiiinnnn!!! long a ne int! nedelya prosrana!
    bind.buffer_length = sizeof(data);
    bind.is_unsigned = &is_unsigned;  // Inform binding to signed or unsigned type -
                          // mean only for client side
    bind.buffer = (char*)&data;
    bind.length = &length;
    bind.is_null = &is_null;
    bind.error = &error;
  };
  ~ParamDatetime(){};
};

// Result structure for statment without result (for example INSERT)
struct NoResult {
  const int param_count{0};
  MYSQL_BIND* bind;  // Emty pointer
};

// V
struct Insert_User {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"insert_user_query "};
  const std::string query{"INSERT INTO users (login, pass) VALUES (?,?)"};
  struct query {
    const int param_count{2};
    MYSQL_BIND bind[2];                        // Two parameters
    ParamString login = ParamString(bind[0]);  // First parameter
    ParamString pass = ParamString(bind[1]);   // Second parameter
  } Query_struct;
  NoResult Result_struct;
};

// V
struct Select_User_By_Id {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_user_by_id_query "};
  const std::string query{"SELECT id, login, pass FROM users WHERE id = ?"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];
    ParamUint id = ParamUint(bind[0]);
  } Query_struct;
  struct result {
    const int param_count{3};  // Number of result filds expected
    MYSQL_BIND bind[3];
    ParamUint id = ParamUint(bind[0]);         // First parameter
    ParamString login = ParamString(bind[1]);  // Second parameter
    ParamString pass = ParamString(bind[2]);   // Third parameter
    int column_count;
    int row_count;
  } Result_struct;
};

// V
struct Select_User_By_Login {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_user_by_login_query "};
  const std::string query{"SELECT id, login, pass FROM users WHERE login = ?"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];
    ParamString login = ParamString(bind[0]);  // First parameter
  } Query_struct;
  struct result {
    const int param_count{3};  // Number of result filds expected
    MYSQL_BIND bind[3];
    ParamUint id = ParamUint(bind[0]);         // First parameter
    ParamString login = ParamString(bind[1]);  // Second parameter
    ParamString pass = ParamString(bind[2]);   // Third parameter
    int column_count;
    int row_count;
  } Result_struct;
};


struct Select_Users_All {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_users_all_query "};
  const std::string query{"SELECT id, login, pass FROM users WHERE id >= ?"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];
    ParamUint dummy = ParamUint(bind[0]);
  } Query_struct;
  struct result {
    const int param_count{3};  // Number of result filds expected
    MYSQL_BIND bind[3];
    ParamUint id = ParamUint(bind[0]);         // First parameter
    ParamString login = ParamString(bind[1]);  // Second parameter
    ParamString pass = ParamString(bind[2]);   // Third parameter
    int column_count;
    int row_count;
  } Result_struct;
};

// V
struct Insert_Chat {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"insert_chat_query "};
  const std::string query{"INSERT INTO chats (name) VALUES (?)"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];        // One parameter
    ParamString name = ParamString(bind[0]);  // First parameter
  } Query_struct;
  NoResult Result_struct;
};

// V
struct Select_Chat_By_Name {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_chat_by_name_query "};
  const std::string query{"SELECT id, name FROM chats WHERE name = ?"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];
    ParamString name = ParamString(bind[0]);  // First parameter
  } Query_struct;
  struct result {
    const int param_count{2};  // Number of result filds expected
    MYSQL_BIND bind[2];
    ParamUint id = ParamUint(bind[0]);        // First parameter
    ParamString name = ParamString(bind[1]);  // Second parameter
    int column_count;
    int row_count;
  } Result_struct;
};

// V
struct Select_Chat_By_Id {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_chat_by_id_query "};
  const std::string query{"SELECT id, name FROM chats WHERE id = ?"};
  struct query {
    const int param_count{1};  // Number of prepared parameters expected
    MYSQL_BIND bind[1];
    ParamUint id = ParamUint(bind[0]);  // First parameter
  } Query_struct;
  struct result {
    const int param_count{2};  // Number of result filds expected
    MYSQL_BIND bind[2];
    ParamUint id = ParamUint(bind[0]);        // First parameter
    ParamString name = ParamString(bind[1]);  // Second parameter
    int column_count;
    int row_count;
  } Result_struct;
};

// V
struct Insert_Chat_User {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"insert_chat_user_query "};
  const std::string query{
      "INSERT INTO chat_user (chat_id, user_id, user_no) VALUES (?,?,?)"};
  struct query {
    const int param_count{3};  // Number of prepared parameters expected
    MYSQL_BIND bind[3];        // One parameter
    ParamUint chat_id = ParamUint(bind[0]);  // First parameter
    ParamUint user_id = ParamUint(bind[1]);  // Second parameter
    ParamUint user_no = ParamUint(bind[2]);  // Third parameter
  } Query_struct;
  NoResult Result_struct;
};

// V
struct Select_Chat_User {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_chat_user_query "};
  const std::string query{"SELECT id FROM chat_user WHERE chat_id = ? AND user_id = ?"};
  struct query {
    const int param_count{2};  // Number of prepared parameters expected
    MYSQL_BIND bind[2];
    ParamUint chat_id = ParamUint(bind[0]);  // First parameter
    ParamUint user_id = ParamUint(bind[1]);  // Second parameter
  } Query_struct;
  struct result {
    const int param_count{1};  // Number of result filds expected
    MYSQL_BIND bind[1];
    ParamUint chat_user_id = ParamUint(bind[0]);  // First parameter
    int column_count;
    int row_count;
  } Result_struct;
};

// V
struct Insert_Message {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"insert_message_query "};
  const std::string query{
      "INSERT INTO messages (chat_user_id, message, dt) VALUES "
      "(?,?,CURRENT_TIMESTAMP())"};
  struct query {
    const int param_count{2};  // Number of prepared parameters expected
    MYSQL_BIND bind[2];        // One parameter
    ParamUint chat_user_id = ParamUint(bind[0]);  // First parameter
    ParamString message = ParamString(bind[1]);   // Second parameter
  } Query_struct;
  NoResult Result_struct;
};

// Проверить необходимость и целесообразность
#define SELECT_MESSAGE \
  "SELECT * FROM message_view WHERE chat_id = ? AND message_id = ?"

struct Select_Message {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_messages_query "};
  const std::string query{
      "SELECT * FROM message_view WHERE chat_id = ? AND message_id = ?"};
  struct query {
    const int param_count{2};  // Number of prepared parameters expected
    MYSQL_BIND bind[2];
    ParamUint chat_id = ParamUint(bind[0]);     // First parameter
    ParamUint message_id = ParamUint(bind[1]);  // Second parameter
    // ParamUint message_id_end = ParamUint(bind[2]); // Third parameter
    // ParamUint status = ParamUint(bind[3]); // 4 parameter
    // ParamUint limit = ParamUint(bind[4]); // 5 parameter
  } Query_struct;
  struct result {
    const int param_count{7};  // Number of result filds expected
    MYSQL_BIND bind[7];
    ParamUint chat_id = ParamUint(bind[0]);           // First parameter
    ParamUint user_id = ParamUint(bind[1]);           // Second parameter
    ParamUint message_id = ParamUint(bind[2]);        // Third parameter
    ParamDatetime timesend = ParamDatetime(bind[3]);  // 4 parameter
    ParamString login = ParamString(bind[4]);         // 5 parameter
    ParamString message = ParamString(bind[5]);       // 6 parameter
    ParamString status = ParamString(bind[6]);        // 7 parameter
    int column_count;
    int row_count;
  } Result_struct;
};


  //тут далее запрос который нивкакую не заработал в Select_Messages_Mult
  // const std::string query{"SELECT id, chat_user_id, dt, message, status FROM messages  WHERE chat_user_id = ? OR chat_user_id = ? AND id BETWEEN ? AND ? AND status = ? ORDER BY dt ASC LIMIT ?"};

struct Select_Messages_Mult {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"select_messages_mult_query "};
  //const std::string query{"SELECT message_id, user_login, message, timesend, status  FROM message_view WHERE chat_id = ? AND status >= ? ORDER BY timesend"};
  const std::string query{"SELECT message_id, user_login, message, timesend, status  FROM message_view WHERE chat_id = ? AND status >= ? ORDER BY timesend"};
  struct query {
    const int param_count{2};  // Number of prepared parameters expected
    MYSQL_BIND bind[2];
    ParamUint chat_id = ParamUint(bind[0]);  // First parameter
    ParamUint status = ParamUint(bind[1]);  // // Second parameter
  } Query_struct;
  struct result {
    const int param_count{5};  // Number of result filds expected
    MYSQL_BIND bind[5];
    ParamUint message_id = ParamUint(bind[0]);  // First parameter
    ParamString user_login = ParamString(bind[1]);  // Second parameter
    ParamString message = ParamString(bind[2]);  // third parameter
    ParamDatetime timesend = ParamDatetime(bind[3]);  // 4 parameter
    ParamUint status = ParamUint(bind[4]);  // 5 parameter
    int column_count;
    int row_count;
  } Result_struct;
};

struct Update_Status_Delivered {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"update_status_delivered_query "};
  const std::string query{
      "UPDATE messages SET status = 2, dt_delivered = CURRENT_TIMESTAMP() WHERE id = ? AND status != 2 AND status != 3"};
  struct query {
    const int param_count{3};  // Number of prepared parameters expected
    MYSQL_BIND bind[3];
    ParamUint id = ParamUint(bind[0]);      // First parameter
    // ParamUint message_id_begin = ParamUint(bind[1]);  // Second parameter
    // ParamUint message_id_end = ParamUint(bind[2]);    // Third parameter
  } Query_struct;
  NoResult Result_struct;
};

struct Update_Status_Read {
  MYSQL_STMT* stmt{nullptr};
  MYSQL_RES* result_metadata{nullptr};
  const std::string headline{"update_status_read_query "};
  const std::string query{
      "UPDATE messages SET status = 2, dt_delivered = CURRENT_TIMESTAMP() "
      "WHERE chat_user_id = ? AND id BETWEEN ? AND ? AND status != 2 AND "
      "status != 3"};
  struct query {
    const int param_count{3};  // Number of prepared parameters expected
    MYSQL_BIND bind[3];
    ParamUint chat_user_id = ParamUint(bind[0]);      // First parameter
    ParamUint message_id_begin = ParamUint(bind[1]);  // Second parameter
    ParamUint message_id_end = ParamUint(bind[2]);    // Third parameter
  } Query_struct;
  NoResult Result_struct;
};