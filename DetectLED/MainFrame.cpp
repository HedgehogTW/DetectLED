#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/config.h>

#include <opencv2/opencv.hpp>

#define WAIT_TIME  10
MainFrame *MainFrame::m_pThis=NULL;
bool 		g_bPause;
bool 		g_bStop;
cv::VideoCapture vidCap;
cv::Mat img_input;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	m_pThis = this;
	int statusWidth[4] = {250, 150, 150};
    m_statusBar->SetFieldsCount(3, statusWidth);
	SetSize(800, 650);
	Center();	
	
	ShowMessage("Hello.... Cute Rat ...\n");	

	wxConfigBase *pConfig = wxConfigBase::Get();
	m_DataPath = pConfig->Read("/set/dataPath", "");
	
	if(m_DataPath.empty()) {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		m_DataPath = "~/Dropbox/Rat_Lick/data";
#else
		m_DataPath = "";
#endif	
	}
	if(! m_DataPath.empty())
		if(m_DataPath.back() != '/' && m_DataPath.back() != '\\')
			m_DataPath += "/";
	

}

MainFrame::~MainFrame()
{
	g_bStop = true;
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Write("/set/dataPath", wxString(m_DataPath));	
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetCopyright(_("My MainFrame"));
    info.SetLicence(_("GPL v2 or later"));
    info.SetDescription(_("Short description goes here"));
    ::wxAboutBox(info);
}
void MainFrame::OnFileOpen(wxCommandEvent& event)
{
	wxString wildcard = "video files (*.mkv;*.mp4;*.avi)|*.mkv;*.mp4;*.avi|\
	All Files (*.*)|*.*";

	wxFileDialog* OpenDialog = new wxFileDialog(
	    this, _("Choose a file to open"), m_DataPath, wxEmptyString,
	    wildcard, wxFD_OPEN, wxDefaultPosition);
	OpenDialog->SetFilterIndex(0);
	if (OpenDialog->ShowModal() == wxID_OK) {
	    wxString pathName = OpenDialog->GetPath();
		m_filename = pathName;
		m_DataPath = OpenDialog->GetDirectory().ToStdString(); 
		wxConfigBase *pConfig = wxConfigBase::Get();
		pConfig->Write("/set/dataPath", wxString(m_DataPath));	

	    wxString str;
		str.Printf("Open file:%s\n", pathName);
		ShowMessage(str);
		PlayVideoClip();
		m_timerVideo->Start(10);
		
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
	
}

void MainFrame::PlayVideoClip()
{

//	cv::Mat img_input;
	std::string 	strVideoName  = m_filename.ToStdString();
	vidCap.open(strVideoName);
	if(vidCap.isOpened()==false) {
		MainFrame::ShowMessage( "Load ... " + strVideoName + " ERROR\n");
		return;
	}

	m_fps = vidCap.get(CV_CAP_PROP_FPS);

	g_bPause = false;
	g_bStop = false;
	vidCap >> img_input;
	if (! img_input.empty()) {
		m_nWidth = img_input.cols;
		m_nHeight = img_input.rows;
		
		wxString str;
	    str.Printf("%d x %d, %.2f fps", m_nWidth, m_nHeight, m_fps);
	    m_statusBar->SetStatusText(str, 1);	
	}
	m_frameNumber = 1;
	return;
		
}


void MainFrame::OnFilePlay(wxCommandEvent& event)
{
//	if(GetThread()==NULL)  return;
//	if (GetThread()->IsAlive()){
//		GetThread()->Resume();
//	}		
}
void MainFrame::OnFileStop(wxCommandEvent& event)
{
//	if(GetThread()==NULL)  return;
//	GetThread()->Resume();
//	g_bStop = true;	
}
void MainFrame::OnFilePause(wxCommandEvent& event)
{
//	if(GetThread()==NULL)  return;
//	if(GetThread()->IsAlive()) {
//		GetThread()->Pause();
//	}
}
void MainFrame::OnPaint(wxPaintEvent& event)
{
//	if(img_input.data!=NULL)
//		m_scrollWin->setImage(img_input);
	m_scrollWin->OnPaint(event);
	ShowMessage( "OnPaint.\n");
}
void MainFrame::OnVideoTimer(wxTimerEvent& event)
{
	vidCap >> img_input;
	if (img_input.empty()) 	return;
	m_scrollWin->setImage(img_input);
	
	float sec = m_frameNumber /m_fps;
	int mm = sec / 60;
	int ss = sec - mm*60;
	wxString str;
	str.Printf("Frame No. %d, %02d:%02d", m_frameNumber, mm, ss);
	
	wxStatusBar* statusBar = MainFrame::m_pThis->GetStatusBar() ;
	statusBar->SetStatusText(str, 2);

	m_frameNumber++;
}
