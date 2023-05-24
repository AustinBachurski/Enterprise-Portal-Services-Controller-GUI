#include "CustomRichTextCtrl.hpp"

CustomRichTextCtrl::CustomRichTextCtrl(wxWindow* parent, long style) :
wxRichTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, style, wxDefaultValidator, wxEmptyString),
m_caretPosition{},
m_findString{}
{}

void CustomRichTextCtrl::discard(wxFocusEvent& event)
{
	event.Skip(false);
}

void CustomRichTextCtrl::findNotFound(wxFindReplaceDialog* findTextWindow,
	const std::string& searchString,
	const int textSize) const
	// findTextWindow is set as the parent of the Modal window so that window interaction is blocked.
{
	if (m_caretPosition > -2 && m_caretPosition < textSize)
		// Search text may be at position 0 and once found
		// m_caretPosition will be decremented to -1 in findText().
		// The inverse is true for top to bottom searching.
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

void CustomRichTextCtrl::findText(wxFindReplaceDialog* findTextWindow, wxFindDialogEvent& event)
{
	switch (event.GetFlags())
	{
	case Constants::SearchMethod::topToBottom:
	{
		std::string statusText{ std::move(this->GetValue().Lower()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString().Lower() != m_findString)
		{
			m_caretPosition = 0;
			m_findString = event.GetFindString().Lower();
		}

		if (statusText.find(m_findString, m_caretPosition) != std::string::npos)
		{
			m_caretPosition = statusText.find(m_findString, m_caretPosition);
			this->SelectWord(m_caretPosition);
			this->ShowPosition(m_caretPosition);
			// Previous position must be incremented in order
			// to skip the work on the next search iteration.
			++m_caretPosition;
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchCase_topToBottom:
	{
		std::string statusText{ std::move(this->GetValue()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString() != m_findString)
		{
			m_caretPosition = 0;
			m_findString = event.GetFindString();
		}

		if (statusText.find(m_findString, m_caretPosition) != std::string::npos)
		{
			m_caretPosition = statusText.find(m_findString, m_caretPosition);
			this->SelectWord(m_caretPosition);
			this->ShowPosition(m_caretPosition);
			// Previous position must be incremented in order
			// to skip the work on the next search iteration.
			++m_caretPosition;
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchWord_topToBottom:
	{
		std::string statusText{ std::move(this->GetValue().Lower()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString().Lower() != m_findString)
		{
			m_caretPosition = 0;
			m_findString = event.GetFindString().Lower();
		}

		if (statusText.find(m_findString, m_caretPosition) != std::string::npos)
		{
			if (statusText.find(m_findString, m_caretPosition) == 0)
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else if (statusText.find(m_findString, m_caretPosition + m_findString.size() >= statusText.size()))
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) - 1)))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) - 1))
					&& isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchBoth_topToBottom:
	{
		std::string statusText{ std::move(this->GetValue()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString() != m_findString)
		{
			m_caretPosition = 0;
			m_findString = event.GetFindString();
		}

		if (statusText.find(m_findString, m_caretPosition) != std::string::npos)
		{
			if (statusText.find(m_findString, m_caretPosition) == 0)
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else if (statusText.find(m_findString, m_caretPosition + m_findString.size() >= statusText.size()))
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) - 1)))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else
			{
				if (isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) - 1))
					&& isNotCharacter(statusText.at(statusText.find(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.find(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					++m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::bottomToTop:
	{
		std::string statusText{ std::move(this->GetValue().Lower()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString().Lower() != m_findString)
		{
			m_caretPosition = statusText.size();
			m_findString = event.GetFindString().Lower();
		}

		if (m_caretPosition < 0)
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		else if (statusText.rfind(m_findString, m_caretPosition) != std::string::npos)
		{
			m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
			this->SelectWord(m_caretPosition);
			this->ShowPosition(m_caretPosition);
			// Previous position must be incremented in order
			// to skip the work on the next search iteration.
			--m_caretPosition;
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchWord_bottomToTop:
	{
		std::string statusText{ std::move(this->GetValue().Lower()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString().Lower() != m_findString)
		{
			m_caretPosition = statusText.size();
			m_findString = event.GetFindString().Lower();
		}

		if (m_caretPosition < 0)
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		else if (statusText.rfind(m_findString, m_caretPosition) != std::string::npos)
		{
			if (statusText.rfind(m_findString, m_caretPosition) == 0)
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else if (statusText.rfind(m_findString, m_caretPosition + m_findString.size() >= statusText.size()))
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) - 1)))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) - 1))
					&& isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchCase_bottomToTop:
	{
		std::string statusText{ std::move(this->GetValue()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString().Lower() != m_findString)
		{
			m_caretPosition = statusText.size();
			m_findString = event.GetFindString().Lower();
		}

		if (m_caretPosition < 0)
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		else if (statusText.rfind(m_findString, m_caretPosition) != std::string::npos)
		{
			m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
			this->SelectWord(m_caretPosition);
			this->ShowPosition(m_caretPosition);
			// Previous position must be decremented in order
			// to skip the work on the next search iteration.
			--m_caretPosition;
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}

	case Constants::SearchMethod::matchBoth_bottomToTop:
	{
		std::string statusText{ std::move(this->GetValue()) };

		// Reset caret position if search string is new - "Find" vs "Find Next".
		if (event.GetFindString() != m_findString)
		{
			m_caretPosition = statusText.size();
			m_findString = event.GetFindString();
		}

		if (m_caretPosition < 0)
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		else if (statusText.rfind(m_findString, m_caretPosition) != std::string::npos)
		{
			if (statusText.rfind(m_findString, m_caretPosition) == 0)
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
			else if (statusText.rfind(m_findString, m_caretPosition + m_findString.size() >= statusText.size()))
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) - 1)))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}

			}
			else
			{
				if (isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) - 1))
					&& isNotCharacter(statusText.at(statusText.rfind(m_findString, m_caretPosition) + m_findString.size())))
				{
					m_caretPosition = statusText.rfind(m_findString, m_caretPosition);
					this->SelectWord(m_caretPosition);
					this->ShowPosition(m_caretPosition);
					// Previous position must be incremented in order
					// to skip the work on the next search iteration.
					--m_caretPosition;
				}
				else
				{
					findNotFound(findTextWindow, m_findString, statusText.size());
				}
			}
		}
		else
		{
			findNotFound(findTextWindow, m_findString, statusText.size());
		}
		break;
	}
	}
}

bool CustomRichTextCtrl::isNotCharacter(char character) const
{
	return character < 'A' || character > 'z';
}

wxBEGIN_EVENT_TABLE(CustomRichTextCtrl, wxRichTextCtrl)
EVT_SET_FOCUS(CustomRichTextCtrl::discard)
wxEND_EVENT_TABLE()