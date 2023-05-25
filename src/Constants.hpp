#ifndef NAMESPACE_CONSTANTS_HPP
#define NAMESPACE_CONSTANTS_HPP

#include "nlohmann/json.hpp"
#include <wx/wx.h>
#include <string>
#include <string_view>

// Constant values.
namespace Constants
{
	// Server commands.
	namespace Commands
	{
		// STARTED
		inline constexpr std::string_view START{ "STARTED" };
		
		// STOPPED
		inline constexpr std::string_view STOP{ "STOPPED" };
	}

	namespace JSON
	{
		// "},
		inline const std::string closeBrace{ "\"}," };

		// "foldername":"
		inline const std::string folderName{ "\"folderName\":\""  };

		// {
		inline const std::string openBrace{ "{" };

		// ","serviceName":"
		inline const std::string serviceName{ "\",\"serviceName\":\"" };

		// ","type":"
		inline const std::string type{ "\",\"type\":\"" };
	}

	// wxWidgets MenuBar ids.
	namespace MenuID
	{
		inline constexpr int changeCredentials = 1;
		inline constexpr int quit = 2;
		inline constexpr int find = 3;
		inline constexpr int copyAll = 4;
		inline constexpr int copySelection = 5;
		inline constexpr int refresh = 6;
		inline constexpr int showAll = 7;
		inline constexpr int showStarted = 8;
		inline constexpr int showStopped = 9;
		inline constexpr int start = 10;
		inline constexpr int stop = 11;
	}

	// Modal window text. 
	namespace Messages
	{
		/*
		Credentials were not found, please restart the tool and
		 enter your Enterprise Portal credentials whe prompted.
		 Tool functionality has been disabled for this session.
		*/
		inline const std::string_view credentialsNotFound{
			"Credentials were not found, please restart the tool\nand "
			"enter your Enterprise Portal credentials when prompted.\n"
			"Tool functionality has been disabled for this session." };

		// START all services on the server? 
		inline const wxString confirmStart{
			"START all services on the server?" };

		// STOP all services on the server? 
		inline const wxString confirmStop{
			"STOP all services on the server?" };

		/*
		'Refresh Server Status' in the
		context menu to update the status display.
		*/
		inline constexpr std::string_view initialStatus{
			"Use 'Refresh Server Status' in the\n"
			"context menu to update the status display." };
		
		// Warning: This will take a few minutes. 
		inline const wxString durationWarning{
			"\n\nWarning: This will take a few minutes." };
		
		/*
		The tool failed to create the .\config directory.
		Please try creating the directory manually in the same
		directory as the tool.
		*/
		inline const wxString directoryError{
			"The tool failed to create the \".\\config\" directory.\n"
			"Please try creating the directory manually"
			"in the same directory as the tool.",
		};

		/*
		Unable to generate config file, check your file/filder permissions.
		Tool requires ".\config\config.ini" to function properly.
		*/
		inline const wxString fileError{
			"Unable to generate config file, check your file/folder permissions.\n"
			"Tool requires \".\\config\\config.ini\" to function properly."
		};

		// {"status": "Services are already in requested state."} 
		inline const nlohmann::json jsonError = nlohmann::json::parse(
			R"({"status": "Error"})");

		/*
		No response was received from the server, is your url correct?\n
		*/
		inline const wxString noResponse{
			"No response was received from the server, is your url correct?\n"
		};

		// {"status": "Services are already in requested state."} 
		inline const nlohmann::json nothingToDo = nlohmann::json::parse(
			R"({"status": "Services are already in requested state."})");

		// Refreshing server status. 
		inline const wxString refreshing{ "Refreshing server status." };

		/*
		No response received from server.
		Please verisy your Enterprise Portal Credentials and restart the tool.
		Tool functionality has been disabled.
		*/
		inline const std::string_view serverError{
			"No response recieved from server.\nPlease "
			"verify your Enterprise Portal Credentials and restart the tool.\n"
			"Tool functionality has been disabled for this session."
		};

		// An error occurred while attempting to aquire a token: 
		inline const wxString tokenError{
			"An error occurred while attempting ao aquire a token:\n" };
		
		// Sending 'Start All' command to server. 
		inline const wxString starting{
			"Sending 'Start All' command to server." };
		
		// Sending 'Stop All' command to server. 
		inline const wxString stopping{
			"Sending 'Stop All' command to server." };
		
		// Assembling commands and sending to server, please wait... 
		inline const wxString waitCommand{
			"Assembling commands and sending to server, please wait..." };
		
		// Retrieving status from server, please wait... 
		inline const wxString waitRefresh{
			"Retrieving status from server, please wait..." };
	}

	// Option flags for "Find" dialog. 
	namespace SearchMethod
	{
		inline constexpr int topToBottom{ 1 };
		inline constexpr int matchWord_topToBottom{ 3 };
		inline constexpr int matchCase_topToBottom{ 5 };
		inline constexpr int matchBoth_topToBottom{ 7 };

		inline constexpr int bottomToTop{ 0 };
		inline constexpr int matchWord_bottomToTop{ 2 };
		inline constexpr int matchCase_bottomToTop{ 4 };
		inline constexpr int matchBoth_bottomToTop{ 6 };
	}

	// Words, yep yep yep.
	namespace Words
	{
		// folders
		inline std::string_view folders{ "folders" };

		// services
		inline std::string_view services{ "services" };
	}
}

#endif
