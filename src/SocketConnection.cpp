#include <iostream>

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

void SocketConnection() {
    const int PORT = 8080;

    int sock;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "inet_pton" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "connect" << std::endl;
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    const char *message = "Hello from client!";

    send(sock, message, strlen(message), 0);
    std::cout << "Message sent: " << message << std::endl;

    read(sock, buffer, 1024);
    std::cout << "Message received: " << buffer << std::endl;

    close(sock);
//    return 0;
}

