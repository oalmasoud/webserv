#ifndef DIRECTORY_LISTING_HPP
#define DIRECTORY_LISTING_HPP
#include <dirent.h>
#include <iostream>
#include <vector>
#include "../utils/Logger.hpp"
#include "../utils/Utils.hpp"
struct FileInfo {
    std::string name;
    bool        isDirectory;
    size_t      size;
    std::string lastModified;
};
class DirectoryListing {
   public:
    DirectoryListing();
    DirectoryListing(const DirectoryListing& other);
    DirectoryListing& operator=(const DirectoryListing& other);
    ~DirectoryListing();
    // Getter and Setter for pathDirectory
    std::string getPathDirectory() const;
    void        setPathDirectory(const std::string& path);
    bool        generateHtml();
    std::string getHtmlContent() const;

   private:
    std::map<std::string, FileInfo> entries;

    std::string pathDirectory;
    std::string htmlContent;
    bool        readDirectoryEntries(const std::string& path, std::map<std::string, FileInfo>& entries);
};

#endif