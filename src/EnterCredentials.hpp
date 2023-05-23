#ifndef ENTERCREDENTIALS_HPP
#define ENTERCREDENTIALS_HPP

#include "Credentials.hpp"
#include <string>
#include <wx/wx.h>

// Forward declaration.
class Credentials;

// UI dialog that prompts user for Enterprise Portal credentials.
class EnterCredentials : public wxDialog
{
public:
	// Displays dialog window.
	EnterCredentials(wxWindow* parent, Credentials& credentials);

private:
	// Captures values from the dialog window
	// and sends them to the Credentials class.
	void captureCredentials(wxCommandEvent& event, Credentials& credentials);

	/*
	Used to prevent the 'required' popup window from
	showing if 'OK' was pressed.
	@return True if 'OK' was pressed in the EnterCredentials dialog window.
	*/
	bool m_okPressed;
	wxTextCtrl* m_urlEntryBox;
	wxTextCtrl* m_usernameEntryBox;
	wxTextCtrl* m_passwordEntryBox;
};

#endif