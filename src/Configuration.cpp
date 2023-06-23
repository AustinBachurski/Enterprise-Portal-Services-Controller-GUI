#include "Configuration.hpp"

Configuration::Configuration()
    : m_encryptionKey{ generateKey() }
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
            Message::directoryError,
            "Directory Error", wxOK | wxICON_ERROR);
        directoryError->ShowModal();
    }

    // Verify the directory was successfully created.
    if (!std::filesystem::exists(m_configDirectory))
    {
        wxMessageDialog* directoryError = new wxMessageDialog(NULL,
            Message::directoryError,
            "Directory Error", wxOK | wxICON_ERROR);
        directoryError->ShowModal();
    }
}

bool Configuration::credentialsAcquired() const
{
    return m_passwordAcquired && m_portalUrlAcquired && m_usernameAcquired;
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
    constexpr int upperBounds{ 126 };
    constexpr int lowerBounds{ 31 };
    constexpr int keySize{ upperBounds - lowerBounds };
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

const wxRect& Configuration::getWindowSettings() const
{
    return m_window;
}

void Configuration::parseData(std::string& data)
{
    std::transform(data.cbegin(), data.cend(), data.begin(),
        [this](char character) -> char { return decrypt(character); });

    if (data.find("url=") != std::string::npos)
    {
        m_portalUrl = data.substr(data.find('=') + 1);
        if (!m_portalUrl.empty())
        {
            m_portalUrlAcquired = true;
        }
    }
    else if (data.find("username=") != std::string::npos)
    {
        m_username = data.substr(data.find('=') + 1);
        if (!m_username.empty())
        {
            m_usernameAcquired = true;
        }
    }
    else if (data.find("password=") != std::string::npos)
    {
        m_password = data.substr(data.find('=') + 1);
        if (!m_password.empty())
        {
            m_passwordAcquired = true;
        }
    }
    else if (data.find("windowWidth=") != std::string::npos)
    {
        m_window.width = std::stoi(data.substr(data.find('=') + 1));
    }
    else if (data.find("windowHeight=") != std::string::npos)
    {
        m_window.height = std::stoi(data.substr(data.find('=') + 1));
    }
    else if (data.find("windowPositionX=") != std::string::npos)
    {
        m_window.x = std::stoi(data.substr(data.find('=') + 1));
    }
    else if (data.find("windowPositionY=") != std::string::npos)
    {
        m_window.y = std::stoi(data.substr(data.find('=') + 1));
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
                std::string data;

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
    const wxRect& windowSettings, const int commandMethod)
{
    if (windowSettings.width != m_window.width
        || windowSettings.height != m_window.height
        || windowSettings.x != m_window.x
        || windowSettings.y != m_window.y
        || commandMethod != m_commandMethod)
    {
        // Open file for writing | discard existing data.
        std::ofstream configFile(m_configFile, std::ios::out | std::ios::trunc);
        if (configFile.is_open())
        {
            configFile
                << encrypt("url=" + m_portalUrl) << '\n'
                << encrypt("username=" + m_username) << '\n'
                << encrypt("password=" + m_password) << '\n'
                << encrypt("windowWidth="
                    + std::to_string(windowSettings.width)) << '\n'
                << encrypt("windowHeight="
                    + std::to_string(windowSettings.height)) << '\n'
                << encrypt("windowPositionX="
                    + std::to_string(windowSettings.x)) << '\n'
                << encrypt("windowPositionY="
                    + std::to_string(windowSettings.y)) << '\n'
                << encrypt("commandMethod="
                    + std::to_string(commandMethod));
        }
    }
}

void Configuration::writeConfig(
    const std::string portalUrl,
    const std::string username,
    const std::string password)
{
    // Open file for writing, discard existing data.
    std::ofstream configFile(m_configFile, std::ios::out, std::ios::trunc);
    if (configFile.is_open())
    {
        configFile
            << encrypt("url=" + portalUrl)
            << '\n'
            << encrypt("username=" + username)
            << '\n'
            << encrypt("password=" + password)
            << '\n'
            << encrypt("windowWidth=" + std::to_string(m_window.width))
            << '\n'
            << encrypt("windowHeight=" + std::to_string(m_window.height))
            << '\n'
            << encrypt("windowPositionX=" + std::to_string(m_window.x))
            << '\n'
            << encrypt("windowPositionY=" + std::to_string(m_window.y))
            << '\n'
            << encrypt("commandMethod=" + std::to_string(m_commandMethod));
        configFile.close();
        readConfig();
    }
    else
    {
        wxMessageDialog* fileError = new wxMessageDialog(NULL,
            Message::fileError,
            "Config File Error", wxOK | wxICON_ERROR);
        fileError->ShowModal();
        return;
    }
}