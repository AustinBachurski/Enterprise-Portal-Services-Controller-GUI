#include "EnterCredentials.hpp"

EnterCredentials::EnterCredentials(
    wxWindow* parent,
    Configuration& configuration)
    : wxDialog(parent, wxID_ANY, "Enter Enterprise Portal Information") 
{
    wxPanel* urlPanel{ new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize) };
    wxPanel* usernamePanel{ new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize) };
    wxPanel* passwordPanel{ new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize) };
    wxPanel* buttonPanel{ new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize) };

    wxBoxSizer* urlSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* usernameSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* passwordSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* buttonSizer{ new wxBoxSizer(wxHORIZONTAL) };
    wxBoxSizer* stacker{ new wxBoxSizer(wxVERTICAL) };

    wxStaticText* urlPrompt{ new wxStaticText(urlPanel, wxID_ANY,
        "Enterprise Portal URL", wxDefaultPosition, wxDefaultSize) };

    m_urlEntryBox = new wxTextCtrl(urlPanel, wxID_ANY,
        "i.e. https://maps.city.name.state.us/",
        wxDefaultPosition, wxSize(300, -1));

    wxStaticText* usernamePrompt{ new wxStaticText(usernamePanel, wxID_ANY,
        "Enterprise Portal Username", wxDefaultPosition, wxDefaultSize) };

    m_usernameEntryBox = new wxTextCtrl(usernamePanel, wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxSize(300, -1));

    wxStaticText* passwordPrompt{ new wxStaticText(passwordPanel, wxID_ANY,
        "Enterprise Portal Password", wxDefaultPosition, wxDefaultSize) };

    m_passwordEntryBox = new wxTextCtrl(passwordPanel,wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxSize(300, -1),
        wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    m_passwordEntryBox->Bind(wxEVT_TEXT_ENTER,
        [this, &configuration](wxCommandEvent& event)
        { 
            configuration.m_updateCredentials = true;
            captureCredentials(event, configuration);
        });
        
    wxButton* OkButton{ new wxButton(buttonPanel, wxID_OK,
        "OK", wxDefaultPosition, wxDefaultSize) };
    OkButton->Bind(wxEVT_BUTTON, 
        [this, &configuration](wxCommandEvent& event)
        { 
            configuration.m_updateCredentials = true;
            captureCredentials(event, configuration);
        });

    wxButton* cancelButton{ new wxButton(buttonPanel, wxID_CANCEL,
        "Cancel", wxDefaultPosition, wxDefaultSize) };
    cancelButton->Bind(wxEVT_BUTTON,
        [this, &configuration](wxCommandEvent& event)
        {
            EndModal(wxCANCEL);
            Destroy();

            // Window closing would trigger the 'required' popup window
            // regardless of why it closed, m_okPressed prevents this.
            if (!m_okPressed && !configuration.credentialsAcquired())
            {
                wxMessageDialog* required{ new wxMessageDialog(this,
                    "Tool will not function unless credentials are entered.",
                    "Required for Operation", wxOK | wxICON_ERROR) };
                required->ShowModal();
            }
        });

    urlSizer->Add(0, 0, wxEXPAND, 0);
    urlSizer->Add(urlPrompt, 0, wxALIGN_CENTER_VERTICAL, 0);
    urlSizer->Add(m_urlEntryBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    urlPanel->SetSizerAndFit(urlSizer);

    usernameSizer->Add(usernamePrompt, 0, wxALIGN_CENTER_VERTICAL, 0);
    usernameSizer->Add(m_usernameEntryBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    usernamePanel->SetSizerAndFit(usernameSizer);

    passwordSizer->Add(0, 0, wxEXPAND, 0);
    passwordSizer->Add(passwordPrompt, 0, wxALIGN_CENTER_VERTICAL, 0);
    passwordSizer->Add(m_passwordEntryBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    passwordPanel->SetSizerAndFit(passwordSizer);
    
    buttonSizer->Add(0, 0, wxEXPAND);
    buttonSizer->Add(OkButton, 0, wxALIGN_CENTER | wxRIGHT, 5);
    buttonSizer->Add(cancelButton, 0, wxALIGN_CENTER);
    buttonPanel->SetSizerAndFit(buttonSizer);

    stacker->Add(urlPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    stacker->Add(usernamePanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    stacker->Add(passwordPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    stacker->Add(buttonPanel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    stacker->Add(0, 5);

    SetSizerAndFit(stacker);
}

void EnterCredentials::captureCredentials(
    wxCommandEvent& event, Configuration& configuration)
{
    m_okPressed = true;

    configuration.writeConfig(
        m_urlEntryBox->GetValue().ToStdString(),
        m_usernameEntryBox->GetValue().ToStdString(),
        m_passwordEntryBox->GetValue().ToStdString());

    Close();
}
