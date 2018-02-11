//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _DETECTLED_DETECTLED_WXCRAFTER_BASE_CLASSES_H
#define _DETECTLED_DETECTLED_WXCRAFTER_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include "MyImageWin.h"
#include <wx/panel.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class MainFrameBaseClass : public wxFrame
{
protected:
    MyImageWin* m_scrollWin;
    wxPanel* m_mainPanel;
    wxRichTextCtrl* m_richTextMsg;
    wxMenuBar* m_menuBar;
    wxMenu* m_menuFile;
    wxMenuItem* m_menuItemOpen;
    wxMenuItem* m_menuItemExit;
    wxMenu* m_name8;
    wxMenuItem* m_menuItem9;
    wxToolBar* m_mainToolbar;
    wxStatusBar* m_statusBar;
    wxTimer* m_timerVideo;

protected:
    virtual void OnFileOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void OnExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAbout(wxCommandEvent& event) { event.Skip(); }
    virtual void OnVideoTimer(wxTimerEvent& event) { event.Skip(); }

public:
    MyImageWin* GetScrollWin() { return m_scrollWin; }
    wxRichTextCtrl* GetRichTextMsg() { return m_richTextMsg; }
    wxPanel* GetMainPanel() { return m_mainPanel; }
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    wxToolBar* GetMainToolbar() { return m_mainToolbar; }
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    wxTimer* GetTimerVideo() { return m_timerVideo; }
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("My Frame"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};

#endif
