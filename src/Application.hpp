#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Constants.hpp"
#include "CustomRichTextCtrl.hpp"
#include "Credentials.hpp"
#include "EnterCredentials.hpp"
#include "Services.hpp"
#include <chrono>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/fdrepdlg.h>
#include <wx/wx.h>
#include <wx/progdlg.h>

// Launch Application.
class Application : public wxApp
{
public:
	virtual bool OnInit();
};

// Main Window Frame.
class Frame : public wxFrame
{
public:
    Frame(const std::string&& title);

private:
	int m_caretPosition;
	Credentials m_credentials;
	wxFindReplaceData m_findSettings;
	std::string m_findString;
	Services m_mapServices;

	// Text box that displays the service state information.
	CustomRichTextCtrl* m_statusText;

	// Display confirmation dialog.
	void areYouSure(const std::string_view command);

	// Loads displayed server status into the clipboard.
	void copyServerStatus();

	// Updates m_statusText with all map services.
	void displayStatusAll();

	// Updates m_statusText with "STARTED" map services..
	void displayStatusStarted();

	// Updates m_statusText with "STOPPED" map services.
	void displayStatusStopped();

	// Displays a window to allow user to enter Enterprise Portal credentials.
	void enterCredentials();

	/*
	Triggers the desired action based on event.GetId()
	@param wxEvent from menu selection.
	*/
	void menuAction(wxCommandEvent& event);

	/* 
	Displays a progress dialog, updates the "status" of
	services in m_serviceInformation, and updates m_statusText.
	 */
	void refreshStatus();

	/*
	Displays a progress dialog and calls mapServices.sendServerCommand
	with the desired command.
	@param Constants::Commands::START or Constants::Commands::STOP
	*/
	void sendCommand(const std::string_view command);

	// Displays a wxFindReplaceDialog - only the "Find" portion.
	void showFindTextPrompt();
};

#endif