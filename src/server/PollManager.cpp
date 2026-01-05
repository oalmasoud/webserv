#include "PollManager.hpp"

PollManager::PollManager() {}

PollManager::~PollManager() {}

void PollManager::addFd(int fd, int events) {
    struct pollfd pfd;
    pfd.fd      = fd;
    pfd.events  = events;
    pfd.revents = 0;
    fds.push_back(pfd);
}

void PollManager::removeFd(size_t index) {
    if (index >= fds.size()) {
        return;
    }

    size_t lastIndex = fds.size() - 1;

    if (index != lastIndex) {
        fds[index] = fds[lastIndex];
    }

    fds.pop_back();
}

int PollManager::pollConnections(int timeout) {
    if (fds.empty()) {
        return 0;
    }
    return poll(&fds[0], fds.size(), timeout);
}

bool PollManager::hasEvent(size_t index, int event) const {
    return (index < fds.size()) && (fds[index].revents & event);
}

size_t PollManager::size() const {
    return fds.size();
}
