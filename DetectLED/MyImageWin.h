#ifndef MYIMAGEWIN_H
#define MYIMAGEWIN_H

#include <wx/dc.h>
#include <wx/string.h>
#include <wx/msgdlg.h> 
#include <wx/scrolwin.h> // Base class: wxScrolledWindow
#include <wx/gdicmn.h>
#include <opencv2/opencv.hpp>

class MyImageWin : public wxScrolledWindow
{
public:
	MyImageWin(wxWindow *parent, wxWindowID id=-1, 
        const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, long style=wxHSCROLL|wxVSCROLL);

	~MyImageWin();

//    wxSize setImage(wxString image_path);
	void setImage(cv::Mat& mat);
    void OnDraw(wxDC& dc);
private:	
    wxBitmap* m_pBitmap;
    int 	m_nWidth;
    int 	m_nHeight;

};

#endif // MYIMAGEWIN_H
