#include "stdwx.h"
#include "wxToolBoxSampleIDEApp.h"
#include "wxToolBoxSampleIDEMainFrame.h"

IMPLEMENT_APP(wxToolBoxSampleIDEApp);

bool wxToolBoxSampleIDEApp::OnInit()
{
	wxImage::AddHandler( new wxXPMHandler );
	wxImage::AddHandler( new wxPNGHandler );
	wxImage::AddHandler( new wxICOHandler );
	wxToolBoxSampleIDEMainFrame * frame = new wxToolBoxSampleIDEMainFrame(NULL, -1, _("Sample IDE"));
	SetTopWindow(frame);
	frame->Show();
	frame->Maximize();
	return true;
}
