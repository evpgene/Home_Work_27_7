.PHONY: all clean install uninstall

LINK = sudo g++ -std=c++20 -D_DEBUG -o
COMPILE = sudo g++ -std=c++20 -D_DEBUG -c -o
FLAGS = `mysql_config --cflags --libs`

all: chat_server

SaveRestor.o: SaveRestor.h SaveRestor.cpp
	$(COMPILE) SaveRestor.o SaveRestor.cpp $(FLAGS)

User.o: User.h User.cpp
	$(COMPILE) User.o User.cpp $(FLAGS)

Message.o: Message.h Message.cpp
	$(COMPILE) Message.o Message.cpp $(FLAGS)

Chat.o: Chat.h Chat.cpp
	$(COMPILE) Chat.o Chat.cpp $(FLAGS)

Chats.o: Chats.h Chats_functions.cpp Chats.cpp
	$(COMPILE) Chats.o Chats.cpp $(FLAGS)

TCP_server.o: TCP_server.h TCP_server.cpp
	$(COMPILE) TCP_server.o TCP_server.cpp $(FLAGS)

Server.o: Server.h Server.cpp ReceivedType.h ReceivedData.h
	$(COMPILE) Server.o Server.cpp $(FLAGS)

DB_Queries_DDL.o: DB_Queries_DDL.h DB_Queries_DDL.cpp
	$(COMPILE) DB_Queries_DDL.o DB_Queries_DDL.cpp $(FLAGS)

DB_Queries_DML.o: DB_Queries_DML.h DB_Queries_DML_structures.h DB_Queries_DML_utilities.cpp DB_Queries_DML.cpp
	$(COMPILE) DB_Queries_DML.o DB_Queries_DML.cpp $(FLAGS)

Main.o: Main.cpp
	$(COMPILE) Main.o Main.cpp $(FLAGS)

chat_server: Main.o DB_Queries_DML.o DB_Queries_DDL.o Server.o TCP_server.o Chats.o Chat.o Message.o User.o SaveRestor.o
	$(LINK) chat_server Main.o DB_Queries_DML.o DB_Queries_DDL.o Server.o TCP_server.o Chats.o Chat.o Message.o User.o SaveRestor.o $(FLAGS)





