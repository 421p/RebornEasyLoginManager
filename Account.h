#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <wx/wx.h>

// Structure to hold account data
struct Account {
    wxString id;
    wxString password;
    wxString description;
};

#endif // ACCOUNT_H
