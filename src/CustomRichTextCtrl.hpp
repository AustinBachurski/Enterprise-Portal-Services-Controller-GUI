#ifndef CUSTOM_RICH_TEXT_CONTROL_HPP
#define CUSTOM_RICH_TEXT_CONTROL_HPP

#include "Constants.hpp"
#include <string>
#include <wx/fdrepdlg.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/wx.h>

/*
Custom wxRichTextCtrl that prevents the carat from being shown by
skipping the EVT_SET_FOCUS event.
*/
class CustomRichTextCtrl : public wxRichTextCtrl
{
public:
	/*
	@param parent Pointer to the parent window, usually 'this'.
	@param style wxWidgets style - see wxRichTextCtrl docs.
	*/
	CustomRichTextCtrl(wxWindow* parent, long style);

	void findText(wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);

private:
	int m_caretPosition;
	std::string m_findString;

	// Discards the wxFocusEvent.
	void discard(wxFocusEvent& event);
	
	/*
	Display modal window that searched text was not found.
	*/
	void findNotFound(wxFindReplaceDialog* findTextWindow,
		const std::string& searchString,
		const int textSize) const;

	// @return True if character is not a A-Z or a-z.
	bool isNotCharacter(char character) const;


	wxDECLARE_EVENT_TABLE();
};

#endif