#include "CustomRichTextCtrl.hpp"

CustomRichTextCtrl::CustomRichTextCtrl(wxWindow* parent, long style)
	: wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
		  wxDefaultSize, style, wxDefaultValidator, wxEmptyString)
{}

void CustomRichTextCtrl::findNotFound(wxFindReplaceDialog* findTextWindow,
	const std::string& searchString) const
{
	if (this->HasSelection())
	{
		wxMessageDialog* endReached = new wxMessageDialog(findTextWindow,
			"Search reached end.",
			"Not Found", wxOK | wxICON_ERROR);
		endReached->ShowModal();
	}
	else
	{
		wxMessageDialog* notFound = new wxMessageDialog(findTextWindow,
			"Can't find text: \"" + searchString + "\"",
			"Not Found", wxOK | wxICON_ERROR);
		notFound->ShowModal();
	}
}

void CustomRichTextCtrl::findText(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	switch (event.GetFlags())
	{
	case SearchMethod::topToBottom:
		searchTopToBottom(findTextWindow, event);
		return;

	case SearchMethod::topToBottom_matchBoth:
		searchTopToBottomMatchBoth(findTextWindow, event);
		return;

	case SearchMethod::topToBottomMatchCase:
		searchTopToBottomMatchCase(findTextWindow, event);
		return;

	case SearchMethod::topToBottomMatchWord:
		searchTopToBottomMatchWord(findTextWindow, event);
		return;

	case SearchMethod::bottomToTop:
		searchBottomToTop(findTextWindow, event);
		return;

	case SearchMethod::bottomToTopMatchBoth:
		searchTopToBottomMatchBoth(findTextWindow, event);
		return;

	case SearchMethod::bottomToTopMatchCase:
		searchBottomToTopMatchCase(findTextWindow, event);
		return;

	case SearchMethod::bottomToTopMatchWord:
		searchBottomToTopMatchWord(findTextWindow, event);
		return;

	default:
		return;
	}
}

bool CustomRichTextCtrl::isNotCharacter(char character) const
{
	return character < 'A' || character > 'z';
}

void CustomRichTextCtrl::preventFocus(wxFocusEvent& event)
{
	event.Skip(false);
}

