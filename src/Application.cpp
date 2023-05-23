#include "Application.hpp"

wxIMPLEMENT_APP(Application); // int main()

// Launch Application.
bool Application::OnInit()
{
	Frame* frame = new Frame("Enterprise Portal Services Controller");
	frame->SetClientSize(400, 500);
	frame->Show();

	return true;
}

// Main Window Frame.
Frame::Frame(const std::string&& title) :
	wxFrame(nullptr, wxID_ANY, title, wxPoint(wxDefaultPosition), wxSize(wxDefaultSize), wxDEFAULT_FRAME_STYLE),
	m_caretPosition{},
	m_credentials{},
	m_findSettings{ wxFR_DOWN }, // Find text from top to bottom by default.
	m_findString{},
	m_mapServices{ m_credentials.getUsername(), m_credentials.getPassword(), m_credentials.getPortal() },
	m_statusText{ nullptr }
{
	/*************************************************************************
	 * BEGIN MENU BAR                                                        *
	 * ***********************************************************************/

	wxMenuBar* menuBar = new wxMenuBar();

	// File Menu.
	wxMenu* fileMenu = new wxMenu();
		
		// Enter Credentials.
		auto credentials = fileMenu->Append(
			Constants::MenuID::changeCredentials,
			"Change Enterprise Portal Credentials");
		credentials->SetBitmap(wxArtProvider::GetBitmap(
			wxART_INFORMATION, wxART_MENU));

		fileMenu->AppendSeparator();

		// Quit.
		auto quit = fileMenu->Append(
			Constants::MenuID::quit,
			"Quit");
		quit->SetBitmap(wxArtProvider::GetBitmap(
			wxART_QUIT, wxART_MENU));

	// Status Menu.
	wxMenu* statusMenu = new wxMenu();

		// Find.
		auto find = statusMenu->Append(
			Constants::MenuID::find,
			"Find");
		find->SetBitmap(wxArtProvider::GetBitmap(
			wxART_FIND, wxART_MENU));

		// Copy Selection.
		auto copySelection = statusMenu->Append(
			Constants::MenuID::copySelection,
			"Copy Selection");
		copySelection->SetBitmap(wxArtProvider::GetBitmap(
			wxART_COPY, wxART_MENU));

		// Copy All
		auto copyAll = statusMenu->Append(
			Constants::MenuID::copyAll,
			"Copy Server Status");
		copyAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_COPY, wxART_MENU));

		statusMenu->AppendSeparator();

		// Refresh status.
		auto refresh = statusMenu->Append(
			Constants::MenuID::refresh,
			"Refresh Server Status");
		refresh->SetBitmap(wxArtProvider::GetBitmap(
			wxART_REFRESH, wxART_MENU));

		statusMenu->AppendSeparator();

		// Status - Show all.
		auto showAll = statusMenu->Append(
			Constants::MenuID::showAll,
			"Show All Services");
		showAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_EXECUTABLE_FILE, wxART_MENU));

		// Status - Show started.
		auto showStarted = statusMenu->Append(
			Constants::MenuID::showStarted,
			"Show Only Started Services");
		showStarted->SetBitmap(wxArtProvider::GetBitmap(
			wxART_TICK_MARK, wxART_MENU));

		// Status - Show stopped.
		auto showStopped = statusMenu->Append(
			Constants::MenuID::showStopped,
			"Show Only Stopped Services");
		showStopped->SetBitmap(wxArtProvider::GetBitmap(
			wxART_CROSS_MARK, wxART_MENU));

	// Server Controls Menu.
	wxMenu* controlsMenu = new wxMenu();

		// Start all.
		auto startAll = controlsMenu->Append(
			Constants::MenuID::start,
			"Start All Map Services");
		startAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_TICK_MARK, wxART_MENU));

		controlsMenu->AppendSeparator();

		// Stop all.
		auto stopAll = controlsMenu->Append(
			Constants::MenuID::stop,
			"Stop All Map Services");
		stopAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_CROSS_MARK, wxART_MENU));

	// Append menus to menu bar.
	menuBar->Append(fileMenu, "File");
	menuBar->Append(statusMenu, "Server Status");
	menuBar->Append(controlsMenu, "Server Controls");

	// Enable menu bar.
	SetMenuBar(menuBar);
	menuBar->Bind(wxEVT_MENU, &Frame::menuAction, this);

	/*************************************************************************
	 * END MENU BAR                                                          *
	 * ***********************************************************************/


	/*************************************************************************
	 * BEGIN TEXT BOX                                                        *
	 * ***********************************************************************/

	m_statusText = new CustomRichTextCtrl(this, wxRE_READONLY);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(m_statusText, 1, wxEXPAND | wxALL, 5);
	SetSizer(sizer);

	// Display welcome message.
	m_statusText->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	m_statusText->BeginBold();
	m_statusText->BeginTextColour({ 0, 0, 255 });
	m_statusText->WriteText("Enterprise Portal: " + m_credentials.getPortal());
	m_statusText->EndBold();
	m_statusText->EndTextColour();
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
	m_statusText->BeginBold();
	m_statusText->BeginTextColour({ 21, 67, 96 });
	m_statusText->WriteText(m_mapServices.getTimeStamp());
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();
	m_statusText->EndAlignment();

	// Right-click context menu.
	wxMenu* contextStatusMenu = new wxMenu();

		// Find
		auto contextfind = contextStatusMenu->Append(
			Constants::MenuID::find,
			"Find");
		contextfind->SetBitmap(wxArtProvider::GetBitmap(
			wxART_FIND, wxART_MENU));

		// Copy Selection
		auto contextcopySelection = contextStatusMenu->Append(
			Constants::MenuID::copySelection,
			"Copy Selection");
		contextcopySelection->SetBitmap(wxArtProvider::GetBitmap(
			wxART_COPY, wxART_MENU));

		// Copy All
		auto contextcopyAll = contextStatusMenu->Append(
			Constants::MenuID::copyAll,
			"Copy Complete Server Status");
		contextcopyAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_COPY, wxART_MENU));

		contextStatusMenu->AppendSeparator();

		// Refresh status.
		auto contextRefresh = contextStatusMenu->Append(
			Constants::MenuID::refresh,
			"Refresh Server Status");
		contextRefresh->SetBitmap(wxArtProvider::GetBitmap(
			wxART_REFRESH, wxART_MENU));

		contextStatusMenu->AppendSeparator();

		// Status - Show all.
		auto contextShowAll = contextStatusMenu->Append(
			Constants::MenuID::showAll,
			"Show All Services");
		contextShowAll->SetBitmap(wxArtProvider::GetBitmap(
			wxART_EXECUTABLE_FILE, wxART_MENU));

		// Status - Show started.
		auto contextShowStarted = contextStatusMenu->Append(
			Constants::MenuID::showStarted,
			"Show Only Started Services");
		contextShowStarted->SetBitmap(wxArtProvider::GetBitmap(
			wxART_TICK_MARK, wxART_MENU));

		// Status - Show stopped.
		auto contextShowStopped = contextStatusMenu->Append(
			Constants::MenuID::showStopped,
			"Show Only Stopped Services");
		contextShowStopped->SetBitmap(wxArtProvider::GetBitmap(
			wxART_CROSS_MARK, wxART_MENU));

	// Enable Right-click context menu.
	m_statusText->SetContextMenu(contextStatusMenu);
	contextStatusMenu->Bind(wxEVT_MENU, &Frame::menuAction, this);

	/*************************************************************************
	 * END TEXT BOX                                                          *
	 * ***********************************************************************/

	// Display enter credentials window if credentials were not found in
	// "/config/config.ini".
	if (!m_credentials.credentialsAquired())
	{
		enterCredentials();
	}
}

