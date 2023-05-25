#include "Services.hpp"

Services::Services(const std::string& username, const std::string& password, const std::string& portal) :
	m_countCurrent{},
	m_countStarted{},
	m_countStopped{},
	m_countTotal{},
	m_error{ false },
	m_functionalityDisabled{ true },
	m_expiration{},
	m_password{ password },
	m_portalAddress{ portal },
	m_portalServer{ portal + "server/admin/services/" },
	m_progress{},
	m_referer{ portal + "server/admin/" },
	m_serverResponse{},
	m_statusTime{ Constants::Messages::initialStatus },
	m_token{},
	m_tokenUrl{ portal + "portal/sharing/rest/generateToken" },
	m_urlStart{ portal + "server/admin/services/startServices" },
	m_urlStop{ portal + "server/admin/services/stopServices" },
	m_username{ username }
{
	/*
	 Retrieving folder and service information from the server happens
	 quickly so we do this once when the program runs.  Retrieving
	 the service count prior to performing any work allows progress bars
	 to be dynamic.  Assuming credentials were found.
	 */

	if (!m_username.empty() && !m_password.empty() && !m_portalAddress.empty())
	{
		m_functionalityDisabled = false;
		setServiceCount();
		if (!m_error)
		{
			retrieveServicesFromServer();
		}
	}
	else
	{
		m_statusTime = "Credentials were not found, please restart the tool and "
			"enter your Enterprise Portal credentials when prompted.\n"
			"Tool functionality has been disabled for this session.";
	}
}

bool Services::functionalityDisabled() const
{
	return m_functionalityDisabled;
}

void Services::aquireToken()
{
	cpr::Response response = cpr::Post(
		cpr::Url{ m_tokenUrl },
		cpr::Payload
		{
			{ "username", m_username },
			{ "password", m_password },
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
		}
		else if (result.contains("error"))
		{
			m_error = true;
			m_functionalityDisabled = true;
			m_statusTime = Constants::Messages::serverError;

			// Spawn modal window containing response[details].
			wxMessageDialog* tokenError = new wxMessageDialog(NULL,
				Constants::Messages::tokenError + result["details"],
				"Token Error",
				wxOK | wxICON_ERROR);
			tokenError->ShowModal();
		}
	}
	else
	{
		m_error = true;
		m_functionalityDisabled = true;
		m_statusTime = Constants::Messages::serverError;

		wxMessageDialog* tokenError = new wxMessageDialog(NULL,
			Constants::Messages::noResponse + "URL: " + m_tokenUrl,
			"No Response from Server",
			wxOK | wxICON_ERROR);
		tokenError->ShowModal();
	}
}

std::string Services::generateJson(const std::string_view command)
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
					+ (folder == "Root" ? "" : folder)
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
	// C++23 if (list.contains(','))
	if (list.rfind(',') != std::string::npos)
	{
		list.replace(list.rfind(','), 2, "]}");
	}
	return std::move(list);
}

const std::string Services::getCountStartedAsString() const
{
	return std::to_string(m_countStarted);
}

const std::string Services::getCountStoppedAsString() const
{
	return std::to_string(m_countStopped);
}

const int Services::getCountTotal() const
{
	return m_countTotal;
}

const std::string Services::getCountTotalAsString() const
{
	return std::to_string(m_countTotal);
}

const std::string Services::getPortal() const
{
	return m_portalAddress;
}

const int Services::getProgress() const
{
	return m_progress;
}

const std::string Services::getTimeStamp() const
{
	return m_statusTime;
}

bool Services::isValidFolder(const std::string& folder) const
{
	return folder != "Hosted"
		&& folder != "System"
		&& folder != "Utilities";
}

void Services::resetProgressCounter()
{
	m_progress = 0;
}

