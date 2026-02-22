#include "MainFrame.h"
#include "AccountDialog.h"
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "L2 Reborn EasyLogin Manager", wxDefaultPosition, wxSize(600, 500)) {
    SetupUI();
    LoadConfig();
}

void MainFrame::SetupUI() {
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Toolbar
    auto* toolbarSizer = new wxBoxSizer(wxHORIZONTAL);
    auto btnOpen = new wxButton(this, wxID_OPEN, "Open File");
    auto btnSave = new wxButton(this, wxID_SAVE, "Save");
    auto btnAdd = new wxButton(this, wxID_ADD, "Add Account");

    toolbarSizer->Add(btnOpen, 0, wxALL, 5);
    toolbarSizer->Add(btnSave, 0, wxALL, 5);
    toolbarSizer->Add(btnAdd, 0, wxALL, 5);

    m_chkShowOnStart = new wxCheckBox(this, wxID_ANY, "Show on Start");
    m_chkHideLogin = new wxCheckBox(this, wxID_ANY, "Hide Login");
    toolbarSizer->Add(m_chkShowOnStart, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    toolbarSizer->Add(m_chkHideLogin, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    mainSizer->Add(toolbarSizer, 0, wxEXPAND);

    // List
    m_listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    m_listView->AppendColumn("Account ID", wxLIST_FORMAT_LEFT, 150);
    m_listView->AppendColumn("Description", wxLIST_FORMAT_LEFT, 250);
    mainSizer->Add(m_listView, 1, wxEXPAND | wxALL, 10);

    // Footer buttons
    auto* footerSizer = new wxBoxSizer(wxHORIZONTAL);
    auto btnUp = new wxButton(this, wxID_UP, "Move Up");
    auto btnDown = new wxButton(this, wxID_DOWN, "Move Down");
    auto btnEdit = new wxButton(this, wxID_EDIT, "Edit");
    auto btnDel = new wxButton(this, wxID_DELETE, "Delete");

    footerSizer->Add(btnUp, 0, wxALL, 5);
    footerSizer->Add(btnDown, 0, wxALL, 5);
    footerSizer->Add(btnEdit, 0, wxALL, 5);
    footerSizer->Add(btnDel, 0, wxALL, 5);
    mainSizer->Add(footerSizer, 0, wxALIGN_CENTER);

    SetSizer(mainSizer);

    // Bindings
    btnOpen->Bind(wxEVT_BUTTON, &MainFrame::OnOpen, this);
    btnSave->Bind(wxEVT_BUTTON, &MainFrame::OnSave, this);
    btnAdd->Bind(wxEVT_BUTTON, &MainFrame::OnAddAccount, this);
    btnUp->Bind(wxEVT_BUTTON, &MainFrame::OnMoveUp, this);
    btnDown->Bind(wxEVT_BUTTON, &MainFrame::OnMoveDown, this);
    btnEdit->Bind(wxEVT_BUTTON, &MainFrame::OnEditAccount, this);
    btnDel->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteAccount, this);
    m_chkShowOnStart->Bind(wxEVT_CHECKBOX, &MainFrame::OnToggleShowOnStart, this);
    m_chkHideLogin->Bind(wxEVT_CHECKBOX, &MainFrame::OnToggleHideLogin, this);
    m_listView->Bind(wxEVT_LIST_ITEM_ACTIVATED, [this](wxListEvent&){ 
        wxCommandEvent dummy;
        OnEditAccount(dummy); 
    });

    m_listView->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnLeftDClick, this);

    m_editCtrl = new wxTextCtrl(m_listView, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxBORDER_SIMPLE);
    m_editCtrl->Hide();
    m_editCtrl->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnEditFinish, this);
    m_editCtrl->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnEditFinish, this);
    m_editCtrl->Bind(wxEVT_CHAR, [this](wxKeyEvent& event) {
        if (event.GetKeyCode() == WXK_ESCAPE) {
            m_editCtrl->Hide();
            m_editingItem = -1;
        } else {
            event.Skip();
        }
    });
}

