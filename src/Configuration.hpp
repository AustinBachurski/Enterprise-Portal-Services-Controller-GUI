#ifndef PSC_CONFIGURATION_HPP
#define PSC_CONFIGURATION_HPP

#include "EnterCredentials.hpp"
#include "Constants.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <wx/wx.h>

class Configuration
{
public:
	bool m_updateCredentials{ false };

	Configuration();

	bool credentialsAquired() const;
	void enterCredentials();
	int getCommandMethod() const;
	std::string getUsername() const;
	std::string getPassword() const;
	const std::string& getPortal() const;
	int getWindowPositionX() const;
	int getWindowPositionY() const;
	int getWindowSizeX() const;
	int getWindowSizeY() const;
	void readConfig();
	void updateConfigSettings(
		const int windowSizeX, const int windowSizeY,
		const int windowPositionX, const int windowPositionY,
		const int commandMethod);
	void writeConfig( // Automatically calls readConfig() after writing.
		const std::string&& portalUrl,
		const std::string&& username,
		const std::string&& password);

private:
	int m_commandMethod{ Constants::Commands::sequentialMode };
	const std::string m_configDirectory{ "./config" };
	const std::string m_configFile{ "./config/config.ini" };
	const std::string m_encryptionKey;
	const int m_invalidCharacterRange{ 32 };
	std::string m_password;
	bool m_passwordAquired{ false };
	std::string m_portalUrl;
	bool m_portalUrlAquired{ false };
	std::string m_username;
	bool m_usernameAquired{ false };
	int m_windowSizeX{ 475 };
	int m_windowSizeY{ 500 };
	int m_windowPositionX{ 50 };
	int m_windowPositionY{ 50 };

	void createDirectory();
	char decrypt(const char character);
	std::string encrypt(std::string&& string);
	std::string generateKey();
	void parseData(std::string& data);
};

#endif
