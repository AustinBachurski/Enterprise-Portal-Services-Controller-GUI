#include "Configuration.hpp"

Configuration::Configuration()
    : m_encryptionKey{ generateKey() },
      m_password{},
      m_portalUrl{},
      m_username{}
{
    readConfig();
}

void Configuration::createDirectory()
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

bool Configuration::credentialsAquired() const
{
    return m_passwordAquired && m_portalUrlAquired && m_usernameAquired;
}

char Configuration::decrypt(const char character)
{
    return m_encryptionKey.find(character) + m_invalidCharacterRange;
}

std::string Configuration::encrypt(std::string&& string)
{
    std::transform(string.cbegin(), string.cend(), string.begin(),
        [this](char character) -> char
        {
            return m_encryptionKey.at(character - m_invalidCharacterRange);
        });

    return string;
}

void Configuration::enterCredentials()
{
    EnterCredentials* credentialsWindow = new EnterCredentials(NULL, *this);
    credentialsWindow->ShowModal();
}

std::string Configuration::generateKey()
{
    const char upperBounds{ 126 };
    const char lowerBounds{ 31 };
    const int keySize{
        static_cast<int>(upperBounds) - static_cast<int>(lowerBounds) };
    std::string key;
    key.reserve(keySize);

    for (char i = upperBounds; i > lowerBounds; --i)
    {
        key.push_back(i);
    }
    return key;
}

int Configuration::getCommandMethod() const
{
    return m_commandMethod;
}

std::string Configuration::getPassword() const
{
    return m_password;
}

const std::string& Configuration::getPortal() const
{
    return m_portalUrl;
}

std::string Configuration::getUsername() const
{
    return m_username;
}

int Configuration::getWindowSizeX() const
{
    return m_windowSizeX;
}

int Configuration::getWindowSizeY() const
{
    return m_windowSizeY;
}

void Configuration::parseData(std::string& data)
{
    std::transform(data.cbegin(), data.cend(), data.begin(),
        [this](char character) -> char { return decrypt(character); });

    if (data.find("url=") != std::string::npos)
    {
        m_portalUrl = data.substr(data.find('=') + 1);
        m_portalUrlAquired = true;
    }
    else if (data.find("username=") != std::string::npos)
    {
        m_username = data.substr(data.find('=') + 1);
        m_usernameAquired = true;
    }
    else if (data.find("password=") != std::string::npos)
    {
        m_password = data.substr(data.find('=') + 1);
        m_passwordAquired = true;
    }
    else if (data.find("windowSizeX=") != std::string::npos)
    {
        m_windowSizeX = std::stoi(data.substr(data.find('=') + 1));
    }
    else if (data.find("windowSizeY=") != std::string::npos)
    {
        m_windowSizeY = std::stoi(data.substr(data.find('=') + 1));
    }
    else if (data.find("commandMethod=") != std::string::npos)
    {
        m_commandMethod = std::stoi(data.substr(data.find('=') + 1));
    }
    else
    {
        enterCredentials();
    }
}

void Configuration::readConfig()
{
    if (std::filesystem::exists(m_configDirectory))
    {
        if (std::filesystem::exists(m_configFile))
        {
            std::ifstream configFile(m_configFile);
            if (configFile.is_open())
            {
                std::string data{};

                while (std::getline(configFile, data))
                {
                    parseData(data);
                }
            }
            else
            {
                wxMessageDialog* fileError = new wxMessageDialog(NULL,
                    "Config File Error",
                    "Unable to open configuration file.",
                    wxOK | wxICON_ERROR);
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
        enterCredentials();
    }
}

void Configuration::updateConfigSettings(
    const int windowSizeX,
    const int windowSizeY,
    const int commandMethod)
{
    if (windowSizeX != m_windowSizeX
        || windowSizeY != m_windowSizeY
        || commandMethod != m_commandMethod)
    {
        // Open file for writing, discard existing data.
        std::ofstream configFile(m_configFile, std::ios::out, std::ios::trunc);
        if (configFile.is_open())
        {
            configFile
                << encrypt("url=" + m_portalUrl) << '\n'
                << encrypt("username=" + m_username) << '\n'
                << encrypt("password=" + m_password) << '\n'
                << encrypt("windowSizeX=" + std::to_string(windowSizeX)) << '\n'
                << encrypt("windowSizeY=" + std::to_string(windowSizeY)) << '\n'
                << encrypt("commandMethod=" + std::to_string(commandMethod));
        }
    }
}

void Configuration::writeConfig(
    const std::string&& portalUrl,
    const std::string&& username,
    const std::string&& password)
{
    // Open file for writing, discard existing data.
    std::ofstream configFile(m_configFile, std::ios::out, std::ios::trunc);
    if (configFile.is_open())
    {
        configFile
            << encrypt("url=" + portalUrl) << '\n'
            << encrypt("username=" + username) << '\n'
            << encrypt("password=" + password) << '\n'
            << encrypt("windowSizeX=" + std::to_string(m_windowSizeX)) << '\n'
            << encrypt("windowSizeY=" + std::to_string(m_windowSizeY)) << '\n'
            << encrypt("commandMethod=" + std::to_string(m_commandMethod));
        configFile.close();
        readConfig();
    }
    else
    {
        wxMessageDialog* fileError = new wxMessageDialog(NULL,
            Constants::Messages::fileError,
            "Config File Error", wxOK | wxICON_ERROR);
        fileError->ShowModal();
        return;
    }
}
