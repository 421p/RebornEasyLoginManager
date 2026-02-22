#include <wx/wx.h>
#include "MainFrame.h"
#include "LanguageManager.h"
#include <wx/intl.h>
#include <wx/config.h>

class MyApp : public wxApp {
public:
    bool OnInit() override {
        // Initialize language
        wxConfigBase* config = wxConfig::Get();
        wxString langCode = config->Read("/Language", "");
        
        if (langCode.IsEmpty()) {
            langCode = "en";
            if (wxLocale::GetSystemLanguage() == wxLANGUAGE_FRENCH) {
                langCode = "fr";
            }
        }
        
        LanguageManager::Get().LoadLanguage(langCode);

        auto* frame = new MainFrame();
        frame->Show(true);
        frame->Raise();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