void MainFrame::OnLeftDClick(wxMouseEvent& event) {
    int flags = 0;
    long item = m_listView->HitTest(event.GetPosition(), flags);
    if (item == wxNOT_FOUND) {
        event.Skip();
        return;
    }

    int col = 1; // Always edit description (column 1)
    
    wxRect rect;
    m_listView->GetSubItemRect(item, col, rect);
    
    // Only start editing if the click was within the description column
    if (event.GetPosition().x >= rect.x && event.GetPosition().x <= rect.x + rect.width) {
        m_editingItem = item;
        m_editCtrl->SetSize(rect);
        m_editCtrl->SetValue(m_listView->GetItemText(item, col));
        m_editCtrl->Show();
        m_editCtrl->SetFocus();
        m_editCtrl->SelectAll();
    } else {
        event.Skip();
    }
}

void MainFrame::OnEditFinish(wxEvent& event) {
    if (m_editingItem != -1 && m_editCtrl->IsShown()) {
        wxString newVal = m_editCtrl->GetValue();
        if (m_accounts[m_editingItem].description != newVal) {
            m_accounts[m_editingItem].description = newVal;
            m_listView->SetItem(m_editingItem, 1, newVal);
        }
        m_editCtrl->Hide();
        m_editingItem = -1;
    }
}

void MainFrame::OnToggleShowOnStart(wxCommandEvent& event) {
    m_showOnStart = event.IsChecked();
}

void MainFrame::OnToggleHideLogin(wxCommandEvent& event) {
    m_hideLogin = event.IsChecked();
}

void MainFrame::LoadConfig() {
    wxConfig config("EasyLoginManager");
    if (config.Read("LastIniPath", &m_iniPath)) {
        if (wxFileExists(m_iniPath)) {
            LoadFile(m_iniPath);
        }
    }
}

void MainFrame::SaveConfig() {
    wxConfig config("EasyLoginManager");
    config.Write("LastIniPath", m_iniPath);
}

