#pragma once
#include <mysql/mysql.h>
#include <iostream>
#include "DB_Queries_MYSQL_Config.h"


class DB_Queries_DDL {
 private:
  MYSQL* mysql;
 public:
  DB_Queries_DDL();
  ~DB_Queries_DDL();
  unsigned int connectToMySQLserver_open(const MYSQL_Config& config);
  unsigned int createDataBase(void);
  void connectToMySQLserver_close(void);

};

#define DROP_DATABASE \
"DROP DATABASE IF EXISTS chat_db"

#define CREATE_DATABASE \
"CREATE DATABASE IF NOT EXISTS chat_db"

// #define CREATE_USER \
// "CREATE USER IF NOT EXISTS 'chat_db1_user'@'localhost' IDENTIFIED BY 'Root123_Root123'"

// #define GRANT_USER \
// "GRANT ALL ON chat_db1.* TO 'chat_db1_user'@'localhost'"

#define CREATE_TABLE_USERS \
"CREATE TABLE IF NOT EXISTS users (\
		id INT UNSIGNED NOT NULL AUTO_INCREMENT,\
		login character varying(255) NOT NULL UNIQUE,\
		pass character varying(255) NOT NULL,\
		firstname character varying(255),\
		lastname character varying(255),\
		email character varying(255),\
		PRIMARY KEY (id)\
	)AUTO_INCREMENT = 1;"
// create table "chats" request string
#define CREATE_TABLE_CHATS \
"CREATE TABLE IF NOT EXISTS chats (\
		id INT UNSIGNED NOT NULL AUTO_INCREMENT,\
		name character varying(255) NOT NULL UNIQUE,\
		PRIMARY KEY (id)\
	)AUTO_INCREMENT = 1"
// create table "chat_user" request string
#define CREATE_TABLE_CHAT_USER \
"CREATE TABLE IF NOT EXISTS \
	chat_user (\
		id INT UNSIGNED NOT NULL AUTO_INCREMENT,\
		chat_id INT UNSIGNED NOT NULL REFERENCES chats(id),\
		user_id INT UNSIGNED NOT NULL REFERENCES users(id),\
		user_no INT UNSIGNED NOT NULL CHECK (user_no = 1 OR user_no = 2),\
		PRIMARY KEY (id),\
		UNIQUE (chat_id, user_no),\
		CONSTRAINT chat_id_fk FOREIGN KEY(chat_id) REFERENCES chats(id),\
		CONSTRAINT user_id_fk FOREIGN KEY(user_id) REFERENCES users(id)\
	)AUTO_INCREMENT = 1"
// create table "messages" request string
#define CREATE_TABLE_MESSAGES \
"CREATE TABLE IF NOT EXISTS \
	messages (\
		id INT UNSIGNED NOT NULL AUTO_INCREMENT,\
		chat_user_id INT UNSIGNED NOT NULL REFERENCES chat_user(id),\
		message character varying(255) NOT NULL,\
		dt DATETIME NOT NULL,\
		status INT UNSIGNED DEFAULT 1 CHECK (status BETWEEN 1 AND 3),\
		dt_delivered DATETIME,\
		dt_read DATETIME,\
		PRIMARY KEY (id),\
		CONSTRAINT chat_user_id_fk FOREIGN KEY(chat_user_id) REFERENCES chat_user(id) \
	)AUTO_INCREMENT = 1"
// create view "message_view" request string
#define CREATE_VIEW_MESSAGE_VIEW \
"CREATE OR REPLACE VIEW \
	message_view AS \
	SELECT \
	chats.id AS chat_id,\
	users.id AS user_id,\
	messages.id AS message_id,\
	messages.dt AS timesend,\
	users.login AS user_login,\
	messages.message AS message,\
	messages.status AS status\
	FROM messages \
	JOIN chat_user ON chat_user.id = messages.chat_user_id \
	JOIN chats ON chats.id = chat_user.chat_id \
	JOIN users ON users.id = chat_user.user_id"
