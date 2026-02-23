#include "SettingsDialog.h"
#include "LanguageManager.h"

SettingsDialog::SettingsDialog(wxWindow* parent, bool showOnStart, bool hideLogin)
    : wxDialog(parent, wxID_ANY, L("SETTINGS_TITLE"), wxDefaultPosition, wxDefaultSize) {

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    auto* panel = new wxPanel(this);
    auto* panelSizer = new wxBoxSizer(wxVERTICAL);

    m_chkShowOnStart = new wxCheckBox(panel, wxID_ANY, L("SHOW_ON_START"));
    m_chkShowOnStart->SetValue(showOnStart);
    panelSizer->Add(m_chkShowOnStart, 0, wxALL, 10);

    m_chkHideLogin = new wxCheckBox(panel, wxID_ANY, L("HIDE_LOGIN"));
    m_chkHideLogin->SetValue(hideLogin);
    panelSizer->Add(m_chkHideLogin, 0, wxALL, 10);
    
    panel->SetSizer(panelSizer);
    mainSizer->Add(panel, 1, wxEXPAND);

    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizerAndFit(mainSizer);
}

bool SettingsDialog::GetShowOnStart() const {
    return m_chkShowOnStart->GetValue();
}

bool SettingsDialog::GetHideLogin() const {
    return m_chkHideLogin->GetValue();
}
