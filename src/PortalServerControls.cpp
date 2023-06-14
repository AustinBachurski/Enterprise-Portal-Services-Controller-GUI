#include "PortalServerControls.hpp"

PortalServerControls::PortalServerControls(Configuration& configuration)
	: m_configuration{ configuration },
	  m_portalServer{ configuration.getPortal() + "server/admin/services/"},
	  m_referer{ configuration.getPortal() + "server/admin/" },
	  m_startAllUrl{ configuration.getPortal()
			+ "server/admin/services/startServices" },
	  m_stopAllUrl{ configuration.getPortal()
			+ "server/admin/services/stopServices" },
	  m_tokenUrl{ configuration.getPortal()
			+ "portal/sharing/rest/generateToken" }
{
	if (m_configuration.credentialsAquired())
	{
		aquireToken();
		if (tokenIsValid())
		{
			setServiceCount();
			retrieveServices();
		}
		else
		{
			m_statusTime = Constants::Messages::serverError;
		}
	}
	else
	{
		m_statusTime = 
			"Credentials must be provided for the tool to work properly.";
	}
}

void PortalServerControls::aquireToken()
{
	cpr::Response response = cpr::Post(
		cpr::Url{ m_tokenUrl },
		cpr::Payload
		{
			{ "username", m_configuration.getUsername() },
			{ "password", m_configuration.getPassword() },
			{ "client", "referer" },
			{ "referer", m_referer },
			{ "expiration", "1 hour" },
			{ "f", "json" }
		});

	if (!response.text.empty())
	{
		nlohmann::json result = nlohmann::json::parse(response.text);

		if (result.contains("token"))
		{
			// Token is acquired for one hour,
			// reacquire after 50 minutes so we don't clip expiration.
			m_expiration = std::chrono::system_clock::now()
				+ std::chrono::minutes(50);
			m_token = result["token"];
			m_credentialsValid = true;
		}
		else if (result.contains("error"))
		{
			m_statusTime = Constants::Messages::serverError;

			std::string details = result["error"]["details"][0];

			wxMessageDialog* tokenError = new wxMessageDialog(NULL,
				Constants::Messages::tokenError + details,
				"Token Error",
				wxOK | wxICON_ERROR);
			tokenError->ShowModal();
		}
	}
	else
	{
		m_statusTime = Constants::Messages::serverError;

		wxMessageDialog* tokenError = new wxMessageDialog(NULL,
			Constants::Messages::noResponse 
			+ "URL: " + m_configuration.getPortal(),
			"No Response from Server",
			wxOK | wxICON_ERROR);
		tokenError->ShowModal();
	}
}

bool PortalServerControls::credentialsAreValid() const
{
	return m_credentialsValid;
}

std::vector<std::string> PortalServerControls::generateTargets(
	const std::string_view command)
{
	std::vector<std::string> list;

	for (const auto& [folder, services] : m_serviceInformation)
	{
		for (const auto& [service, status] : services)
		{
			if (status != command)
			{
				list.emplace_back(
					m_portalServer
					+ (folder == Constants::Words::root ? "" : folder + '/')
					+ service
					+ (command == Constants::Commands::START
						? "/start" : "/stop")
				);
			}
		}
	}
	return list;
}

std::string PortalServerControls::generateJson(const std::string_view command)
{
	std::string list{"{\"services\":["};

	for (const auto& [folder, services] : m_serviceInformation)
	{
		for (const auto& [service, status] : services)
		{
			if (status != command)
			{
				list.append(
					Constants::JSON::openBrace 
					+ Constants::JSON::folderName
					+ (folder == Constants::Words::root ? "" : folder)
					+ Constants::JSON::serviceName
					+ service.substr(0, service.find('.'))
					+ Constants::JSON::type
					+ service.substr(service.find('.') + 1)
					+ Constants::JSON::closeBrace
				);
			}
		}
	}
	// Remove the trailing ','  and append "]}" to properly terminate the JSON.
	if (list.rfind(',') != std::string::npos)
	{
		list.replace(list.rfind(','), 2, "]}");
	}
	return list;
}

std::string PortalServerControls::getCountStartedAsString() const
{
	return std::to_string(m_countStarted);
}

std::string PortalServerControls::getCountStoppedAsString() const
{
	return std::to_string(m_countStopped);
}

int PortalServerControls::getCountTotal() const
{
	return m_countTotal;
}

std::string PortalServerControls::getCountTotalAsString() const
{
	return std::to_string(m_countTotal);
}

std::string PortalServerControls::getTimeStamp() const
{
	return m_statusTime;
}

bool PortalServerControls::isStatusValid() const
{
	return m_initialStatusGathered;
}

