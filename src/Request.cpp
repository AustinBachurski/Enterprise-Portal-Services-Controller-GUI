#include "Request.hpp"

Request::Request() : 
    m_configFile{ "./config/config.ini" },
    m_encryptionKey
    { "~}|{zyxwvutsrqponmlkjihgfedcba`_^]\\[ZYXWVUTSRQPO"
      "NMLKJIHGFEDCBA@?>=<;:9876543210/.-,+*)('&%$#\"! " },
    m_expiration{},
    m_password{},
    m_portalAddress{},
    m_portalServer{},
    m_referer{},
    m_token{},
    m_tokenUrl{},
    m_username{}
{
    readConfig();
    m_tokenUrl.append(m_portalAddress + "/portal/sharing/rest/generateToken");
    m_referer.append(m_portalAddress + "/server/admin");
    m_portalServer.append(m_portalAddress + "/server/admin/services");
    getToken();
}

bool Request::tokenIsValid()
{
    return std::chrono::system_clock::now() < m_expiration;
}

std::string Request::decrypt(const std::string& string)
{
	std::string decrypted {};
	for (const auto& character : string)
	{
		decrypted += m_encryptionKey.find(character) + 32;
	}
	return decrypted;
}

std::string Request::encrypt(const std::string& string)
{
	std::string encrypted{};
	for (const auto& character : string)
	{
		encrypted += m_encryptionKey.at(character - 32);
	}
	return encrypted;
}

void Request::readConfig()
{
    std::string line{};
    if (std::filesystem::exists(m_configFile))
    {
        std::fstream configFile(m_configFile);
        if (configFile.is_open())
        {
            // Read URL.
            std::getline(configFile, line);
            if (line.contains("url="))
            {
                m_portalAddress = decrypt(line.substr(line.find('=') + 1));
            }
            else
            {
                // TODO: Get information from user and call setConfig().
            }
            // Read username.
            std::getline(configFile, line);
            if (line.contains("username="))
            {
                m_username = decrypt(line.substr(line.find('=') + 1));
            }
            else
            {
                // TODO: Get information from user and call setConfig().
            }
            // Read password.
            std::getline(configFile, line);
            if (line.contains("password="))
            {
                m_password = decrypt(line.substr(line.find('=') + 1));
            }
            else
            {
                // TODO: Get information from user and call setConfig().
            }
        }
        else
        {
            // TODO: Spawn modal window with unable to open config file error.
        }
    }
    else
    {
        // TODO: Get information from user and call setConfig().
    }
}

void Request::setConfig(std::string address,
                        const std::string& username,
                        const std::string& password)
{
    // Open file for writing, discard existing data.
    std::ofstream configFile(m_configFile, std::ios::out, std::ios::trunc);
    if (configFile.is_open())
    {
        /* 
        * Copy/paste of a URL frequently appends a '/' to the end of 
        * the string even if not intended, so we check for that and
        * clean as necessary to prevent issues.
        */
        while (address.back() == '/')
        {
            address.pop_back();
        }

        configFile << "url=" << encrypt(address) << '\n'
            << "username=" << encrypt(username) << '\n'
            << "password=" << encrypt(password);
    }
    else
    {
        // TODO: Spawn modal window with unable to generate config file error.
    }
}

nlohmann::json Request::getStatusFromServer()
{
    // Retrieves all folders from the server.
	if (!tokenIsValid())
	{
		getToken();
	}

	return nlohmann::json::parse(cpr::Get(cpr::Url{ m_portalServer },
            cpr::Parameters{ { "token", m_token },{ "f", "json" } }).text);
}

nlohmann::json Request::getStatusFromServer(std::string& target)
{
    // Retrieves information from the server based on the target passed in.
	if (!tokenIsValid())
	{
		getToken();
	}

	return nlohmann::json::parse(cpr::Get(
            cpr::Url{ m_portalServer + '/' + target },
            cpr::Parameters{ { "token", m_token },{ "f", "json" } }).text);
}

void Request::getToken()
{
	nlohmann::json response = nlohmann::json::parse(cpr::Post(
		cpr::Url{ m_tokenUrl },
		cpr::Payload
		{
			{ "username", m_username },
			{ "password", m_password },
			{ "client", "referer" },
			{ "referer", m_referer },
			{ "expiration", "1 hour" },
			{ "f", "json" }
		}).text);

	if (response.contains("token"))
	{
		// Token is acquired for one hour,
        // reacquire after 50 minutes so we don't clip expiration.
		m_expiration = std::chrono::system_clock::now()
                        + std::chrono::minutes(50);
		m_token = response["token"];
	}
	else if (response.contains("error"))
	{
        // Spawn modal window containing response[details].
	}
}

