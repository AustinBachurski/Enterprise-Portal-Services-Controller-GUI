#ifndef ENTERCREDENTIALS_HPP
#define ENTERCREDENTIALS_HPP

#include "Configuration.hpp"

#include <wx/wx.h>

#include <string>

class Configuration;

class EnterCredentials : public wxDialog
{
public:
	EnterCredentials(wxWindow* parent, Configuration& configuration);

private:
	bool m_okPressed{ false };
	wxTextCtrl* m_urlEntryBox{ nullptr };
	wxTextCtrl* m_usernameEntryBox{ nullptr };
	wxTextCtrl* m_passwordEntryBox{ nullptr };

	void captureCredentials(wxCommandEvent& event, Configuration& configuration);
};

#endif