nlohmann::json PortalServerControls::issueCommand(const std::string url)
{
	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Payload
		{
			{ "token", m_token },
			{ "f", "json" }
		});

	nlohmann::json result = nlohmann::json::parse(response.text);

	return result;
}

bool PortalServerControls::isValidFolder(const std::string& folder) const
{
	return folder != "Hosted"
		&& folder != "System"
		&& folder != "Utilities";
}

void PortalServerControls::retrieveCount(const std::string& folder)
{
	if (folder != Constants::Words::root)
	{
		std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
		nlohmann::json serverResponse = retrieveInformationFromServer(folder);

		for (const auto& service
			: serverResponse[Constants::Words::services])
		{
			lock.lock();
			++m_countTotal;
			lock.unlock();
		}
	}
}

nlohmann::json PortalServerControls::retrieveInformationFromServer(
	const std::string& target)
{
	if (!tokenIsValid())
	{
		aquireToken();
	}

	cpr::Response response = cpr::Get(
		cpr::Url{ m_portalServer + target },
		cpr::Parameters
		{
			{ "token", m_token },
			{ "f", "json" }
		});

	if (!response.text.empty())
	{
		nlohmann::json result = nlohmann::json::parse(response.text);

		return result;
	}
	else
	{
		return Constants::Messages::jsonError;
	}
}

void PortalServerControls::retrieveServices()
{
	nlohmann::json serverResponse{};

	for (const auto& [folder, _unused_] : m_serviceInformation)
	{
		if (folder != Constants::Words::root)
		{
			serverResponse = retrieveInformationFromServer(folder);
		}
		else
		{
			serverResponse = retrieveInformationFromServer();
		}

		if (serverResponse.contains(Constants::Words::services))
		{
			for (const auto& service
				: serverResponse[Constants::Words::services])
			{
				m_serviceInformation[folder]
					[service["serviceName"].get<std::string>()
					+ "."
					+ service["type"].get<std::string>()];
			}
		}
	}
}

void PortalServerControls::retrieveStatus(
	const std::string& folder,
	const std::string& service)
{
	std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
	nlohmann::json serverResponse{};

	if (folder == Constants::Words::root)
	{
		serverResponse = retrieveInformationFromServer(
			service + "/status");
	}
	else
	{
		serverResponse = retrieveInformationFromServer(
			folder + '/' + service + "/status");
	}

	for (const auto& status : serverResponse["realTimeState"])
	{
		lock.lock();
		m_serviceInformation[folder][service] = status;

		if (status.get<std::string>() == "STARTED")
		{
			++m_countStarted;
		}
		else if (status.get<std::string>() == "STOPPED")
		{
			++m_countStopped;
		}
		lock.unlock();
	}
}

void PortalServerControls::retrieveStatus(
	const std::string& folder,
	const std::string& service,
	threadState& state)
{
	std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
	nlohmann::json serverResponse{};

	if (folder == Constants::Words::root)
	{
		serverResponse = retrieveInformationFromServer(
			service + "/status");
	}
	else
	{
		serverResponse = retrieveInformationFromServer(
			folder + '/' + service + "/status");
	}

	for (const auto& status : serverResponse["realTimeState"])
	{
		lock.lock();
		++state.progressValue;
		if (state.message != Constants::Messages::gathering)
		{
			state.message = Constants::Messages::gathering;
		}

		m_serviceInformation[folder][service] = status;

		if (status.get<std::string>() == "STARTED")
		{
			++m_countStarted;
		}
		else if (status.get<std::string>() == "STOPPED")
		{
			++m_countStopped;
		}
		lock.unlock();
	}
}

void PortalServerControls::sendBatchServerCommand(
	std::promise<nlohmann::json>&& promise,
	const std::string_view command)
{
	updateStatus();
	const std::string services = generateJson(command);

	// If "type" is not in services, no commands were appended to the JSON
	// string because services were already at requested state.
	if (services.find("type") != std::string::npos)
	{
		cpr::Response response = cpr::Post(
			cpr::Url{ command == Constants::Commands::START
					  ? m_startAllUrl : m_stopAllUrl },
			cpr::Payload
			{
				{ "services", services},
				{ "token", m_token },
				{ "f", "json" }
			});

		nlohmann::json result = nlohmann::json::parse(response.text);
		updateStatus();

		promise.set_value(result);
	}
	else
	{
		promise.set_value(Constants::Messages::jsonNothingToDo);
	}
}

