#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <wx/wx.h>
#include "Account.h"

// Dialog for Adding/Editing an Account
class AccountDialog : public wxDialog {
public:
    AccountDialog(wxWindow* parent, const wxString& title, const Account& acc = {"", "", ""});

    Account GetAccount() const;

private:
    wxTextCtrl *m_idCtrl, *m_passCtrl, *m_descCtrl;
};

#endif // ACCOUNTDIALOG_H
