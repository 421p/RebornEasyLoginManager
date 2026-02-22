#include "AccountDialog.h"
#include "LanguageManager.h"

AccountDialog::AccountDialog(wxWindow* parent, const wxString& title, const Account& acc)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) {

    auto* mainSizer = new wxBoxSizer(wxVERTICAL);
    auto gridSizer = new wxFlexGridSizer(2, 5, 5);
    gridSizer->AddGrowableCol(1, 1);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, L("ACCOUNT_ID_LABEL")), 0, wxALIGN_CENTER_VERTICAL);
    m_idCtrl = new wxTextCtrl(this, wxID_ANY, acc.id);
    gridSizer->Add(m_idCtrl, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, L("PASSWORD_LABEL")), 0, wxALIGN_CENTER_VERTICAL);
    auto* passSizer = new wxBoxSizer(wxHORIZONTAL);
    m_passCtrl = new wxTextCtrl(this, wxID_ANY, acc.password, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    m_revealBtn = new wxButton(this, wxID_ANY, "👁", wxDefaultPosition, wxSize(30, -1), wxBU_EXACTFIT);
    passSizer->Add(m_passCtrl, 1, wxEXPAND);
    passSizer->Add(m_revealBtn, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    gridSizer->Add(passSizer, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, L("DESCRIPTION_LABEL")), 0, wxALIGN_CENTER_VERTICAL);
    m_descCtrl = new wxTextCtrl(this, wxID_ANY, acc.description);
    gridSizer->Add(m_descCtrl, 1, wxEXPAND);

    mainSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxALL, 10);

    SetSizerAndFit(mainSizer);

    m_revealBtn->Bind(wxEVT_BUTTON, &AccountDialog::OnToggleReveal, this);
}

void AccountDialog::OnToggleReveal(wxCommandEvent&) {
    m_revealed = !m_revealed;
    m_revealBtn->SetLabel(m_revealed ? "🙈" : "👁");
    wxString val = m_passCtrl->GetValue();
    long style = m_passCtrl->GetWindowStyleFlag();

    auto* sizer = m_passCtrl->GetContainingSizer();
    
    // Find the item in the sizer to get its flags and proportion
    wxSizerItem* item = nullptr;
    for (size_t i = 0; i < sizer->GetItemCount(); ++i) {
        if (sizer->GetItem(i)->GetWindow() == m_passCtrl) {
            item = sizer->GetItem(i);
            break;
        }
    }
    
    int flags = item ? item->GetFlag() : wxEXPAND;
    int proportion = item ? item->GetProportion() : 1;
    
    wxWindowID id = m_passCtrl->GetId();
    m_passCtrl->Destroy();
    
    long newStyle = m_revealed ? 0 : wxTE_PASSWORD;
    m_passCtrl = new wxTextCtrl(this, id, val, wxDefaultPosition, wxDefaultSize, newStyle);
    
    sizer->Prepend(m_passCtrl, proportion, flags);
    sizer->Layout();
}

Account AccountDialog::GetAccount() const {
    return {m_idCtrl->GetValue(), m_passCtrl->GetValue(), m_descCtrl->GetValue()};
}
