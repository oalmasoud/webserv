#include "DirectoryListing.hpp"
#include <sys/stat.h>
DirectoryListing::DirectoryListing() : pathDirectory("") {}
DirectoryListing::DirectoryListing(const DirectoryListing& other) : pathDirectory(other.pathDirectory) {}
DirectoryListing& DirectoryListing::operator=(const DirectoryListing& other) {
    if (this != &other) {
        pathDirectory = other.pathDirectory;
    }
    return *this;
}
DirectoryListing::~DirectoryListing() {}

bool DirectoryListing::readDirectoryEntries(const std::string& path, std::map<std::string, FileInfo>& entries) {
    DIR* dir = opendir(path.c_str());
    if (!dir)
        return Logger::error("Failed to open directory: " + path);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;

        if (name == "." || name == "..")
            continue;

        std::string fullPath = path + "/" + name;

        struct stat buf;
        if (stat(fullPath.c_str(), &buf) == -1) {
            Logger::error("stat failed: " + fullPath);
            continue;
        }
        FileInfo info;
        info.name         = name;
        info.isDirectory  = S_ISDIR(buf.st_mode);
        info.size         = buf.st_size;
        info.lastModified = trimSpaces(ctime(&buf.st_mtime));
        entries[name]     = info;
    }
    if (closedir(dir) == -1)
        return Logger::error("Failed to close directory: " + path);
    return true;
}

bool DirectoryListing::generateHtml() {
    if (pathDirectory.empty() || !readDirectoryEntries(pathDirectory, entries))
        return false;

    htmlContent =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "  <title>Directory Listing</title>\n"
        "  <style>\n"
        "    body { font-family: monospace; background: #f0f0f0; color: #333; }\n"
        "    ul { list-style: none; padding: 0; }\n"
        "    li { padding: 0.5rem; background: white; margin: 0.2rem 0; border-radius: 4px; }\n"
        "    a { text-decoration: none; color: #1a73e8; }\n"
        "    .meta { color: #555; float: right; font-size: 0.9rem; }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <h2>Directory Listing: " +
        pathDirectory +
        "</h2>\n"
        "  <ul>\n";

    for (std::map<std::string, FileInfo>::iterator it = entries.begin(); it != entries.end(); ++it) {
        std::string sizeStream = typeToString(it->second.size) + " B";
        htmlContent += "    <li><a href=\"" + it->second.name + "\">" + it->second.name + "</a><span class=\"meta\">" + sizeStream + " | " +
                       it->second.lastModified + "</span></li>\n";
    }
    htmlContent += "  </ul>\n</body>\n</html>\n";

    return true;
}

std::string DirectoryListing::getPathDirectory() const {
    return pathDirectory;
}
std::string DirectoryListing::getHtmlContent() const {
    return htmlContent;
}
void DirectoryListing::setPathDirectory(const std::string& path) {
    pathDirectory = path;
}