#include "AccountDialog.h"

AccountDialog::AccountDialog(wxWindow* parent, const wxString& title, const Account& acc)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) {

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    auto gridSizer = new wxFlexGridSizer(2, 5, 5);
    gridSizer->AddGrowableCol(1, 1);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Account ID:"), 0, wxALIGN_CENTER_VERTICAL);
    m_idCtrl = new wxTextCtrl(this, wxID_ANY, acc.id);
    gridSizer->Add(m_idCtrl, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Password:"), 0, wxALIGN_CENTER_VERTICAL);
    m_passCtrl = new wxTextCtrl(this, wxID_ANY, acc.password, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    gridSizer->Add(m_passCtrl, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Description:"), 0, wxALIGN_CENTER_VERTICAL);
    m_descCtrl = new wxTextCtrl(this, wxID_ANY, acc.description);
    gridSizer->Add(m_descCtrl, 1, wxEXPAND);

    mainSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizerAndFit(mainSizer);
}

Account AccountDialog::GetAccount() const {
    return {m_idCtrl->GetValue(), m_passCtrl->GetValue(), m_descCtrl->GetValue()};
}
