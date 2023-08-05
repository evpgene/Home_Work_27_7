
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

// using namespace std;

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных
#define PORT 7777           // Будем использовать этот номер порта

class TCP_server
{
private:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];

public:
    TCP_server(/* args */);
    ~TCP_server();
    void configureConnection();
    void openConnection();
    void closeConnection();
    bool send(const std::string& str);
    bool receive(std::string& str);
    void listening();
};

