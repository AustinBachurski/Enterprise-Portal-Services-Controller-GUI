#ifndef SERVICES_HPP
#define SERVICES_HPP

#include "Constants.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <format>
#include <string>
#include <string_view>
#include <thread>
#include <wx/wx.h>

// Handles the communication with the Enterprise Portal.
class Services
{
public:
	/*
	@param Enterprise Portal URL.
	@param Enterprise Portal username.
	@param Enterprise Portal password.
	*/
	Services(const std::string& username,
			 const std::string& password,
			 const std::string& portal);

	/**
	 * Nested unordered_map of strings containing map server information.
	 * ------------------------------
	 * |Folder One					|
	 * |....Service One, status		|
	 * |....Service Two, status		|
	 * |Folder Two					|
	 * |....Service Three, status	|
	 * |....Service Four, status	|
	 * ------------------------------
	 */
	std::unordered_map<
		std::string, std::unordered_map<
			std::string, std::string>> m_serviceInformation;

	// @return True if functionality is disabled due to error.
	bool functionalityDisabled() const;

	// @return Number of services started as a string.
	const std::string getCountStartedAsString() const;

	// @return Number of services stopped as a string.
	const std::string getCountStoppedAsString() const;

	// @return Total services on the server.
	const int getCountTotal() const;

	// @return Number of services as a string.
	const std::string getCountTotalAsString() const;

	// @return Enterprise Portal URL.
	const std::string getPortal() const;

	// @return Number of services successfully read.
	const int getProgress() const;

	// @return The time the server status was last updated.
	const std::string getTimeStamp() const;

	// Set the progress counter to zero.
	void resetProgressCounter();

	/*
	Sends start/stop map services command to the server.
	@param Promise from calling thread.
	@param Constants::Commands::START or Constants::Commands::STOP
	*/
	void sendServerCommand(std::promise<nlohmann::json>&& promise,
		const std::string_view command);

	// Updates the "status" portion of m_serviceInformation.
	void updateStatus();

private:
	int m_countCurrent;
	int m_countStarted;
	int m_countStopped;
	int m_countTotal;
	bool m_error;
	// Block tool function unless credentials were entered.
	bool m_functionalityDisabled;
	std::chrono::system_clock::time_point m_expiration;
	const std::string m_password;
	const std::string m_portalAddress;
	const std::string m_portalServer;
	int m_progress;
	const std::string m_referer;
	nlohmann::json m_serverResponse;
	std::string m_statusTime;
	std::string m_token;
	const std::string m_tokenUrl;
	const std::string m_urlStart;
	const std::string m_urlStop;
	const std::string m_username;

	/*
	Aquires a security token from the server and sets m_expiration to 50
	minutes, displays a popup window if a valid token is not returned.
	*/
	void aquireToken();

	/*
	 Compares status information in m_serviceInformation with the paramater
	 to generate a JSON formatted string to be sent to the server.
	 @param Constants::Commands::START or Constants::Commands::STOP
	 @return JSON formatted string of map services that are not in the "command" state.
	 */
	std::string generateJson(const std::string_view command);

	// @return True if folder name is not "Hosted", "System", or "Utilities".
	bool isValidFolder(const std::string& folder) const;

	/*
	 Retrieves folders and services from Root.
	 @return Server response as JSON.
	 */
	nlohmann::json retrieveInformationFromServer();

	/*
	 Retrieves information from the server based on the folder/service passed in.
	 @return Server response as JSON.
	 */
	nlohmann::json retrieveInformationFromServer(const std::string& target);

	// Updates the map services information in m_serviceInformation.
	void retrieveServicesFromServer();

	// Sets m_countTotal to the current map service count.
	void setServiceCount();

	// Sets m_statusTime to the current time.
	void timeStamp();

	// @return True if token has not expired.
	bool tokenIsValid() const;
};

#endif