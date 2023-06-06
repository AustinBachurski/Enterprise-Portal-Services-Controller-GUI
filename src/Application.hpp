#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Constants.hpp"
#include "CustomRichTextCtrl.hpp"
#include "Configuration.hpp"
#include "EnterCredentials.hpp"
#include "PortalServerControls.hpp"
#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/fdrepdlg.h>
#include <wx/wx.h>
#include <wx/progdlg.h>

class Application : public wxApp
{
public:
	bool OnInit() override;
};

class Frame : public wxFrame
{
public:
    Frame(const std::string&& title);
	~Frame();

private:
	Configuration m_configuration;
	// Find text from top to bottom by default.
	wxFindReplaceData m_findOptions{ wxFR_DOWN };
	std::unique_ptr<PortalServerControls> m_portalServerControl;
	int m_serverCommandMethod;
	CustomRichTextCtrl* m_statusText{ nullptr };
	wxMenuItem* m_menuModeBatch{ nullptr };
	wxMenuItem* m_menuModeSequential{ nullptr };

	void areYouSure(const std::string_view command);
	void copyServerStatus();
	void displayStatusAll();
	void displayStatusStarted();
	void displayStatusStopped();
	void displayWelcomeMessage();
	void enterCredentials();
	void exportJson(MenuID mode);
	void keyboardShortcuts(wxKeyEvent& event);
	void menuAction(wxCommandEvent& event);
	void refreshStatus();
	void sendBatchCommand(const std::string_view command);
	void sendSequentialCommand(const std::string_view command);
	void showFindTextPrompt();
	void updateCredentials();
};

#endif
