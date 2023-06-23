#ifndef CONSTANT_NAMESPACES_HPP
#define CONSTANT_NAMESPACES_HPP

#include "nlohmann/json.hpp"

#include <wx/wx.h>

#include <string>
#include <string_view>

enum MenuID
{
	changeCredentials,
	setMethodSequential,
	setMethodBatch,
	updateFolders,
	quit,
	find,
	copySelection,
	copyAll,
	refresh,
	showAll,
	showStarted,
	showStopped,
	exportStart,
	exportStop,
	start,
	stop,
	about,
};

struct threadState
{
	int progressValue{};
	int progressMax{};
	wxString message{};
	bool working{};
};

namespace Command
{
	inline constexpr std::string_view START{ "STARTED" };
	inline constexpr std::string_view STOP{ "STOPPED" };
	inline constexpr int batchMode{ 0 };
	inline constexpr int sequentialMode{ 1 };
}

namespace JSON
{
	inline const std::string closeBrace{ "\"}," };
	inline const std::string folderName{ "\"folderName\":\""  };
	inline const std::string openBrace{ "{" };
	inline const std::string serviceName{ "\",\"serviceName\":\"" };
	inline const std::string type{ "\",\"type\":\"" };
}

namespace Message
{
	inline const wxString credentialsRequired{
		"Credentials must be valid for the tool to function correctly." };

	inline constexpr std::string_view initialStatus{
		"Use 'Refresh Server Status' in the\n"
		"context menu to update the status display." };
		
	inline const wxString durationWarning{
		"\n\nWarning: This will take a few minutes." };
		
	inline const wxString directoryError{
		"The tool failed to create the \".\\config\" directory.\n"
		"Please try creating the directory manually"
		"in the same directory as the tool." };

	inline const wxString fileError{
		"Unable to generate config file, check your file/folder permissions.\n"
		"Tool requires \".\\config\\config.ini\" to function properly." };

	inline const wxString gathering{ "Gathering current server status." };

	inline const nlohmann::json jsonNothingToDo = nlohmann::json::parse(
		R"({"status": "Services are already at requested state."})");

	inline const wxString noResponse{
		"No response was received from the server, is your url correct?\n" };

	inline const std::string_view serverError{
		"Error communicating with the server.\n"
		"You can change your credentials by going to\n"
		"File->Change Enterprise Portal Credentials." };

	inline const nlohmann::json success = nlohmann::json::parse(
		R"({"status": "success"})");

	inline const wxString tokenError{
		"An error occurred while attempting to acquire a token.\n\n" };

	inline const wxString waiting{
		"Commands sent, awaiting server responses."
		"\nThis can take a few minutes..." };

	inline const wxString waitCommand{
		"Assembling command and sending to server, please wait..." };
		
	inline const wxString waitRefresh{
		"Retrieving status from server, please wait..." };
}

// Option flags for "Find" dialog from wxWidgets. 
namespace SearchMethod
{
	inline constexpr int topToBottom{ 1 };
	inline constexpr int topToBottomMatchWord{ 3 };
	inline constexpr int topToBottomMatchCase{ 5 };
	inline constexpr int topToBottom_matchBoth{ 7 };
	inline constexpr int bottomToTop{ 0 };
	inline constexpr int bottomToTopMatchWord{ 2 };
	inline constexpr int bottomToTopMatchCase{ 4 };
	inline constexpr int bottomToTopMatchBoth{ 6 };
}

namespace ServerItem
{
	inline std::string_view folders{ "folders" };
	inline std::string_view hosted{ "Hosted" };
	inline std::string_view realTimeState{ "realTimeState" };
	inline std::string_view root{ "Root" };
	inline std::string_view services{ "services" };
	inline std::string_view system{ "System" };
	inline std::string_view utilities{ "Utilities" };
}


#endif