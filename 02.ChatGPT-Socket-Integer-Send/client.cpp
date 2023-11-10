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
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("server_ip_address"); // Replace with the IP address of your server
    serverAddress.sin_port = htons(12345); // Same port used in the server

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    // Send the start message
    const char startMessage[] = "10101";
    if (send(clientSocket, startMessage, sizeof(startMessage), 0) == -1) {
        std::cerr << "Error sending start message\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Communication started!\n";

    while (true) {
        // Send integer to the server
        int valueToSend;
        std::cout << "Enter an integer to send: ";
        std::cin >> valueToSend;

        if (send(clientSocket, &valueToSend, sizeof(valueToSend), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }

        // Receive integer from the server
        int receivedValue;
        if (recv(clientSocket, &receivedValue, sizeof(receivedValue), 0) == -1) {
            std::cerr << "Error receiving data\n";
            break;
        }

        std::cout << "Received value: " << receivedValue << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
