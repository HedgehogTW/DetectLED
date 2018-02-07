#include "MainFrame.h"
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

MainFrame *MainFrame::m_pThis=NULL;

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	m_pThis = this;
	int statusWidth[4] = {250, 150, 150};
    m_statusBar->SetFieldsCount(3, statusWidth);

}

MainFrame::~MainFrame()
{
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
	wxString wildcard = "BMP and JPG files (*.bmp;*.jpg;*.jpeg)|*.bmp;*.jpg;*.jpeg|\
	PNG files (*.png)|*.png|\
	All supported graphic formats(*.bmp;*.jpg;*.jpeg;*.tif;*.png)|*.bmp;*.jpg;*.jpeg;*.tif;*.png|\
	All Files (*.*)|*.*";

	wxFileDialog* OpenDialog = new wxFileDialog(
	    this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
	    wildcard, wxFD_OPEN, wxDefaultPosition);
	OpenDialog->SetFilterIndex(2);
	if (OpenDialog->ShowModal() == wxID_OK) {
	    wxString pathName = OpenDialog->GetPath();
	    wxSize sz = m_scrollWin->setImage(pathName);
	    wxString str;
	    str.Printf("W %d, H %d", sz.GetWidth(), sz.GetHeight());
	    m_statusBar->SetStatusText(str, 1);	
		str.Printf("Open Image:%s\n", pathName);
		ShowMessage(str);

	}
	// Clean up after ourselves
	OpenDialog->Destroy();
	
}