void Frame::areYouSure(const std::string_view command)
{
	// Shorten line length with intent to improve terenary readability.
	using namespace Constants::Messages;

	wxMessageDialog* youSure = new wxMessageDialog(
		this,
		command == Constants::Commands::START ? confirmStart : confirmStop
		+ durationWarning,
		"Are You Sure?",
		wxYES_NO);

	if (youSure->ShowModal() == wxID_YES)
	{
		sendCommand(command);
	}
}

void Frame::copyServerStatus()
{
	m_statusText->SelectAll();
	m_statusText->Copy();
	m_statusText->SelectNone();
}

void Frame::displayStatusAll()
{
	m_statusText->Clear();
	m_statusText->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	m_statusText->BeginBold();
	m_statusText->BeginTextColour({ 0, 0, 255 });
	m_statusText->WriteText("Enterprise Portal: " + m_credentials.getPortal());
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->BeginTextColour({ 21, 67, 96 });
	m_statusText->WriteText(m_mapServices.getTimeStamp());
	m_statusText->Newline();
	m_statusText->WriteText("Total Services: " + m_mapServices.getCountTotalAsString());
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStartedAsString() + " - Started.");
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStoppedAsString() + " - Stopped.");
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();
	m_statusText->BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
	m_statusText->BeginBold();
	m_statusText->WriteText("*** Displaying All Services***");
	m_statusText->EndAlignment();
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndBold();

	for (const auto& [folder, services] : m_mapServices.m_serviceInformation)
	{
		m_statusText->BeginBold();
		m_statusText->BeginTextColour({ 23, 32, 42 });
		m_statusText->WriteText(folder + ":");
		m_statusText->Newline();
		m_statusText->EndBold();
		m_statusText->EndTextColour();
		for (const auto& [service, status] : services)
		{
			if (status == "STARTED") // Green Text if STARTED.
			{
				m_statusText->BeginTextColour({ 0, 139, 0 });
				m_statusText->WriteText("\t" + service + " - " + status);
				m_statusText->EndTextColour();
			}
			else // Red Text if STOPPED.
			{
				m_statusText->BeginTextColour({ 255, 0, 0 });
				m_statusText->WriteText("\t" + service + " - " + status);
				m_statusText->EndTextColour();
			}
			m_statusText->Newline();
		}
		m_statusText->Newline();
	}
	m_statusText->Update();
}

