#include "MyImageWin.h"

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