void MainFrame::OnOpen(wxCommandEvent&) {
    wxFileDialog openFileDialog(this, "Open INI file", "", "", "INI files (*.ini)|*.ini", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;
    m_iniPath = openFileDialog.GetPath();
    LoadFile(m_iniPath);
    SaveConfig();
}

void MainFrame::LoadFile(const wxString& path) {
    wxTextFile file;
    if (!file.Open(path)) return;

    m_accounts.clear();
    m_otherLines.clear();
    m_showOnStart = false;
    m_hideLogin = false;

    // Temporary storage for slots 1-199
    struct RawAcc { wxString id, pass, desc; };
    std::vector<RawAcc> raw(200); // 1-indexed

    for (size_t i = 0; i < file.GetLineCount(); ++i) {
        wxString line = file.GetLine(i);
        if (line.StartsWith("showOnStart=")) {
            long val;
            line.Mid(12).ToLong(&val);
            m_showOnStart = (val != 0);
        } else if (line.StartsWith("hideLogin=")) {
            long val;
            line.Mid(10).ToLong(&val);
            m_hideLogin = (val != 0);
        } else if (line.Contains("_id=") || line.Contains("_password=") || line.Contains("_description=")) {
            int underscorePos = line.Find('_');
            int equalPos = line.Find('=');
            if (underscorePos != wxNOT_FOUND && equalPos != wxNOT_FOUND) {
                long index;
                line.Left(underscorePos).ToLong(&index);
                wxString key = line.Mid(underscorePos + 1, equalPos - underscorePos - 1);
                wxString val = line.Mid(equalPos + 1);
                if (index >= 1 && index <= 199) {
                    if (key == "id") raw[index].id = val;
                    else if (key == "password") raw[index].pass = val;
                    else if (key == "description") raw[index].desc = val;
                }
            }
        } else if (!line.IsEmpty() && line != "[L2REBORN_EASYLOGIN]") {
            m_otherLines.Add(line);
        }
    }

    for (int i = 1; i <= 199; ++i) {
        if (!raw[i].id.IsEmpty()) {
            m_accounts.push_back({raw[i].id, raw[i].pass, raw[i].desc});
        }
    }

    m_chkShowOnStart->SetValue(m_showOnStart);
    m_chkHideLogin->SetValue(m_hideLogin);

    RefreshList();
}

void MainFrame::OnSave(wxCommandEvent& event) {
    if (m_iniPath.IsEmpty()) {
        OnOpen(event);
        if (m_iniPath.IsEmpty()) return;
    }

    wxTextFile file;
    if (wxFileExists(m_iniPath)) {
        file.Open(m_iniPath);
        file.Clear();
    } else {
        file.Create(m_iniPath);
    }

    file.AddLine("[L2REBORN_EASYLOGIN]");
    for (int i = 1; i <= 199; ++i) {
        Account acc = {"", "", ""};
        if (i <= (int)m_accounts.size()) {
            acc = m_accounts[i-1];
        }
        file.AddLine(wxString::Format("%d_id=%s", i, acc.id));
        file.AddLine(wxString::Format("%d_password=%s", i, acc.password));
        file.AddLine(wxString::Format("%d_description=%s", i, acc.description));
    }

    for (const auto& line : m_otherLines) {
        file.AddLine(line);
    }

    file.AddLine(wxString::Format("showOnStart=%d", m_showOnStart ? 1 : 0));
    file.AddLine(wxString::Format("hideLogin=%d", m_hideLogin ? 1 : 0));

    if (file.Write()) {
        wxMessageBox("File saved successfully!", "Success", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Failed to save file.", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::RefreshList() {
    m_listView->DeleteAllItems();
    for (const auto& acc : m_accounts) {
        long itemIndex = m_listView->InsertItem(m_listView->GetItemCount(), acc.id);
        m_listView->SetItem(itemIndex, 1, acc.description);
    }
}

void MainFrame::UpdateListViewItem(long index) {
    if (index < 0 || index >= (long)m_accounts.size()) return;
    m_listView->SetItem(index, 0, m_accounts[index].id);
    m_listView->SetItem(index, 1, m_accounts[index].description);
}

void MainFrame::OnAddAccount(wxCommandEvent&) {
    if (m_accounts.size() >= 199) {
        wxMessageBox("Maximum number of accounts (199) reached.", "Limit", wxOK | wxICON_WARNING);
        return;
    }
    AccountDialog dialog(this, "Add Account");
    if (dialog.ShowModal() == wxID_OK) {
        m_accounts.push_back(dialog.GetAccount());
        long index = m_listView->GetItemCount();
        m_listView->InsertItem(index, m_accounts.back().id);
        m_listView->SetItem(index, 1, m_accounts.back().description);
        m_listView->EnsureVisible(index);
        m_listView->Select(index);
    }
}

void MainFrame::OnEditAccount(wxCommandEvent&) {
    long selected = m_listView->GetFirstSelected();
    if (selected == wxNOT_FOUND) return;

    AccountDialog dialog(this, "Edit Account", m_accounts[selected]);
    if (dialog.ShowModal() == wxID_OK) {
        m_accounts[selected] = dialog.GetAccount();
        UpdateListViewItem(selected);
    }
}

void MainFrame::OnDeleteAccount(wxCommandEvent&) {
    long selected = m_listView->GetFirstSelected();
    if (selected == wxNOT_FOUND) return;

    if (wxMessageBox("Are you sure you want to delete this account?", "Confirm", wxYES_NO | wxICON_QUESTION) == wxYES) {
        m_accounts.erase(m_accounts.begin() + selected);
        m_listView->DeleteItem(selected);
        
        if (m_listView->GetItemCount() > 0) {
            long nextSelect = (selected < m_listView->GetItemCount()) ? selected : selected - 1;
            m_listView->Select(nextSelect);
        }
    }
}

void MainFrame::OnMoveUp(wxCommandEvent&) {
    long selected = m_listView->GetFirstSelected();
    if (selected <= 0) return;

    std::swap(m_accounts[selected], m_accounts[selected - 1]);
    UpdateListViewItem(selected);
    UpdateListViewItem(selected - 1);
    m_listView->Select(selected - 1);
    m_listView->EnsureVisible(selected - 1);
}

void MainFrame::OnMoveDown(wxCommandEvent&) {
    long selected = m_listView->GetFirstSelected();
    if (selected == wxNOT_FOUND || selected >= (long)m_accounts.size() - 1) return;

    std::swap(m_accounts[selected], m_accounts[selected + 1]);
    UpdateListViewItem(selected);
    UpdateListViewItem(selected + 1);
    m_listView->Select(selected + 1);
    m_listView->EnsureVisible(selected + 1);
}
