#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/config.h>

#include <opencv2/opencv.hpp>

#define WAIT_TIME  10
#define BLOB_DIST  20
MainFrame *MainFrame::m_pThis=NULL;
bool 		g_bPause;
bool 		g_bPlay;
cv::VideoCapture vidCap;
cv::Mat img_input;

int blob_id = 0;
//FILE *fp = fopen("_hist.csv", "w");
MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	m_pThis = this;
	int statusWidth[4] = {200, 130, 130, 100};
    m_statusBar->SetFieldsCount(4, statusWidth);
	SetSize(700, 760);
	Center();	
	
	g_bPause = false;
	g_bPlay = true;
	m_num_grid = 8;
	
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

	int ksize = 17; // shuold be odd
	m_nKnlBW =ksize/2;
	m_kernel.resize(2*m_nKnlBW + 1); 
	Mat mKernel = getGaussianKernel(ksize, -1);
	for(int i=-m_nKnlBW; i<=m_nKnlBW; i++) {
//		float x = (float)i/m_nKnlBW;
//		m_kernel[i+m_nKnlBW] = (1.0-x*x)/ (m_nKnlBW);    或者後面再 /= m_frameCount*h
		m_kernel[i+m_nKnlBW] = mKernel.at<double>(i+m_nKnlBW, 0);
//		ShowMessage("%.4f ", m_kernel[i+m_nKnlBW]);
	}	
}

MainFrame::~MainFrame()
{
	m_vBlob.clear();	
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
//	g_bStop = false;
	vidCap >> img_input;
	if (! img_input.empty()) {
		m_nWidth = img_input.cols;
		m_nHeight = img_input.rows;
		m_xgap = m_nWidth / m_num_grid ;
		m_ygap = m_nHeight / m_num_grid ;
		wxString str;
	    str.Printf("%d x %d, %.2f fps", m_nWidth, m_nHeight, m_fps);
	    m_statusBar->SetStatusText(str, 1);	
	}
	m_frameNumber = 1;
	m_timerVideo->Start(20);
	m_vBlob.clear();	
	blob_id = 0;
	return;
		
}


