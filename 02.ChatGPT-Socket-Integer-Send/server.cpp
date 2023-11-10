//
// Created by Defiant on 10/11/2023.
//

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Bind the socket to a specific port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345); // You can use any available port

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is waiting for a connection...\n";

    // Accept a connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection\n";
        close(serverSocket);
        return -1;
    }

    // Receive the start message
    char startMessage[6];
    if (recv(clientSocket, startMessage, sizeof(startMessage), 0) == -1) {
        std::cerr << "Error receiving start message\n";
        close(clientSocket);
        close(serverSocket);
        return -1;
    }

    if (std::string(startMessage) != "10101") {
        std::cerr << "Invalid start message\n";
        close(clientSocket);
        close(serverSocket);
        return -1;
    }

    std::cout << "Communication started!\n";

    while (true) {
        // Receive integer from the client
        int receivedValue;
        if (recv(clientSocket, &receivedValue, sizeof(receivedValue), 0) == -1) {
            std::cerr << "Error receiving data\n";
            break;
        }

        std::cout << "Received value: " << receivedValue << std::endl;

        // Send integer to the client
        int valueToSend;
        std::cout << "Enter an integer to send: ";
        std::cin >> valueToSend;

        if (send(clientSocket, &valueToSend, sizeof(valueToSend), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
