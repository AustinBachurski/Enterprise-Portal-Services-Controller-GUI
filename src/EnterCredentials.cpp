#include "EnterCredentials.hpp"

EnterCredentials::EnterCredentials(
    wxWindow* parent,
    Configuration& configuration)
    : wxDialog(parent, wxID_ANY, "Enter Enterprise Portal Information") 
{
    SetClientSize(350, 135);
    
    wxStaticText* urlPrompt{ new wxStaticText(this, wxID_ANY,
        "Enterprise Portal URL -", wxPoint(10, 14), wxSize(-1, 25)) };
    m_urlEntryBox = new wxTextCtrl(this, wxID_ANY,
        "i.e. https://maps.city.name.state.us/",
        wxPoint(131, 10), wxSize(209, 25));

    wxStaticText* usernamePrompt{ new wxStaticText(this, wxID_ANY,
        "Enterprise Portal Username -", wxPoint(10, 44), wxSize(-1, 25)) };
    m_usernameEntryBox = new wxTextCtrl(this, wxID_ANY,
        wxEmptyString, wxPoint(163, 40), wxSize(177, 25));

    wxStaticText* passwordPrompt{ new wxStaticText(this, wxID_ANY,
        "Enterprise Portal Password -", wxPoint(10, 74), wxSize(-1, 25)) };
    m_passwordEntryBox = new wxTextCtrl(this,wxID_ANY,
        wxEmptyString, wxPoint(160, 70), wxSize(180, 25),
        wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    m_passwordEntryBox->Bind(wxEVT_TEXT_ENTER,
        [this, &configuration](wxCommandEvent& event)
        { 
            configuration.m_updateCredentials = true;
            captureCredentials(event, configuration);
        });
        
    wxButton* OkButton{ new wxButton(this, wxID_OK,
        "OK", wxPoint(173, 105), wxSize(85, -1)) };
    OkButton->Bind(wxEVT_BUTTON, 
        [this, &configuration](wxCommandEvent& event)
        { 
            configuration.m_updateCredentials = true;
            captureCredentials(event, configuration);
        });

    wxButton* cancelButton{ new wxButton(this, wxID_CANCEL,
        "Cancel", wxPoint(87, 105), wxSize(85, -1)) };
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