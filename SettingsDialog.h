#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <wx/wx.h>

class SettingsDialog : public wxDialog {
public:
    SettingsDialog(wxWindow* parent, bool showOnStart, bool hideLogin);

    bool GetShowOnStart() const;
    bool GetHideLogin() const;

private:
    wxCheckBox* m_chkShowOnStart;
    wxCheckBox* m_chkHideLogin;
};

#endif // SETTINGSDIALOG_H
