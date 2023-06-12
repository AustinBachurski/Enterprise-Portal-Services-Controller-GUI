#include "AboutWindow.hpp"

AboutWindow::AboutWindow(wxWindow* parent)
	: wxFrame(parent, wxID_ANY, "About", wxDefaultPosition, wxSize(600, 400))
{
	CenterOnParent();
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* name = new wxStaticText(panel, wxID_ANY,
		"Enterprise Portal Services Controller");
	wxFont nameFont = name->GetFont();
	nameFont.SetPointSize(20);
	name->SetFont(nameFont);

	wxStaticText* version = new wxStaticText(panel, wxID_ANY,
		"Version: *Place Holder* - Built June 12, 2023"); //TODO - Set Version
	wxStaticText* author = new wxStaticText(panel, wxID_ANY,
		"Author: Austin Bachurski");

	wxHyperlinkCtrl * git = new wxHyperlinkCtrl(panel, wxID_ANY, 
		"Project GitHub",
		"https://github.com/AustinBachurski/Enterprise-Portal-Services-Controller-GUI/tree/main");

	wxStaticText* openSource = new wxStaticText(panel, wxID_ANY,
		"Powered by open-source software:");
	wxFont openSourceFont = openSource->GetFont();
	openSourceFont.SetPointSize(14);
	openSource->SetFont(openSourceFont);

	wxHyperlinkCtrl* cpp = new wxHyperlinkCtrl(panel, wxID_ANY,
		"C++ Programming Language", "https://isocpp.org/");
	wxHyperlinkCtrl* cpr = new wxHyperlinkCtrl(panel, wxID_ANY,
		"C++ Requests: Curl for People", "https://github.com/libcpr/cpr");
	wxHyperlinkCtrl* json = new wxHyperlinkCtrl(panel, wxID_ANY,
		"JSON for Modern C++", "https://github.com/nlohmann/json");
	wxHyperlinkCtrl* wxWidgets = new wxHyperlinkCtrl(panel, wxID_ANY,
		"wxWidgets", "https://www.wxwidgets.org/");

	wxStaticText* noaffil = new wxStaticText(panel, wxID_ANY,
		"Not affiliated with, associated with, authorized by, endorsed by,"
		" or in any way officially connected with Esri.\n"
		"Esri, as well as related names, marks, emblems and images are"
		" registered trademarks of their respective owners.",
		wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	wxFont noaffilFont = noaffil->GetFont();
	noaffilFont.SetPointSize(8);
	noaffil->SetFont(noaffilFont);

	sizer->Add(name, 1, wxALIGN_CENTER_HORIZONTAL);
	sizer->Add(version, 1, wxALIGN_CENTER_HORIZONTAL);
	sizer->Add(author, 1, wxALIGN_CENTER_HORIZONTAL);
	sizer->Add(git, 1, wxALIGN_CENTER_HORIZONTAL);
	sizer->AddSpacer(5);
	sizer->Add(openSource, 1, wxLEFT, 10);
	sizer->Add(cpp, 1, wxLEFT, 25);
	sizer->Add(cpr, 1, wxLEFT, 25);
	sizer->Add(json, 1, wxLEFT, 25);
	sizer->Add(wxWidgets, 1, wxLEFT, 25);
	sizer->Add(noaffil, 1, wxALIGN_CENTER_HORIZONTAL);

	panel->SetSizerAndFit(sizer);
}