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
	SetSize(700, 760);
	Center();	
	
	ShowMessage("Hello.... \n");	

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
	
    
    m_FileHistory = new wxFileHistory(9);
    m_FileHistory->UseMenu(m_menuFile);
    m_FileHistory->AddFilesToMenu(m_menuFile);
    m_FileHistory->Load(*pConfig);
	
    this->Connect(wxID_FILE1, wxID_FILE9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnMRUFile), NULL, this);

}

MainFrame::~MainFrame()
{
	g_bStop = true;
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->Write("/set/dataPath", wxString(m_DataPath));	
	

	m_FileHistory->Save(*pConfig);
	delete m_FileHistory;	
	delete wxConfigBase::Set((wxConfigBase *) NULL);
	
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

void MainFrame::OnMRUFile(wxCommandEvent& event)
{
    wxString f(m_FileHistory->GetHistoryFile(event.GetId() - wxID_FILE1));
    if (!f.empty())  {
		m_filename = f;
		PlayVideoClip();;
	}
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
		m_FileHistory->AddFileToHistory(pathName);

		m_filename = pathName;
		m_DataPath = OpenDialog->GetDirectory().ToStdString(); 
		wxConfigBase *pConfig = wxConfigBase::Get();
		pConfig->Write("/set/dataPath", wxString(m_DataPath));	


		PlayVideoClip();
		
		
	}
	// Clean up after ourselves
	OpenDialog->Destroy();
	
}

void MainFrame::PlayVideoClip()
{

	wxString str;
	str.Printf("Open file:%s\n", m_filename);
	ShowMessage(str);
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
	m_timerVideo->Start(10);
	
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
	

	img_process(img_input);
	
	draw_grid(img_input);	
	m_scrollWin->setImage(img_input);
	
	float sec = m_frameNumber /m_fps;
	int mm = sec / 60;
	int ss = sec - mm*60;
	wxString str;
	str.Printf("Frame %d, %02d:%02d", m_frameNumber, mm, ss);
	
	wxStatusBar* statusBar = MainFrame::m_pThis->GetStatusBar() ;
	statusBar->SetStatusText(str, 2);

	m_frameNumber++;
}

void MainFrame::img_process(cv::Mat &img)
{
	cv::Mat hsv, bin;
	cv::cvtColor(img, hsv, CV_BGR2HSV);
	std::vector<cv::Mat> planes;
	cv::split(hsv, planes);
	cv::imshow("hue", planes[0]);
	cv::imshow("Value", planes[2]);
	
	cv::threshold(planes[2], bin, 220, 255, cv::THRESH_BINARY );
	cv::medianBlur(bin, bin, 7);
//	cv::Mat st = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
//	cv::morphologyEx(bin, bin, cv::MORPH_OPEN, st);
	//// find maxima CC
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	int mode = CV_RETR_LIST;
	int method = CV_CHAIN_APPROX_NONE;
	cv::Mat m = bin.clone();
	findContours(m, contours, hierarchy, mode, method);
	
	cv::imshow("binary", bin);
}
void MainFrame::draw_grid(cv::Mat &img)
{
	int num_grid = 10;
	int xgap = m_nWidth / num_grid ;
	int ygap = m_nHeight / num_grid ;
	char  box_text[10];
	for(int i=0; i<num_grid; i++) {
		cv::line(img, cv::Point(xgap * i, 0), cv::Point(xgap * i, m_nHeight), cv::Scalar(0, 255, 255), 1);
		cv::line(img, cv::Point(0, ygap * i), cv::Point(m_nWidth, ygap * i), cv::Scalar(0, 255, 255), 1);
		
		sprintf(box_text, "%d", i);
		cv::putText(img, box_text, cv::Point(xgap * i+30, 20), cv::FONT_HERSHEY_DUPLEX, 0.6, CV_RGB(0,255,0), 1.0);
		sprintf(box_text, "%c", i+'A');
		cv::putText(img, box_text, cv::Point(10, ygap * i+30), cv::FONT_HERSHEY_DUPLEX, 0.6, CV_RGB(0,255,0), 1.0);		
	}	
}