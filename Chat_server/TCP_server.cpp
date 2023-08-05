#include "TCP_server.h"

TCP_server::TCP_server(/* args */)
{
}

TCP_server::~TCP_server()
{
}

void TCP_server::configureConnection()
{
    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed.!" << std::endl;
        exit(1);
    }
    //
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind_status = bind(socket_file_descriptor, (struct sockaddr *)&serveraddress,
                       sizeof(serveraddress));
    if (bind_status == -1)
    {
        std::cout << "Socket binding failed.!" << std::endl;
        exit(1);
    }
}

void TCP_server::listening()
{
    // Поставим сервер на прием данных
    connection_status = listen(socket_file_descriptor, 5);
    if (connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections.!" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Server is listening for new connection: " << std::endl;
    }
}

void TCP_server::openConnection()
{
    length = sizeof(client);
    connection = accept(socket_file_descriptor, (struct sockaddr *)&client, &length);
    if (connection == -1)
    {
        std::cout << "Server is unable to accept the data from client.!" << std::endl;
        exit(1);
    }
}

bool TCP_server::send(const std::string& str)
{
    bzero(message, MESSAGE_LENGTH);
    //std::cout << "Enter the message you want to send to the client: " << std::endl;
    //std::cin >> message;
    strncpy(message, str.c_str(), sizeof(message)/* sizeof(str) > sizeof(message) ? sizeof(message) : sizeof(str) */);
    ssize_t bytes = write(connection, message, sizeof(message));
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0)
    {
        //тут далее раскоментировать для диагностики, если потребуется
        //std::cout << "Data successfully sent to the client.!" << std::endl;
        std::cout << "sent to client message start " << std::endl;
        std::cout << message << std::endl;
        std::cout << " sent to client message end" << std::endl;
        return true;
    }
    return false;
}

bool TCP_server::receive(std::string& str)
{

    //std::string str; // потом убрать это

    bzero(message, MESSAGE_LENGTH);
    bool retval = read(connection, message, sizeof(message));
    if (strncmp("end", message, 3) == 0)
    {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
        // break;
    }
    str = std::string(message);
    //std::cout << "Data received from client: " << message << std::endl;
    std::cout << "received from client message start " << std::endl;
    std::cout << message << std::endl;
    std::cout << " received from client message end" << std::endl;
    return retval;
}

void TCP_server::closeConnection()
{
    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}