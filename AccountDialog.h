#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <wx/wx.h>
#include "Account.h"

// Dialog for Adding/Editing an Account
class AccountDialog : public wxDialog {
public:
    AccountDialog(wxWindow* parent, const wxString& title, const Account& acc = {"", "", ""});

    Account GetAccount() const;
    void OnToggleReveal(wxCommandEvent& event);

private:
    wxTextCtrl *m_idCtrl, *m_passCtrl, *m_descCtrl;
    wxButton* m_revealBtn;
    bool m_revealed = false;
};

#endif // ACCOUNTDIALOG_H
