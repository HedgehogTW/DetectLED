#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include <wx/filehistory.h>


using namespace std;
using namespace cv;

extern int blob_id;
class Blob
{
public:
	
	Blob() { 
		coord = Point(0,0);
		hue = 0;
		counter = 0;
		willRemove = false;
		bOn = false;
		id = 0;
	}
	Blob(cv::Point c, int h) {
		coord = c;
		hue = h;
		counter = 0;
		willRemove = false;
		bOn = false;
		id = ++blob_id;
		detect_color();
	}
	void detect_color() {
		int diff_cyan = abs(hue-180);
		int diff_red = abs(hue - 360);
		int diff_red1 = abs(hue);
		if(diff_cyan < diff_red && diff_cyan < diff_red1)
			color = "blue";
		else 
			color = "red";
	}
	int id;
	cv::Point coord;
	int hue;
	int counter;
	bool willRemove;
	bool bOn;
	string color;
	string position;
};

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
	void PlayVideoClip();
	void draw_grid(cv::Mat &img);
	void img_process(cv::Mat &img);
	int  find_blobs(cv::Mat &img, vector<vector<cv::Point>>& contours);
	void identify_blobs(vector<vector<cv::Point>>&contours, cv::Mat &hue);
	void remove_blobs();
	int  get_hue(cv::Mat &hue, vector<vector<cv::Point>>& contours, int idx);


	vector<Blob>  m_vBlob;
	wxFileHistory*    m_FileHistory;
	wxString m_filename;
	std::string 	m_DataPath;
	int 	m_frameNumber;
	int 	m_nWidth;
	int		m_nHeight;
	double  m_fps;
	int 	m_num_grid;
	int 	m_xgap ;
	int 	m_ygap ;
	
	std::vector<float> m_kernel;
	int	m_nKnlBW;
	
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnMRUFile(wxCommandEvent& event);
	static MainFrame* m_pThis;
protected:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnVideoTimer(wxTimerEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnFileStop(wxCommandEvent& event);
    virtual void OnFilePlay(wxCommandEvent& event);
//    virtual void OnClose(wxCloseEvent& event);
    virtual void OnFilePause(wxCommandEvent& event);
    virtual void OnFileOpen(wxCommandEvent& event);
	
//	void OnThreadFinished(wxThreadEvent& evt);
//    virtual wxThread::ExitCode Entry();	
};
#endif // MAINFRAME_H