void Frame::displayStatusStarted()
{
	m_statusText->Clear();
	m_statusText->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	m_statusText->BeginBold();
	m_statusText->BeginTextColour({ 0, 0, 255 });
	m_statusText->WriteText("Enterprise Portal: " + m_credentials.getPortal());
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->BeginTextColour({ 21, 67, 96 });
	m_statusText->WriteText(m_mapServices.getTimeStamp());
	m_statusText->Newline();
	m_statusText->WriteText("Total Services: " + m_mapServices.getCountTotalAsString());
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStartedAsString() + " - Started.");
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStoppedAsString() + " - Stopped.");
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();
	m_statusText->BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
	m_statusText->BeginBold();
	m_statusText->WriteText("*** Displaying Started Services***");
	m_statusText->EndAlignment();
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();

	for (const auto& [folder, services] : m_mapServices.m_serviceInformation)
	{
		m_statusText->BeginBold();
		m_statusText->BeginTextColour({ 23, 32, 42 });
		m_statusText->WriteText(folder + ":");
		m_statusText->Newline();
		m_statusText->EndBold();
		m_statusText->EndTextColour();
		for (const auto& [service, status] : services)
		{
			if (status == "STARTED") // Green Text if STARTED.
			{
				m_statusText->BeginTextColour({ 0, 139, 0 });
				m_statusText->WriteText("\t" + service + " - " + status);
				m_statusText->EndTextColour();
				m_statusText->Newline();
			}
		}
		m_statusText->Newline();
	}
	m_statusText->Update();
}

void Frame::displayStatusStopped()
{
	m_statusText->Clear();
	m_statusText->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	m_statusText->BeginBold();
	m_statusText->BeginTextColour({ 0, 0, 255 });
	m_statusText->WriteText("Enterprise Portal: " + m_credentials.getPortal());
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->BeginTextColour({ 21, 67, 96 });
	m_statusText->WriteText(m_mapServices.getTimeStamp());
	m_statusText->Newline();
	m_statusText->WriteText("Total Services: " + m_mapServices.getCountTotalAsString());
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStartedAsString() + " - Started.");
	m_statusText->Newline();
	m_statusText->WriteText(m_mapServices.getCountStoppedAsString() + " - Stopped.");
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();
	m_statusText->BeginAlignment(wxTEXT_ALIGNMENT_CENTER);
	m_statusText->BeginBold();
	m_statusText->WriteText("*** Displaying Stopped Services***");
	m_statusText->EndAlignment();
	m_statusText->Newline();
	m_statusText->Newline();
	m_statusText->EndTextColour();
	m_statusText->EndBold();

	for (const auto& [folder, services] : m_mapServices.m_serviceInformation)
	{
		m_statusText->BeginBold();
		m_statusText->BeginTextColour({ 23, 32, 42 });
		m_statusText->WriteText(folder + ":");
		m_statusText->Newline();
		m_statusText->EndBold();
		m_statusText->EndTextColour();
		for (const auto& [service, status] : services)
		{
			if (status == "STOPPED") // Red Text if STOPPED.
			{
				m_statusText->BeginTextColour({ 255, 0, 0 });
				m_statusText->WriteText("\t" + service + " - " + status);
				m_statusText->EndTextColour();
				m_statusText->Newline();
			}
		}
		m_statusText->Newline();
	}
	m_statusText->Update();
}

