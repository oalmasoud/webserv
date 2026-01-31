#ifndef LOCATION_CONFIG_HPP
#define LOCATION_CONFIG_HPP
#include <iostream>
#include <vector>
#include "../utils/Logger.hpp"
#include "../utils/Utils.hpp"
class LocationConfig {
   public:
    LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    LocationConfig(const std::string& p);
    ~LocationConfig();

    bool setRoot(const VectorString& r);
    void setRoot(const std::string& r);

    void setAutoIndex(bool v);
    bool setAutoIndex(const VectorString& v);

    bool setIndexes(const VectorString& i);
    void setUploadEnabled(bool v);
    void setUploadPath(const std::string& p);
    bool setUploadPath(const VectorString& p);
    void setCgiEnabled(bool v);
    void setCgiPath(const std::string& p);
    bool setCgiPath(const VectorString& p);
    void setCgiExtension(const std::string& e);
    bool setCgiExtension(const VectorString& e);
    void setRedirect(const std::string& r);
    bool setRedirect(const VectorString& r);

    void setClientMaxBody(const std::string& c);
    bool setClientMaxBody(const VectorString& c);

    void         addAllowedMethod(const std::string& m);
    bool         setAllowedMethods(const VectorString& m);
    std::string  getPath() const;
    std::string  getRoot() const;
    bool         getAutoIndex() const;
    VectorString getIndexes() const;
    bool         getUploadEnabled() const;
    std::string  getUploadPath() const;
    bool         getCgiEnabled() const;
    std::string  getCgiPath() const;
    std::string  getCgiExtension() const;
    std::string  getRedirect() const;
    std::string  getClientMaxBody() const;
    VectorString getAllowedMethods() const;

   private:
    // required location parameters
    std::string path;
    // optional location parameters
    std::string  root;           // default root of server if not set (be required)
    bool         autoIndex;      // default: false
    VectorString indexes;        // default: root if not set be default "index.html"
    bool         uploadEnabled;  // default: false
    std::string  uploadPath;     // be required if uploadEnabled is true
    bool         cgiEnabled;     // default: false
    std::string  cgiPath;        // be required if cgiEnabled is true
    std::string  cgiExtension;   // be required if cgiEnabled is true
    std::string  redirect;       // default: ""
    std::string  clientMaxBody;  // default: ""
    VectorString allowedMethods; // default: GET
};

#endif