#include "Credentials.hpp"

Credentials::Credentials() :
    m_configFile{ "./config/config.ini" },
    m_configDirectory{ "./config" },
    m_encryptionKey
    { "~}|{zyxwvutsrqponmlkjihgfedcba`_^]\\[ZYXWVUTSRQPO"
      "NMLKJIHGFEDCBA@?>=<;:9876543210/.-,+*)('&%$#\"! " },
    m_invalidCharacterRange{ 32 },
    m_password{},
    m_passwordAquired{ false },
    m_portalAddress{},
    m_portalAddressAquired{ false },
    m_username{},
    m_usernameAquired{ false }
{
    readConfig();
}

void Credentials::createDirectory()
{
    try
    {
        std::filesystem::create_directory(m_configDirectory);
    }
    catch (std::filesystem::filesystem_error)
    {
        wxMessageDialog* directoryError = new wxMessageDialog(NULL,
            Constants::Messages::directoryError,
            "Directory Error", wxOK | wxICON_ERROR);
        directoryError->ShowModal();
    }

    // Verify the directory was successfully created.
    if (!std::filesystem::exists(m_configDirectory))
    {
        wxMessageDialog* directoryError = new wxMessageDialog(NULL,
            Constants::Messages::directoryError,
            "Directory Error", wxOK | wxICON_ERROR);
        directoryError->ShowModal();
    }
}

bool Credentials::credentialsAquired() const
{
    return m_passwordAquired && m_portalAddressAquired && m_usernameAquired;
}

std::string Credentials::decrypt(const std::string& string)
{
    std::string decrypted{};
    for (const auto& character : string)
    {
        decrypted += m_encryptionKey.find(
            std::stoi(std::to_string(character))) + m_invalidCharacterRange;
    }
    return decrypted;
}

std::string Credentials::encrypt(const std::string&& string)
{
    std::string encrypted{};
    for (const auto& character : string)
    {
        encrypted += m_encryptionKey.at(
            std::stoi(std::to_string(character)) - m_invalidCharacterRange);
    }
    return encrypted;
}

void Credentials::enterCredentials()
{
    EnterCredentials* credentialsWindow = new EnterCredentials(NULL, *this);
    credentialsWindow->ShowModal();
}

const std::string Credentials::getPassword() const
{
    return m_password;
}

const std::string Credentials::getPortal() const
{
    return m_portalAddress;
}

const std::string Credentials::getUsername() const
{
    return m_username;
}

void Credentials::readConfig()
{
    if (std::filesystem::exists(m_configDirectory))
    {
        if (std::filesystem::exists(m_configFile))
        {
            std::ifstream configFile(m_configFile);
                if (configFile.is_open())
                {
                    std::string line{};

                    while (std::getline(configFile, line))
                    {
                            std::string data = decrypt(line);

                            // C++23 if (data.contains("url="))
                            if (data.find("url=") != std::string::npos)
                            {
                                m_portalAddress = data.substr(data.find('=') + 1);
                                m_portalAddressAquired = true;
                            }
                        // C++23 else if (data.contains("username="))
                            else if (data.find("username=") != std::string::npos)
                            {
                                m_username = data.substr(data.find('=') + 1);
                                m_usernameAquired = true;
                            }
                        // C++23 else if (data.contains("password="))
                            else if (data.find("password=") != std::string::npos)
                            {
                                m_password = data.substr(data.find('=') + 1);
                                m_passwordAquired = true;
                            }
                            else
                            {
                                enterCredentials();
                            }
                    }
                }
                else
                {
                    wxMessageDialog* fileError = new wxMessageDialog(NULL,
                        "Config File Error",
                        "Unable to open configuration file.", wxOK | wxICON_ERROR);
                    fileError->ShowModal();
                }
        }
        else
        {
            enterCredentials();
        }
    }
    else
    {
        createDirectory();
        readConfig();
    }
}

void Credentials::setConfig(const std::string&& address,
                            const std::string&& username,
                            const std::string&& password)
{
    // Open file for writing, discard existing data.
    std::ofstream configFile(m_configFile, std::ios::out, std::ios::trunc);
    if (configFile.is_open())
    {
        configFile << encrypt("url=" + address) << '\n'
                   << encrypt("username=" + username) << '\n'
                   << encrypt("password=" + password);
        configFile.close();

        readConfig();
    }
    else
    {
        wxMessageDialog* fileError = new wxMessageDialog(NULL,
            Constants::Messages::fileError,
            "Config File Error", wxOK | wxICON_ERROR);
        fileError->ShowModal();
    }
}