nlohmann::json Services::retrieveInformationFromServer()
{
	if (!tokenIsValid())
	{
		aquireToken();
	}

	cpr::Response response = cpr::Get(cpr::Url{ m_portalServer },
		cpr::Parameters{ { "token", m_token },{ "f", "json" } });

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

nlohmann::json Services::retrieveInformationFromServer(const std::string& target)
{
	if (!tokenIsValid())
	{
		aquireToken();
	}

	cpr::Response response = cpr::Get(cpr::Url{ m_portalServer + target },
		cpr::Parameters{ { "token", m_token },{ "f", "json" } });

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

void Services::retrieveServicesFromServer()
{
	for (const auto& [folder, service] : m_serviceInformation)
	{
		if (folder == "Root")
		{
			m_serverResponse = retrieveInformationFromServer();
		}
		else
		{
			m_serverResponse = retrieveInformationFromServer(folder);
		}

		if (m_serverResponse.contains(Constants::Words::services))
		{
			for (const auto& service : m_serverResponse[Constants::Words::services])
			{
				m_serviceInformation[folder]
					[service["serviceName"].get<std::string>()
					+ "."
					+ service["type"].get<std::string>()];
			}
		}
	}
}

void Services::sendServerCommand(std::promise<nlohmann::json>&& promise, const std::string_view command)
{
	updateStatus();
	const std::string services = generateJson(command);

	// auto workToDo = [](const std::string& services)->bool
	//	   { return services.contains("type")}; // C++23
	auto workToDo = [](const std::string& services)->bool
	{ return services.find("type") != std::string::npos; };

	if (workToDo(services))
	{
		cpr::Response response = cpr::Post(cpr::Url
			{
				command == Constants::Commands::START ? m_urlStart : m_urlStop
			},
			cpr::Payload
			{
				{ "services", services},
				{"token", m_token},
				{"f", "json"}
			});

		nlohmann::json result = nlohmann::json::parse(response.text);
		updateStatus();

		promise.set_value(result);
	}
	else
	{
		promise.set_value(Constants::Messages::nothingToDo);
	}
}

void Services::setServiceCount()
{
	m_countTotal = 0;

	std::string folderName{ "Root" };
	m_serviceInformation[folderName];
	m_serverResponse = retrieveInformationFromServer();

	if (m_serverResponse.contains(Constants::Words::folders)
		&& m_serverResponse.contains(Constants::Words::services))
	{
		// Add folders to the status container.
		for (const auto& folder : m_serverResponse[Constants::Words::folders])
		{
			if (isValidFolder(folder))
			{
				m_serviceInformation[folder];
			}
		}

		// Count services in root directory.
		for (const auto& service : m_serverResponse[Constants::Words::services])
		{
			++m_countTotal;
		}

		// Count services in folders.
		for (const auto& [folder, service] : m_serviceInformation)
		{
			if (folder != "Root" && !m_error)
			{
				m_serverResponse = retrieveInformationFromServer(folder);

				for (const auto& service : m_serverResponse[Constants::Words::services])
				{
					++m_countTotal;
				}
			}
		}
	}
}

void Services::timeStamp()
{
	const std::chrono::zoned_time now{ std::chrono::current_zone(),
		std::chrono::system_clock::now() };

	// Format time as Hours:Minutes:AM/PM - 12 hour.
	std::string time = std::format("{:%I:%M %p}", now);

	// Remove leading '0' from time.
	if (time.front() == '0')
	{
		time.erase(0, 1);
	}

	m_statusTime = "Status as of " + time;
}

bool Services::tokenIsValid() const
{
	return std::chrono::system_clock::now() < m_expiration;
}

void Services::updateStatus()
{
	m_countStarted = 0;
	m_countStopped = 0;
	m_progress = 0;

	for (const auto& [folder, services] : m_serviceInformation)
	{
		for (const auto& [service, status] : services)
		{
			if (folder == "Root")
			{
				m_serverResponse = retrieveInformationFromServer(
					service + "/status");
			}
			else
			{
				m_serverResponse = retrieveInformationFromServer(
					folder + '/' + service + "/status");
			}

			for (const auto& status : m_serverResponse["realTimeState"])
			{
				++m_progress; // Progress bar math.
				m_serviceInformation[folder][service] = status;

				if (status.get<std::string>() == "STARTED")
				{
					++m_countStarted;
				}
				else if (status.get<std::string>() == "STOPPED")
				{
					++m_countStopped;
				}
			}
		}
	}
	timeStamp();
}
