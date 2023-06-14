#ifndef CUSTOM_RICH_TEXT_CONTROL_HPP
#define CUSTOM_RICH_TEXT_CONTROL_HPP

#include "Constants.hpp"
#include <string>
#include <wx/fdrepdlg.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/wx.h>

/*
Custom wxRichTextCtrl that prevents the carat from being shown by
skipping the EVT_SET_FOCUS event, also adds a "findText" method.
*/
class CustomRichTextCtrl : public wxRichTextCtrl
{
public:
	CustomRichTextCtrl(wxWindow* parent, long style);

	void findText(wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);

private:
	int m_caretPosition{ 0 };
	std::string m_findString;
	int m_lastFlags{ 0 };
	std::string m_text;

	void findNotFound(
		wxFindReplaceDialog* findTextWindow,
		const std::string& searchString) const;
	bool isNotCharacter(char character) const;
	void preventFocus(wxFocusEvent& event);
	void searchTopToBottom(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchTopToBottomMatchCase(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchTopToBottomMatchWord(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchTopToBottomMatchBoth(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchBottomToTop(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchBottomToTopMatchCase(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchBottomToTopMatchWord(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);
	void searchBottomToTopMatchBoth(
		wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif