#include <poll.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "server/Client.hpp"
#include "server/PollManager.hpp"
#include "server/Server.hpp"
#include "utils/Logger.hpp"

int main(int argc, char** argv) {
    uint16_t port = (argc > 1) ? std::atoi(argv[1]) : 9090;

    Server server(port);
    if (!server.init()) {
        Logger::error("Failed to initialize server");
        return 1;
    }

    Logger::info("Server ready");

    std::vector<Client*> clients;
    PollManager          pollManager;
    pollManager.addFd(server.getFd(), POLLIN);

    while (true) {
        if (pollManager.pollConnections(-1) < 0)
            break;

        // New connection
        if (pollManager.hasEvent(0, POLLIN)) {
            int fd = server.acceptConnection();
            if (fd >= 0) {
                clients.push_back(new Client(fd));
                pollManager.addFd(fd, POLLIN);
                Logger::info("Client connected");
            }
        }

        // Handle client data
        for (size_t i = 1; i < pollManager.size(); i++) {
            if (pollManager.hasEvent(i, POLLIN)) {
                Client* client = clients[i - 1];

                if (client->receiveData() <= 0) {
                    Logger::info("Client disconnected");
                    client->closeConnection();
                    delete client;
                    clients.erase(clients.begin() + (i - 1));
                    pollManager.removeFd(i);
                    i--;
                } else {
                    std::cout << "\n[Request]\n" << client->getBuffer() << std::endl;

                    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\nhello friend\n";
                    client->sendData(response);

                    // Close connection after sending response
                    client->closeConnection();
                    delete client;
                    clients.erase(clients.begin() + (i - 1));
                    pollManager.removeFd(i);
                    i--;
                }
            }
        }
    }

    for (size_t i = 0; i < clients.size(); i++) {
        clients[i]->closeConnection();
        delete clients[i];
    }
    server.stop();

    return 0;
}
