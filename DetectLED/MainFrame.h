#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include <wx/thread.h>

wxDECLARE_EVENT(wxEVT_MYTHREAD_FINISHED, wxThreadEvent);

class MainFrame : public MainFrameBaseClass, public wxThreadHelper
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
	
	static void ShowMessage(wxString szFormat,...) {
		wxString strMsg;
		va_list argList;
		va_start(argList, szFormat);
		strMsg.PrintfV(szFormat, argList);
		va_end(argList); 
		// above is used to process arguments like as printf

		m_pThis->m_richTextMsg->AppendText(strMsg);
		int lastPos = m_pThis->m_richTextMsg->GetLastPosition();
		m_pThis->m_richTextMsg->ShowPosition(lastPos);
	}
	void PlayVideoClip();
	
	wxString m_filename;
	std::string 	m_DataPath;
	
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	static MainFrame* m_pThis;
protected:
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnFileStop(wxCommandEvent& event);
    virtual void OnFilePlay(wxCommandEvent& event);
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnFilePause(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
	
	void OnThreadFinished(wxThreadEvent& evt);
    virtual wxThread::ExitCode Entry();	
};
#endif // MAINFRAME_H
