#include "stdwx.h"
#include "wxToolBoxSkinEditorApp.h"
#include "ToolBoxTestFrame.h"
#include "AdditionalProperties.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	//wxPGRegisterPropertyClass(wxToolObjectBackgroundInfoProperty);

	wxImage::AddHandler( new wxXPMHandler );
	wxImage::AddHandler( new wxPNGHandler );
	ToolBoxTestFrame * frame = new ToolBoxTestFrame(NULL);	
	frame->Show();	
	SetTopWindow(frame);
	return true;
}
