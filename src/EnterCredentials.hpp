#ifndef ENTERCREDENTIALS_HPP
#define ENTERCREDENTIALS_HPP

#include "Configuration.hpp"
#include <string>
#include <wx/wx.h>

class Configuration;

class EnterCredentials : public wxDialog
{
public:
	EnterCredentials(wxWindow* parent, Configuration& configuration);

private:
	void captureCredentials(
		wxCommandEvent& event, Configuration& configuration);

	bool m_okPressed{ false };
	wxTextCtrl* m_urlEntryBox{ nullptr };
	wxTextCtrl* m_usernameEntryBox{ nullptr };
	wxTextCtrl* m_passwordEntryBox{ nullptr };
};

#endif