void MainFrame::OnFilePlay(wxCommandEvent& event)
{
	g_bPlay = true;		
}
void MainFrame::OnFileStop(wxCommandEvent& event)
{
//	if(GetThread()==NULL)  return;
//	GetThread()->Resume();
//	g_bStop = true;	
}
void MainFrame::OnFilePause(wxCommandEvent& event)
{
	g_bPlay = false;	
//	fclose(fp);
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
	if(g_bPlay ==false) return;
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
//	cv::imshow("hue", planes[0]);
//	cv::imshow("Value", planes[2]);
	
	cv::threshold(planes[2], bin, 240, 255, cv::THRESH_BINARY );
	cv::medianBlur(bin, bin, 7);
//	cv::imshow("binary", bin);
//	cv::Mat st = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
//	cv::morphologyEx(bin, bin, cv::MORPH_OPEN, st);
	//// find maxima CC
	vector<vector<cv::Point>> contours;
	int numBlobs = find_blobs(bin, contours);
	if(numBlobs <= 0) {
		for(int b=0; b<m_vBlob.size(); b++)
			m_vBlob[b].willRemove = true;
		remove_blobs();
	}else
		identify_blobs(contours, planes[0]);
		


}
void MainFrame::identify_blobs(vector<vector<cv::Point>>&contours, cv::Mat &hue)
{
	wxString str;
	char  strPos[10];
	int h;
	int numBlobs = m_vBlob.size(); 
	
	for(int b=0; b<numBlobs; b++)
		m_vBlob[b].willRemove = true;
		
	int numCont = contours.size();		
	for(int j=0; j<numCont; j++) {
		cv::Point coord = Point(0,0);
		vector<cv::Point>& con = contours[j];
		int h = 0;
		for(int i=0; i<con.size(); i++) {
			coord += con[i];
//			h += hue.at<uchar>(con[i]) * 2;
		}
		coord.x = coord.x / con.size();
		coord.y = coord.y / con.size();
		h = get_hue(hue, contours, j);	
		bool bFound = false;
		for(int b=0; b<numBlobs; b++){
			Point diff_pt = coord - m_vBlob[b].coord;
			double dist = norm(diff_pt);
			if(dist < BLOB_DIST) {
				m_vBlob[b].hue = h;
				m_vBlob[b].coord = coord;
				m_vBlob[b].counter ++;
				m_vBlob[b].willRemove = false;
				m_vBlob[b].detect_color();
				sprintf(strPos, "%c%d", 'A'+coord.y/ m_ygap, coord.x/ m_xgap);
				m_vBlob[b].position = strPos;
				bFound = true;
				
				if(m_vBlob[b].counter==(int)m_fps){
					m_vBlob[b].bOn = true;
					str.Printf("LED ON hue %d--%s, %s\n", h, m_vBlob[b].color, m_vBlob[b].position);
					ShowMessage(str);				
				}
				break;
			}			
		}
		if(bFound == false) {				
			Blob blob(coord, h);
			m_vBlob.push_back(blob);
//			ShowMessage("add %d\n", blob.id);
		}
	}
	
	
}
int MainFrame::get_hue(cv::Mat &hue, vector<vector<cv::Point>>& contours, int idx)
{
	Mat mMask = Mat::zeros( hue.size(), CV_8UC1);
	cv::drawContours(mMask, contours, idx, cv::Scalar(255,255,255), CV_FILLED );	
	
	// Quantize the hue to 30 levels and the saturation to 32 levels
	int hbins = 180;//, sbins = 32;
	int histSize[] = {hbins}; //, sbins
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to 255 (pure spectrum color)
	// float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges}; //, sranges };
	Mat hist, kde;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = {0}; //, 1};
	calcHist( &hue, 1, channels, mMask, // do not use mask
		hist, 1, histSize, ranges,
		true, // the histogram is uniform
		false );
		
	kde = Mat::zeros( hist.size(), CV_32F);
	for( int h = 0; h < hbins; h++ ){
		float binVal = hist.at<float>(h, 0);
		for(int x=-m_nKnlBW; x<=m_nKnlBW; x++) {
			float p = m_kernel[x +m_nKnlBW];
			p *= binVal;	
			if(h+x <0) 
				kde.at<float>(h+x+hbins, 0) += p;
			else if(h+x >hbins)
				kde.at<float>(h+x-hbins, 0) += p;
			else
				kde.at<float>(h+x, 0) += p;
			
		}
	} 
	Point  maxIdx;
//	minMaxLoc(hist, 0, 0, 0, &maxIdx);
//	int imax = maxIdx.y ;
//	fprintf(fp, "%d,", imax);
//	for( int h = 0; h < hbins; h++ ){
//		float binVal = hist.at<float>(h, 0);	
//		fprintf(fp, "%.2f,", binVal);
//	}
//	fprintf(fp, "\n");
	
	minMaxLoc(kde, 0, 0, 0, &maxIdx);
	int imax = maxIdx.y *2;
//	fprintf(fp, "%d,", imax);
//	
//	for( int h = 0; h < hbins; h++ ){
//		float binKde = kde.at<float>(h, 0);
//		fprintf(fp, "%.2f,", binKde);
//	}		
//	fprintf(fp, "\n");

	return imax;
}
void MainFrame::remove_blobs()
{
	wxString str;
	
	for(int j=0; j<m_vBlob.size(); j++) {
		if(m_vBlob[j].willRemove) {
			if(m_vBlob[j].bOn){
				str.Printf("LED OFF hue %d--%s, %s\n", 
					m_vBlob[j].hue, m_vBlob[j].color, m_vBlob[j].position);
				ShowMessage(str);			
			}
//			ShowMessage("remove %d\n", m_vBlob[j].id);	
			m_vBlob.erase (m_vBlob.begin()+j);
			j--;
			continue;			
		}
	}
}

int MainFrame::find_blobs(cv::Mat &img, vector<vector<cv::Point>>& contours)
{
//	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	int mode = CV_RETR_LIST;
	int method = CV_CHAIN_APPROX_NONE;
	cv::Mat m = img.clone();
	findContours(m, contours, hierarchy, mode, method);
	int areaLower = 50;
	int areaUpper = 1500;

	for(int j=0; j<contours.size(); j++) {
		float area = cv::contourArea(contours[j]);
		if(area < areaLower || area > areaUpper) {
			contours.erase (contours.begin()+j);
			j--;
			continue;			
		}
	}
	cv::drawContours(img_input, contours, -1, cv::Scalar(0,255,0), 2);	
	int numCont = contours.size();
	wxString str;
	str.Printf("CC %d", numCont);
	m_statusBar->SetStatusText(str, 3);
	
	return numCont;
}
void MainFrame::draw_grid(cv::Mat &img)
{


	char  box_text[10];
	for(int i=0; i<m_num_grid; i++) {
		cv::line(img, cv::Point(m_xgap * i, 0), cv::Point(m_xgap * i, m_nHeight), cv::Scalar(0, 255, 255), 1);
		cv::line(img, cv::Point(0, m_ygap * i), cv::Point(m_nWidth, m_ygap * i), cv::Scalar(0, 255, 255), 1);
		
		sprintf(box_text, "%d", i);
		cv::putText(img, box_text, cv::Point(m_xgap * i+30, 20), cv::FONT_HERSHEY_DUPLEX, 0.6, CV_RGB(0,255,0), 1.0);
		sprintf(box_text, "%c", i+'A');
		cv::putText(img, box_text, cv::Point(10, m_ygap * i+30), cv::FONT_HERSHEY_DUPLEX, 0.6, CV_RGB(0,255,0), 1.0);		
	}	
}
void MainFrame::OnClose(wxCloseEvent& event)
{
	m_timerVideo->Stop();
	Destroy(); 
}
