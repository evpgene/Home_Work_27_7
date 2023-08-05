#pragma once
#include <mysql/mysql.h>

struct MYSQL_Config {
  char* host{(char*)("localhost")};
  char* user{(char*)("root")};
  char* passwd{(char*)("root")};
  char* db{NULL};
  int port{0};
  char* unix_socket{NULL};
  unsigned long clientflag{0};
};