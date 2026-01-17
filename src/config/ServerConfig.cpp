#include "ServerConfig.hpp"
ServerConfig::ServerConfig() : port(80) {}
ServerConfig::~ServerConfig() {}
void ServerConfig::setPort(int p) {
    port = p;
}
void ServerConfig::setServerName(const std::string& name) {
    serverName = name;
}
void ServerConfig::setRoot(const std::string& r) {
    root = r;
}
void ServerConfig::addLocation(const LocationConfig& loc) {
    locations.push_back(loc);
}

void ServerConfig::indexLocations() {
    for (size_t i = 0; i < locations.size(); ++i) {
        if (locations[i].getRoot().empty())
            locations[i].setRoot(root);
    }
}

int ServerConfig::getPort() const {
    return port;
}
std::string ServerConfig::getServerName() const {
    return serverName;
}
std::string ServerConfig::getRoot() const {
    return root;
}
std::vector<LocationConfig> ServerConfig::getLocations() const {
    return locations;
}