void PortalServerControls::sendSequentialServerCommands(
	std::promise<nlohmann::json>&& promise,
	const std::string_view command,
	threadState& state)
{
	std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
	const int seventyFivePercentProgress{ m_countTotal * 3 };
	updateStatus(state);
	state.message = "Generating commands and sending to server.";
	const std::vector<std::string> targetServices = generateTargets(command);

	std::vector<URLandServerResponse> results;
	std::vector<std::string> buffer;

	if (!targetServices.empty())
	{
		std::for_each(
			std::execution::par, targetServices.begin(), targetServices.end(),
			[&](const std::string& targetUrl)
			{
				lock.lock();
				++state.progressValue;

				results.emplace_back(URLandServerResponse(targetUrl,
					std::async(std::launch::async, 
						[this, &targetUrl]()->nlohmann::json
						{
							return issueCommand(targetUrl);
						})));
				lock.unlock();
			});

		while (!results.empty())
		{
			for (const auto& urlAndServerResponse : results)
			{
				urlAndServerResponse.serverResponse.wait();
				state.message = Constants::Messages::waiting;

				if (state.progressValue < seventyFivePercentProgress)
				{
					++state.progressValue;
				}
			}
			
			std::erase_if(results,
				[](URLandServerResponse& element) -> bool
				{ 
					nlohmann::json response = element.serverResponse.get();
					std::string test = response["status"];
					return response["status"] == "success";
				});

			if (!results.empty())
			{
				for (const auto& urlAndServerResponse : results)
				{
					// Using the buffer vector to avoid modifying
					// the results vector while iterating over it.
					buffer.push_back(urlAndServerResponse.url);
				}

				std::for_each(
					std::execution::par, buffer.begin(), buffer.end(),
					[&](const std::string& targetUrl)
					{
						lock.lock();
						++state.progressValue;

						results.emplace_back(URLandServerResponse(targetUrl,
							std::async(std::launch::async,
								[this, &targetUrl]()->nlohmann::json
								{
									return issueCommand(targetUrl);
								})));
						lock.unlock();
					});

				std::erase_if(results,
					[](URLandServerResponse& element) -> bool
					{
						return !element.serverResponse.valid();
					});
			}
		}
		state.progressValue = seventyFivePercentProgress;
		updateStatus(state);

		promise.set_value(Constants::Messages::success);
		state.working = false;
		return;
	}
	promise.set_value(Constants::Messages::jsonNothingToDo);
	state.working = false;
}

void PortalServerControls::setServiceCount()
{
	m_countTotal = 0;
	m_serviceInformation[ std::string(Constants::Words::root) ];
	std::vector<std::future<void>> futures;
	nlohmann::json serverResponse = retrieveInformationFromServer();

	if (serverResponse.contains(Constants::Words::folders)
		&& serverResponse.contains(Constants::Words::services))
	{
		// Add folders to the status container.
		for (const auto& folder
			: serverResponse[Constants::Words::folders])
		{
			if (isValidFolder(folder))
			{
				m_serviceInformation[folder];
			}
		}

		// Count services in root directory.
		for (const auto& service
			: serverResponse[Constants::Words::services])
		{
			++m_countTotal;
		}

		// Count services in each folder.
		for (const auto& [folder, _unused_] : m_serviceInformation)
		{
			futures.emplace_back(std::async(std::launch::async,
				[this, &folder]()
				{
					retrieveCount(folder);
				}));
		}
		for (const auto& future : futures)
		{
			future.wait();
		}
	}
}

void PortalServerControls::timeStamp()
{
	const std::chrono::zoned_time now{ std::chrono::current_zone(),
		std::chrono::system_clock::now() };

	// Format time as Hours:Minutes:AM/PM - 12 hour.
	std::string time = std::format("{:%I:%M %p}", now);

	if (time.front() == '0')
	{
		time.erase(0, 1);
	}

	m_statusTime = "Status as of " + time;
}

bool PortalServerControls::tokenIsValid() const
{
	return std::chrono::system_clock::now() < m_expiration;
}

void PortalServerControls::updateStatus()
{
	m_countStarted = 0;
	m_countStopped = 0;
	std::vector<std::future<void>> futures;

	for (const auto& [folder, services] : m_serviceInformation)
	{
		for (const auto& [service, _unused_] : services)
		{
			futures.emplace_back(std::async(std::launch::async, 
				[this, &folder, &service]()
				{
					retrieveStatus(folder, service);
				}));
		}
	}
	for (const auto& future : futures)
	{
		future.wait();
	}
	timeStamp();
	m_initialStatusGathered = true;
}

void PortalServerControls::updateStatus(threadState& state)
{
	m_countStarted = 0;
	m_countStopped = 0;
	std::vector<std::future<void>> futures;

	for (const auto& [folder, services] : m_serviceInformation)
	{
		for (const auto& [service, _unused_] : services)
		{
			futures.emplace_back(std::async(std::launch::async,
				[this, &folder, &service, &state]()
				{
					retrieveStatus(folder, service, state);
				}));
		}
	}
	for (const auto& future : futures)
	{
		future.wait();
	}
	timeStamp();
	m_initialStatusGathered = true;
	if (state.progressMax == m_countTotal)
	{
		state.working = false;
	}
}