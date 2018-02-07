#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
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


    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	static MainFrame* m_pThis;
protected:
    virtual void OnFileOpen(wxCommandEvent& event);
};
#endif // MAINFRAME_H
