#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>
#include "Account.h"

// Main Frame
class MainFrame : public wxFrame {
public:
    MainFrame();

private:
    wxListView* m_listView{};
    wxTextCtrl* m_editCtrl{};
    long m_editingItem = -1;
    wxString m_iniPath;
    std::vector<Account> m_accounts;
    wxArrayString m_otherLines; // To store lines that aren't account data

    bool m_showOnStart = false;
    bool m_hideLogin = false;
    wxCheckBox* m_chkShowOnStart{};
    wxCheckBox* m_chkHideLogin{};

    void SetupUI();
    void OnLeftDClick(wxMouseEvent& event);
    void OnEditFinish(wxEvent& event);
    void OnToggleShowOnStart(wxCommandEvent&);
    void OnToggleHideLogin(wxCommandEvent&);
    void LoadConfig();
    void SaveConfig();
    void OnOpen(wxCommandEvent&);
    void LoadFile(const wxString& path);
    void OnSave(wxCommandEvent& event);
    void RefreshList();
    void UpdateListViewItem(long index);
    void OnAddAccount(wxCommandEvent&);
    void OnEditAccount(wxCommandEvent&);
    void OnDeleteAccount(wxCommandEvent&);
    void OnMoveUp(wxCommandEvent&);
    void OnMoveDown(wxCommandEvent&);
};

#endif // MAINFRAME_H
