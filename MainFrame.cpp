#include "MainFrame.h"
#include "AccountDialog.h"
#include "SettingsDialog.h"
#include "LanguageManager.h"
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/icon.h>
#include <wx/artprov.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, L("TITLE"), wxDefaultPosition, wxSize(700, 500)) {
#ifdef __WXMSW__
    SetIcon(wxIcon("IDI_ICON1", wxBITMAP_TYPE_ICO_RESOURCE));
#endif
    SetupUI();
    LoadConfig();
}

void MainFrame::SetupUI() {
    m_mainPanel = new wxPanel(this);
    auto* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Toolbar (Buttons)
    auto* toolbarSizer = new wxBoxSizer(wxHORIZONTAL);
    m_btnOpen = new wxButton(m_mainPanel, wxID_OPEN, L("OPEN_FILE"));
    m_btnSave = new wxButton(m_mainPanel, wxID_SAVE, L("SAVE"));
    m_btnAdd = new wxButton(m_mainPanel, wxID_ADD, L("ADD_ACCOUNT"));
    m_btnSettings = new wxButton(m_mainPanel, wxID_ANY, L("MANAGER_SETTINGS"));

#ifdef __WXMSW__
    m_btnOpen->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_BUTTON));
    m_btnSave->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_BUTTON));
    m_btnAdd->SetBitmap(wxArtProvider::GetBitmap(wxART_NEW, wxART_BUTTON));
    m_btnSettings->SetBitmap(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_BUTTON));
#endif

    toolbarSizer->Add(m_btnOpen, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    toolbarSizer->Add(m_btnSave, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    toolbarSizer->Add(m_btnAdd, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    toolbarSizer->Add(m_btnSettings, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);

    toolbarSizer->AddStretchSpacer();

    wxArrayString choices;
    choices.Add("English");
    choices.Add("Français");
    m_choiceLang = new wxChoice(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    
    wxString currentLang = LanguageManager::Get().GetCurrentLanguage();
    if (currentLang == "fr") m_choiceLang->SetSelection(1);
    else m_choiceLang->SetSelection(0);

    m_choiceLang->SetMinSize(wxSize(100, -1));

    toolbarSizer->Add(m_choiceLang, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 5);

    mainSizer->Add(toolbarSizer, 0, wxEXPAND | wxTOP, 5);

    // List
    m_listView = new wxListView(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
#ifdef __WXMSW__
    m_listView->SetWindowStyleFlag(m_listView->GetWindowStyleFlag() | wxLC_HRULES | wxLC_VRULES);
#endif
    m_listView->AppendColumn(L("ACCOUNT_ID"), wxLIST_FORMAT_LEFT, 150);
    m_listView->AppendColumn(L("DESCRIPTION"), wxLIST_FORMAT_LEFT, 250);
    mainSizer->Add(m_listView, 1, wxEXPAND | wxALL, 10);

    // Footer buttons
    auto* footerSizer = new wxBoxSizer(wxHORIZONTAL);
    m_btnUp = new wxButton(m_mainPanel, wxID_UP, L("MOVE_UP"));
    m_btnDown = new wxButton(m_mainPanel, wxID_DOWN, L("MOVE_DOWN"));
    m_btnEdit = new wxButton(m_mainPanel, wxID_EDIT, L("EDIT"));
    m_btnDel = new wxButton(m_mainPanel, wxID_DELETE, L("DELETE"));

#ifdef __WXMSW__
    m_btnUp->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_UP, wxART_BUTTON));
    m_btnDown->SetBitmap(wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_BUTTON));
    m_btnEdit->SetBitmap(wxArtProvider::GetBitmap(wxART_EDIT, wxART_BUTTON));
    m_btnDel->SetBitmap(wxArtProvider::GetBitmap(wxART_DELETE, wxART_BUTTON));
#endif

    footerSizer->Add(m_btnUp, 0, wxALL, 5);
    footerSizer->Add(m_btnDown, 0, wxALL, 5);
    footerSizer->Add(m_btnEdit, 0, wxALL, 5);
    footerSizer->Add(m_btnDel, 0, wxALL, 5);
    mainSizer->Add(footerSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

    m_mainPanel->SetSizer(mainSizer);

    // Bindings
    m_btnOpen->Bind(wxEVT_BUTTON, &MainFrame::OnOpen, this);
    m_btnSave->Bind(wxEVT_BUTTON, &MainFrame::OnSave, this);
    m_btnAdd->Bind(wxEVT_BUTTON, &MainFrame::OnAddAccount, this);
    m_btnSettings->Bind(wxEVT_BUTTON, &MainFrame::OnSettings, this);
    m_btnUp->Bind(wxEVT_BUTTON, &MainFrame::OnMoveUp, this);
    m_btnDown->Bind(wxEVT_BUTTON, &MainFrame::OnMoveDown, this);
    m_btnEdit->Bind(wxEVT_BUTTON, &MainFrame::OnEditAccount, this);
    m_btnDel->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteAccount, this);
    m_choiceLang->Bind(wxEVT_CHOICE, &MainFrame::OnLanguageSelected, this);
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

void MainFrame::OnSettings(wxCommandEvent&) {
    SettingsDialog dialog(this, m_showOnStart, m_hideLogin);
    if (dialog.ShowModal() == wxID_OK) {
        m_showOnStart = dialog.GetShowOnStart();
        m_hideLogin = dialog.GetHideLogin();
        SaveConfig();
    }
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
    wxFileDialog openFileDialog(this, L("OPEN_INI_DIALOG_TITLE"), "", "", L("INI_FILES_WILDCARD"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
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
        wxMessageBox(L("LIMIT_REACHED_MESSAGE"), L("LIMIT_REACHED_TITLE"), wxOK | wxICON_WARNING);
        return;
    }
    AccountDialog dialog(this, L("ADD_ACCOUNT_TITLE"));
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

    AccountDialog dialog(this, L("EDIT_ACCOUNT_TITLE"), m_accounts[selected]);
    if (dialog.ShowModal() == wxID_OK) {
        m_accounts[selected] = dialog.GetAccount();
        UpdateListViewItem(selected);
    }
}

void MainFrame::OnLanguageSelected(wxCommandEvent&) {
    int sel = m_choiceLang->GetSelection();
    wxString langCode = (sel == 1) ? "fr" : "en";
    
    if (LanguageManager::Get().LoadLanguage(langCode)) {
        wxConfig::Get()->Write("/Language", langCode);
        wxConfig::Get()->Flush();
        RefreshLabels();
    }
}

void MainFrame::RefreshLabels() {
    SetTitle(L("TITLE"));
    m_btnOpen->SetLabel(L("OPEN_FILE"));
    m_btnSave->SetLabel(L("SAVE"));
    m_btnAdd->SetLabel(L("ADD_ACCOUNT"));
    m_btnSettings->SetLabel(L("MANAGER_SETTINGS"));
    
    m_btnUp->SetLabel(L("MOVE_UP"));
    m_btnDown->SetLabel(L("MOVE_DOWN"));
    m_btnEdit->SetLabel(L("EDIT"));
    m_btnDel->SetLabel(L("DELETE"));
    
    wxListItem item0;
    m_listView->GetColumn(0, item0);
    item0.SetText(L("ACCOUNT_ID"));
    m_listView->SetColumn(0, item0);
    
    wxListItem item1;
    m_listView->GetColumn(1, item1);
    item1.SetText(L("DESCRIPTION"));
    m_listView->SetColumn(1, item1);
    
    Layout();
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
