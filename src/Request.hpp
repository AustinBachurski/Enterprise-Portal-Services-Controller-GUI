#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>


class Request
{
public:
    Request();

    nlohmann::json getStatusFromServer();
    nlohmann::json getStatusFromServer(std::string& folder);
    bool tokenIsValid();

private:
    const std::string m_configFile {};
    const std::string_view m_encryptionKey;
    std::chrono::system_clock::time_point m_expiration;
    std::string m_password;
    std::string m_portalAddress;
    std::string m_portalServer;
    std::string m_referer;
    std::string m_token;
    std::string m_tokenUrl;
    std::string m_username;
    std::string decrypt(const std::string& string);
    std::string encrypt(const std::string& string);
    void getToken();
    void readConfig();
    void setConfig(std::string address,
                    const std::string& username,
                    const std::string& password);
};

#endif