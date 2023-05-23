#ifndef CREDENTIALS_HPP
#define CREDENTIALS_HPP

#include "EnterCredentials.hpp"
#include "Constants.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <wx/wx.h>

class Credentials
{
public:
	Credentials();

	// @return True if all credentials were read successfully.
	bool credentialsAquired() const;

	// Displays a window to allow user to enter Enterprise Portal credentials.
	void enterCredentials();

	// @return Decrypted Enterprise Portal URL.
	const std::string getPortal() const;

	/*
	Writes data encrypted by encrypt() to the ./config/config.ini file.
	@param Enterprise Portal URL.
	@param Enterprise Portal username.
	@param Enterprise Portal password.
	*/
	void setConfig(const std::string&& address,
		const std::string&& username,
		const std::string&& password);

	// @retrun Decrypted Enterprise Portal username.
	const std::string getUsername() const;

	// @return Decrypted Enterprise Portal password.
	const std::string getPassword() const;

private:
	const std::string m_configFile;
	const std::string m_configDirectory;
	const std::string m_encryptionKey;

	// Offset to avoid invalid characters in ASCII.
	const int m_invalidCharacterRange;

	std::string m_password;
	bool m_passwordAquired;
	std::string m_portalAddress;
	bool m_portalAddressAquired;
	std::string m_username;
	bool m_usernameAquired;

	/*
	Creates the "./config" directory.
	*/
	void createDirectory();

	/*
	Descrypts the passed string.
	@param String previously encrypted by encrypt()
	@return Encrypted string.
	*/
	std::string decrypt(const std::string& string);

	/*
	Encrypts the passed string.
	@param String you wish to encrypt.
	@return Encrypted string.
	*/
	std::string encrypt(const std::string&& string);

	/*
	Reads config data from the "./config/config.ini" file, decrypts said data.
	If data is valid, sets the values of m_portalAddress, m_username, and
	m_password.
	*/
	void readConfig();
};

#endif