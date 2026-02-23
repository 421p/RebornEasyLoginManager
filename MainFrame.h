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
    wxPanel* m_mainPanel{};
    wxListView* m_listView{};
    wxTextCtrl* m_editCtrl{};
    long m_editingItem = -1;
    wxString m_iniPath;
    std::vector<Account> m_accounts;
    wxArrayString m_otherLines; // To store lines that aren't account data

    bool m_showOnStart = false;
    bool m_hideLogin = false;
    wxChoice* m_choiceLang{};

    void SetupUI();
    void RefreshLabels();
    void OnLanguageSelected(wxCommandEvent&);
    void OnSettings(wxCommandEvent&);
    void OnLeftDClick(wxMouseEvent& event);
    void OnEditFinish(wxEvent& event);
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
    
    // UI elements that need refreshing
    wxButton* m_btnOpen{};
    wxButton* m_btnSave{};
    wxButton* m_btnAdd{};
    wxButton* m_btnSettings{};
    wxButton* m_btnUp{};
    wxButton* m_btnDown{};
    wxButton* m_btnEdit{};
    wxButton* m_btnDel{};
};

#endif // MAINFRAME_H