void Frame::enterCredentials()
{
	EnterCredentials* credentialsWindow = new EnterCredentials(this, m_credentials);
	credentialsWindow->ShowModal();
}

void Frame::menuAction(wxCommandEvent& event)
{
	if (!m_mapServices.functionalityDisabled())
	{
		switch (event.GetId())
		{
		case Constants::MenuID::changeCredentials:
		{
			enterCredentials();

			wxMessageDialog* restartPrompt = new wxMessageDialog(this,
				"Tool must be restarted to apply credential changes.",
				"Restart to Apply", wxOK | wxICON_INFORMATION);
			restartPrompt->ShowModal();

			break;
		}

		case Constants::MenuID::quit:
			Close();
			break;

		case Constants::MenuID::find:
			showFindTextPrompt();
			break;

		case Constants::MenuID::copyAll:
			copyServerStatus();
			break;

		case Constants::MenuID::copySelection:
			m_statusText->Copy();
			break;

		case Constants::MenuID::refresh:
			refreshStatus();
			displayStatusAll();
			break;

		case Constants::MenuID::showAll:
			displayStatusAll();
			break;

		case Constants::MenuID::showStarted:
			displayStatusStarted();
			break;

		case Constants::MenuID::showStopped:
			displayStatusStopped();
			break;

		case Constants::MenuID::start:
			areYouSure(Constants::Commands::START);
			break;
		case Constants::MenuID::stop:
			areYouSure(Constants::Commands::STOP);
			break;
		default:
			break;
		}
	}
	else
	{
		if (event.GetId() == Constants::MenuID::quit)
		{
			Close();
		}
		else if (event.GetId() == Constants::MenuID::changeCredentials)
		{
			enterCredentials();
		}
	}
}

void Frame::refreshStatus()
{
	std::thread update(&Services::updateStatus, &m_mapServices);
	update.detach();

	wxGenericProgressDialog progressDisplay(
		Constants::Messages::refreshing,
		Constants::Messages::waitRefresh,
		m_mapServices.getCountTotal(),
		this,
		wxPD_APP_MODAL | wxPD_AUTO_HIDE);
	progressDisplay.Show();

	int progress{ 0 };
	m_mapServices.resetProgressCounter();
	while (progress != m_mapServices.getCountTotal())
	{
		progress = m_mapServices.getProgress();
		progressDisplay.Update(progress);
	}
	displayStatusAll();
}

void Frame::sendCommand(const std::string_view command)
{
	std::chrono::time_point start{ std::chrono::system_clock::now() };

	std::promise<nlohmann::json> promise;
	std::shared_future<nlohmann::json> future = promise.get_future();
	std::thread sendIt(&Services::sendServerCommand, &m_mapServices, std::move(promise), command);
	sendIt.detach();

	wxGenericProgressDialog progress(
		command == Constants::Commands::START ? Constants::Messages::starting : Constants::Messages::stopping,
		Constants::Messages::waitCommand,
		10,
		this,
		wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);

	progress.Show();

	while (future.wait_for(std::chrono::milliseconds(50)) != std::future_status::ready)
	{
		progress.Pulse();
	}
	progress.Hide();

	nlohmann::json result = future.get();
	std::chrono::time_point finish{ std::chrono::system_clock::now() };
	auto et = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
	displayStatusAll();

	wxMessageDialog* done = new wxMessageDialog(this,
		"Operation completed in " + std::to_string(et.count()) + " seconds.\n\n"
		+ "Server Response: " + result["status"].get<std::string>(),
		"Operation Complete", wxOK | wxICON_INFORMATION);
	done->ShowModal();
}

void Frame::showFindTextPrompt()
{
	wxFindReplaceDialog* findTextWindow = new wxFindReplaceDialog(this, &m_findSettings, "Find");

	// First click of "Find Next".
	findTextWindow->Bind(wxEVT_FIND, [this, findTextWindow](wxFindDialogEvent& event)
		{
			// Pass pointer to findTextWindow so that it can be
			// passed to findNotFound() and blocked when needed.
			m_statusText->findText(findTextWindow, event);
		});

	// Additional clicks of "Find Next".
	findTextWindow->Bind(wxEVT_FIND_NEXT, [this, findTextWindow](wxFindDialogEvent& event)
		{
			// Pass pointer to findTextWindow so that it can be
			// passed to findNotFound() and blocked when needed.
			m_statusText->findText(findTextWindow, event);
		});

	findTextWindow->Show();
}