//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"


// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;


MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer1);
    
    m_scrollWin = new MyImageWin(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxHSCROLL|wxVSCROLL);
    m_scrollWin->SetScrollRate(5, 5);
    
    boxSizer1->Add(m_scrollWin, 3, wxALL|wxEXPAND, WXC_FROM_DIP(1));
    
    m_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer1->Add(m_mainPanel, 1, wxEXPAND, WXC_FROM_DIP(1));
    
    wxBoxSizer* boxSizer13 = new wxBoxSizer(wxVERTICAL);
    m_mainPanel->SetSizer(boxSizer13);
    
    m_richTextMsg = new wxRichTextCtrl(m_mainPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1,-1)), wxTE_MULTILINE|wxTE_PROCESS_TAB|wxTE_PROCESS_ENTER|wxWANTS_CHARS);
    
    boxSizer13->Add(m_richTextMsg, 1, wxALL|wxEXPAND, WXC_FROM_DIP(2));
    
    m_menuBar = new wxMenuBar(0);
    this->SetMenuBar(m_menuBar);
    
    m_menuFile = new wxMenu();
    m_menuBar->Append(m_menuFile, _("File"));
    
    m_menuItemOpen = new wxMenuItem(m_menuFile, wxID_OPEN, _("Open"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemOpen);
    
    m_menuItemPlay = new wxMenuItem(m_menuFile, wxID_PLAY, _("Play"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemPlay);
    
    m_menuItemPause = new wxMenuItem(m_menuFile, wxID_PAUSE, _("Pause"), wxT(""), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemPause);
    
    m_menuItemExit = new wxMenuItem(m_menuFile, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    m_menuFile->Append(m_menuItemExit);
    
    m_name8 = new wxMenu();
    m_menuBar->Append(m_name8, _("Help"));
    
    m_menuItem9 = new wxMenuItem(m_name8, wxID_ABOUT, _("About..."), wxT(""), wxITEM_NORMAL);
    m_name8->Append(m_menuItem9);
    
    m_mainToolbar = this->CreateToolBar(wxTB_FLAT, wxID_ANY);
    m_mainToolbar->SetToolBitmapSize(wxSize(32,32));
    
    m_mainToolbar->AddTool(wxID_OPEN, _("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_mainToolbar->AddTool(wxID_PLAY, _("Play"), wxXmlResource::Get()->LoadBitmap(wxT("play")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_mainToolbar->AddTool(wxID_PAUSE, _("Pause"), wxXmlResource::Get()->LoadBitmap(wxT("pause")), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    m_mainToolbar->Realize();
    
    m_statusBar = new wxStatusBar(this, wxID_ANY, wxSTB_DEFAULT_STYLE);
    m_statusBar->SetFieldsCount(1);
    this->SetStatusBar(m_statusBar);
    
    m_timerVideo = new wxTimer;
    
    SetName(wxT("MainFrameBaseClass"));
    SetSize(500,300);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrameBaseClass::OnClose), NULL, this);
    this->Connect(m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Connect(m_menuItemPlay->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFilePlay), NULL, this);
    this->Connect(m_menuItemPause->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFilePause), NULL, this);
    this->Connect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    m_timerVideo->Connect(wxEVT_TIMER, wxTimerEventHandler(MainFrameBaseClass::OnVideoTimer), NULL, this);
    
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrameBaseClass::OnClose), NULL, this);
    this->Disconnect(m_menuItemOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFileOpen), NULL, this);
    this->Disconnect(m_menuItemPlay->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFilePlay), NULL, this);
    this->Disconnect(m_menuItemPause->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnFilePause), NULL, this);
    this->Disconnect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItem9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrameBaseClass::OnAbout), NULL, this);
    m_timerVideo->Disconnect(wxEVT_TIMER, wxTimerEventHandler(MainFrameBaseClass::OnVideoTimer), NULL, this);
    
    m_timerVideo->Stop();
    wxDELETE( m_timerVideo );

}