void CustomRichTextCtrl::searchTopToBottom(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = 0;
		m_findString = event.GetFindString().Lower();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue().Lower();
	}
	if (m_text.find(m_findString, m_caretPosition) != std::string::npos)
	{
		m_caretPosition = m_text.find(m_findString, m_caretPosition);
		this->SetCaretPosition(m_caretPosition);
		this->ShowPosition(m_caretPosition);
		this->SetSelection(
			m_caretPosition, m_caretPosition + m_findString.size());

		// Previous position must be incremented in order
		// to skip the word on the next search iteration.
		++m_caretPosition;
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchTopToBottomMatchBoth(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = 0;
		m_findString = event.GetFindString();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue();
	}

	if (m_text.find(m_findString, m_caretPosition) != std::string::npos)
	{
		if (m_text.find(m_findString, m_caretPosition) == 0)
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) + m_findString.size())))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else if (m_text.find(
			m_findString, m_caretPosition + m_findString.size())
			>= m_text.size())
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) - 1)))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) - 1))
				&& isNotCharacter(m_text.at(m_text.find(
					m_findString, m_caretPosition) + m_findString.size())))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchTopToBottomMatchCase(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = 0;
		m_findString = event.GetFindString();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue();
	}
	if (m_text.find(m_findString, m_caretPosition) != std::string::npos)
	{
		m_caretPosition = m_text.find(m_findString, m_caretPosition);
		this->SetCaretPosition(m_caretPosition);
		this->ShowPosition(m_caretPosition);
		this->SetSelection(
			m_caretPosition, m_caretPosition + m_findString.size());

		// Previous position must be incremented in order
		// to skip the word on the next search iteration.
		++m_caretPosition;
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchTopToBottomMatchWord(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = 0;
		m_findString = event.GetFindString().Lower();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue().Lower();
	}

	if (m_text.find(m_findString, m_caretPosition) != std::string::npos)
	{
		if (m_text.find(m_findString, m_caretPosition) == 0)
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) + m_findString.size())))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else if (m_text.find(m_findString, m_caretPosition
			+ m_findString.size()) >= m_text.size())
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) - 1)))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else
		{
			if (isNotCharacter(m_text.at(m_text.find(
				m_findString, m_caretPosition) - 1))
				&& isNotCharacter(m_text.at(m_text.find(
					m_findString, m_caretPosition) + m_findString.size())))
			{
				m_caretPosition =
					m_text.find(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be incremented in order
				// to skip the word on the next search iteration.
				++m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchBottomToTop(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = m_text.size();
		m_findString = event.GetFindString().Lower();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue().Lower();
	}

	if (m_caretPosition < 0)
	{
		findNotFound(findTextWindow, m_findString);
	}
	else if (m_text.rfind(m_findString, m_caretPosition)
		!= std::string::npos)
	{
		m_caretPosition = m_text.rfind(m_findString, m_caretPosition);
		this->SetCaretPosition(m_caretPosition);
		this->ShowPosition(m_caretPosition);
		this->SetSelection(
			m_caretPosition, m_caretPosition + m_findString.size());

		// Previous position must be decremented in order
		// to skip the word on the next search iteration.
		--m_caretPosition;
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchBottomToTopMatchBoth(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = m_text.size();
		m_findString = event.GetFindString();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue();
	}

	if (m_caretPosition < 0)
	{
		findNotFound(findTextWindow, m_findString);
	}
	else if (m_text.rfind(
		m_findString, m_caretPosition) != std::string::npos)
	{
		if (m_text.rfind(m_findString, m_caretPosition) == 0)
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) + m_findString.size() - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else if (m_text.rfind(
			m_findString, m_caretPosition + m_findString.size())
			>= m_text.size())
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) - 1))
				&& isNotCharacter(m_text.at(m_text.rfind(
					m_findString, m_caretPosition)
					+ m_findString.size() - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchBottomToTopMatchCase(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND
		|| event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = m_text.size();
		m_findString = event.GetFindString();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue();
	}

	if (m_caretPosition < 0)
	{
		findNotFound(findTextWindow, m_findString);
	}
	else if (m_text.rfind(
		m_findString, m_caretPosition) != std::string::npos)
	{
		m_caretPosition = m_text.rfind(m_findString, m_caretPosition);
		this->SetCaretPosition(m_caretPosition);
		this->ShowPosition(m_caretPosition);
		this->SetSelection(
			m_caretPosition, m_caretPosition + m_findString.size());

		// Previous position must be decremented in order
		// to skip the word on the next search iteration.
		--m_caretPosition;
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

void CustomRichTextCtrl::searchBottomToTopMatchWord(
	wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	// Reset caret position on wxEVT_FIND as opposed to wxEVT_FIND_NEXT,
	// or if find method changes.
	if (event.GetEventType() == wxEVT_FIND || event.GetFlags() != m_lastFlags)
	{
		this->SelectNone();
		m_caretPosition = m_text.size();
		m_findString = event.GetFindString().Lower();
		m_lastFlags = event.GetFlags();
		m_text = this->GetValue().Lower();
	}

	if (m_caretPosition < 0)
	{
		findNotFound(findTextWindow, m_findString);
	}
	else if (m_text.rfind(m_findString, m_caretPosition) != std::string::npos)
	{
		if (m_text.rfind(m_findString, m_caretPosition) == 0)
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) + m_findString.size() - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else if (m_text.rfind(m_findString, m_caretPosition
			+ m_findString.size()) >= m_text.size())
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
		else
		{
			if (isNotCharacter(m_text.at(m_text.rfind(
				m_findString, m_caretPosition) - 1))
				&& isNotCharacter(m_text.at(m_text.rfind(
					m_findString, m_caretPosition)
					+ m_findString.size() - 1)))
			{
				m_caretPosition =
					m_text.rfind(m_findString, m_caretPosition);
				this->SetCaretPosition(m_caretPosition);
				this->ShowPosition(m_caretPosition);
				this->SetSelection(
					m_caretPosition, m_caretPosition + m_findString.size());

				// Previous position must be decremented in order
				// to skip the word on the next search iteration.
				--m_caretPosition;
			}
			else
			{
				findNotFound(findTextWindow, m_findString);
			}
		}
	}
	else
	{
		findNotFound(findTextWindow, m_findString);
	}
}

wxBEGIN_EVENT_TABLE(CustomRichTextCtrl, wxRichTextCtrl)
EVT_SET_FOCUS(CustomRichTextCtrl::preventFocus)
wxEND_EVENT_TABLE()