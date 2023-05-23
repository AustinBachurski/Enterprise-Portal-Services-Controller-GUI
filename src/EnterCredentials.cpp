#include "EnterCredentials.hpp"

EnterCredentials::EnterCredentials(wxWindow* parent, Credentials& credentials) :
    wxDialog(parent, wxID_ANY, "Enter Enterprise Portal Information"),
    m_okPressed{ false }
{
    SetClientSize(350, 135);
    //CenterOnScreen();
    
    // URL Input Box.
    wxStaticText* urlPrompt = new wxStaticText(this, wxID_ANY,
        "Enterprise Portal URL -", wxPoint(10, 14), wxSize(-1, 25));
    m_urlEntryBox = new wxTextCtrl(this, wxID_ANY,
        "i.e. https://maps.city.name.state.us/",
        wxPoint(131, 10), wxSize(209, 25));

    // Username Input Box.
    wxStaticText* usernamePrompt = new wxStaticText(this, wxID_ANY,
        "Enterprise Portal Username -", wxPoint(10, 44), wxSize(-1, 25));
    m_usernameEntryBox = new wxTextCtrl(this, wxID_ANY,
        wxEmptyString, wxPoint(163, 40), wxSize(177, 25));

    // Password Input Box.
    wxStaticText* passwordPrompt = new wxStaticText(this, wxID_ANY,
        "Enterprise Portal Password -", wxPoint(10, 74), wxSize(-1, 25));
    m_passwordEntryBox = new wxTextCtrl(this,wxID_ANY,
        wxEmptyString, wxPoint(160, 70), wxSize(180, 25),
        wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    m_passwordEntryBox->Bind(wxEVT_TEXT_ENTER,
        [this, &credentials](wxCommandEvent& event)
        { captureCredentials(event, credentials); });
        
    // OK Button.
    wxButton* OkButton = new wxButton(this, wxID_OK,
        "OK", wxPoint(173, 105), wxSize(85, -1));
    OkButton->Bind(wxEVT_BUTTON, 
        [this, &credentials](wxCommandEvent& event)
        {
            m_okPressed = true;
            captureCredentials(event, credentials);
        });

    // Cancel Button.
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL,
        "Cancel", wxPoint(87, 105), wxSize(85, -1));
    cancelButton->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent& event)
        {
            EndModal(wxCANCEL);
            Destroy();

            if (!m_okPressed)
            {
                wxMessageDialog* required = new wxMessageDialog(this,
                    "Tool will not function unless credentials are entered.",
                    "Required for Operation", wxOK | wxICON_ERROR);
                required->ShowModal();
            }
        });
}

/*
Captures text from the text entry boxes and sends to Credentials::setConfig().
*/ 
void EnterCredentials::captureCredentials(
    wxCommandEvent& event,Credentials& credentials)
{
    credentials.setConfig(
        std::move(m_urlEntryBox->GetValue().ToStdString()),
        std::move(m_usernameEntryBox->GetValue().ToStdString()),
        std::move(m_passwordEntryBox->GetValue().ToStdString()));

    Close();
}