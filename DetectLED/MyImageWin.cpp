#include "MyImageWin.h"
#include <wx/dcclient.h>
#include <wx/log.h> 
#include "MainFrame.h"

MyImageWin::MyImageWin(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style): wxScrolledWindow(parent, id)
{
    m_pBitmap = NULL;
}

MyImageWin::~MyImageWin()
{
    if(m_pBitmap != NULL)	delete m_pBitmap;
}

void MyImageWin::OnDraw(wxDC& dc)
{
    if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);
}
wxSize MyImageWin::setImage(wxString image_path)
{
    wxImage im(image_path);
    if(!im.IsOk()){
        wxMessageBox(wxT("there was an error loading the image"));
        return wxDefaultSize;
    }
    m_nWidth = im.GetWidth();
    m_nHeight = im.GetHeight();
		
    if(m_pBitmap)  delete m_pBitmap;
    m_pBitmap = new wxBitmap( im );	

    SetVirtualSize( m_nWidth, m_nHeight );
    Refresh();
    wxSize sz = m_pBitmap->GetSize();
    return sz;
}


void MyImageWin::setImage(cv::Mat& mat)
{
    wxImage wxIm;
    cv::Mat  rgbOutput;
    int type = mat.type();
    int channel = mat.channels();
    bool ret = false;
	cv::rectangle(mat, cv::Point(170, 2), cv::Point(315, 214), cv::Scalar(0, 0, 255), 2);
    if(type ==CV_8UC1) {
        cvtColor(mat, rgbOutput, CV_GRAY2RGB);
        ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);	
    }else if(type == CV_8UC3) {
        cvtColor(mat, rgbOutput, CV_BGR2RGB);
		ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);
    }else if(channel ==1){
		cv::Mat  m8UC1;
		double min, max, a;
		cv::minMaxLoc(mat, &min, &max);
		a = 255./(max - min);
		mat.convertTo(m8UC1, CV_8UC1, a, -min*a );
		cv::cvtColor(m8UC1, rgbOutput, CV_GRAY2RGB);
		ret = wxIm.Create(mat.cols, mat.rows, rgbOutput.data, true);
    }
    if(ret) {
		if(m_pBitmap)  delete m_pBitmap;
		m_pBitmap = new wxBitmap( wxIm );	
		int w = wxIm.GetWidth();
		int h = wxIm.GetHeight();
		SetVirtualSize( w, h );
//		wxClientDC dc(this);
//		if(m_pBitmap)	dc.DrawBitmap(*m_pBitmap, 0, 0, false);

		Refresh();
		MainFrame::ShowMessage( "MyImageWin::setImage\n");
    }else wxLogMessage(wxT("wxIm.Create failed"));	

}