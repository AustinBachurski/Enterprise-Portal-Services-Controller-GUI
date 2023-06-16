#ifndef PSC_PORTAL_SERVER_CONTROLS_HPP
#define PSC_PORTAL_SERVER_CONTROLS_HPP

#include "Configuration.hpp"
#include "Constants.hpp"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <wx/wx.h>
#include <wx/progdlg.h>

#include <algorithm>
#include <execution>
#include <chrono>
#include <format>
#include <future>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

class Configuration;

class PortalServerControls
{
public:
	/*
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
	std::unordered_map<std::string,
		std::unordered_map<std::string, std::string>> m_serviceInformation;

	PortalServerControls(Configuration& configuration);

	bool credentialsAreValid() const;
	std::string generateJson(const std::string_view command);
	std::string getCountStartedAsString() const;
	std::string getCountStoppedAsString() const;
	int getCountTotal() const;
	std::string getCountTotalAsString() const;
	std::string getTimeStamp() const;
	bool isStatusValid() const;
	void sendBatchServerCommand(
		std::promise<nlohmann::json>&& promise,
		const std::string_view command);
	void sendSequentialServerCommands(
		std::promise<nlohmann::json>&& promise,
		const std::string_view command,
		threadState& state);
	bool tokenIsValid() const;
	void updateStatus();
	void updateStatus(threadState& state);

private:
	Configuration& m_configuration;
	int m_countCurrent{ 0 };
	int m_countStarted{ 0 };
	int m_countStopped{ 0 };
	int m_countTotal{ 0 };
	bool m_credentialsValid{ false };
	std::chrono::system_clock::time_point m_expiration;
	bool m_initialStatusGathered{ false };
	std::mutex m_mutex;
	const std::string m_portalServer;
	const std::string m_referer;
	const std::string m_startAllUrl;
	std::string m_statusTime{ Messages::initialStatus };
	const std::string m_stopAllUrl;
	std::string m_token;
	const std::string m_tokenUrl;

	struct URLandServerResponse
	{
		std::string url;
		std::future<nlohmann::json> serverResponse;
	};

	void aquireToken();
	std::vector<std::string> generateTargets(const std::string_view command);
	nlohmann::json issueCommand(const std::string url);
	bool isValidFolder(const std::string& folder) const;
	nlohmann::json retrieveInformationFromServer(const std::string& target = "");
	void retrieveCount(const std::string& folder);
	void retrieveServices();
	void retrieveStatus(
		const std::string& folder,
		const std::string& service);
	void retrieveStatus(
		const std::string& folder,
		const std::string& service,
		threadState& state);
	void setServiceCount();
	void timeStamp();
};

#endif