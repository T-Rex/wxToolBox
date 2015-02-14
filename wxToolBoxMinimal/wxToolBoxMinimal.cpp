#include "stdwx.h"
#include <wx/wxToolBox/wxToolBox.h>
#if !defined(__WXWINCE__)
#include <wx/wxFlatNotebook/wxFlatNotebook.h>
#include <wx/wxFlatNotebook/renderer.h>
#else
#include <aygshell.h>
#endif

#if defined(__WXMSW__) && !defined(__WXWINCE__)
#define SUPPORTS_TAB_CHILD_CONTROLS
#endif

class wxToolBoxMinimalTestApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(wxToolBoxMinimalTestApp)
IMPLEMENT_APP(wxToolBoxMinimalTestApp)

class wxToolBoxMinimalTestFrame : public wxFrame
{
	wxToolBox * m_ToolBox;
	wxGauge * m_tab4gauge;
	wxTimer * m_gaugetimer;
public:
	wxToolBoxMinimalTestFrame();
	~wxToolBoxMinimalTestFrame();
	//bool Create(wxWindow * parent, wxWindowID id = wxID_ANY,
	//	const wxString & title = wxEmptyString);
	void CreateToolBox();
	void OnGaugeTimer(wxTimerEvent & event);
	void OnSubmit(wxCommandEvent & event);
};

wxToolBoxMinimalTestFrame::wxToolBoxMinimalTestFrame()
 : wxFrame(NULL, wxID_ANY, _("wxToolBox Minimal"))
{
	//Create(NULL, -1, _("wxToolBox Minimal"));
	SetBackgroundColour(*wxRED);
}

wxToolBoxMinimalTestFrame::~wxToolBoxMinimalTestFrame()
{
	wxDELETE(m_gaugetimer);
#if !defined(__WXWINCE__)
	wxFNBRendererMgrST::Free();
#endif
}

/*bool wxToolBoxMinimalTestFrame::Create(wxWindow * parent,
		wxWindowID id, const wxString & title)
{
    wxMessageBox(_("Attach to process"));
	bool res = wxFrame::Create(parent, id, title, wxDefaultPosition, wxSize(220, 500), wxDEFAULT_FRAME_STYLE | wxCAPTION);
	if(res)
	{
		//CreateToolBox();
		long timer_id = wxNewId();
		m_gaugetimer = new wxTimer(this, timer_id);
		Connect(timer_id, wxEVT_TIMER,
			wxObjectEventFunction(&wxToolBoxMinimalTestFrame::OnGaugeTimer));
	}
	return res;
}
*/
void wxToolBoxMinimalTestFrame::OnGaugeTimer(wxTimerEvent & event)
{
	if(m_tab4gauge->GetValue() >= 100)
	{
		m_tab4gauge->SetValue(0);
	}
	else
	{
		m_tab4gauge->SetValue(m_tab4gauge->GetValue() + 10);
	}
}

void wxToolBoxMinimalTestFrame::OnSubmit(wxCommandEvent & event)
{
	if(m_gaugetimer->IsRunning())
	{
		m_gaugetimer->Stop();
	}
	else
	{
		m_gaugetimer->Start(500);
	}
}

void wxToolBoxMinimalTestFrame::CreateToolBox()
{
	m_ToolBox = new wxToolBox(this, 10001, wxDefaultPosition,
		wxSize(220,465), wxNO_BORDER);
	m_ToolBox->SetMinSize(wxSize(220,465));
	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(m_ToolBox, 1, wxEXPAND);
	sizer->Fit(this);
	Centre();
//#if !defined(__WXWINCE__)
	wxSetWorkingDirectory(wxPathOnly(wxGetApp().argv[0]));
	wxLogDebug(wxString::Format(wxT("Working directory: %s"), wxGetCwd().c_str()));
	m_ToolBox->LoadSkin(wxPathOnly(wxGetApp().argv[0])+ wxFILE_SEP_PATH + wxT("Standard.xml"));
//#endif
	wxFont font = m_ToolBox->GetFont();
	m_ToolBox->SetItemNormalFont(font);
	font.SetWeight(wxFONTWEIGHT_BOLD);
	m_ToolBox->SetItemHoverFont(font);

	time_t t;
	time(&t);
	srand(t);
	int icon(0);
	wxToolBoxTab * tab1 = new wxToolBoxTab(wxT("Tab 1"));
#if !defined(__WXWINCE__)
	icon = rand()%m_ToolBox->GetTabImageList().Count();
	tab1->SetImageIndex(icon);
#else
	icon = -1;
#endif;
	wxToolBoxTab * tab2 = new wxToolBoxTab(wxT("Tab 2"));
#if defined(SUPPORTS_TAB_CHILD_CONTROLS)
	wxToolBoxTab * tab3 = new wxToolBoxTab(wxT("Tab 3"));
#endif
#if defined(SUPPORTS_TAB_CHILD_CONTROLS)
	icon = rand()%m_ToolBox->GetTabImageList().Count();
#else
	icon = -1;
#endif;

#if defined(SUPPORTS_TAB_CHILD_CONTROLS)
	tab3->SetImageIndex(icon);

	wxNotebook * notebook = new wxNotebook(m_ToolBox, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	notebook->SetBackgroundColour(m_ToolBox->GetBackgroundColour());
	wxTextCtrl * textCtrl = new wxTextCtrl(notebook, wxID_ANY, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER);
	wxArrayString listBoxItems;
	listBoxItems.Add(_("Item 1"));
	listBoxItems.Add(_("Item 2"));
	listBoxItems.Add(_("Item 3"));
	wxListBox * listBox = new wxListBox(notebook, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, listBoxItems, wxNO_BORDER);
	notebook->AddPage(textCtrl, _("Text"));
	notebook->AddPage(listBox, _("List"));
	tab3->SetChildWindow(notebook);
#endif

	m_ToolBox->AddTab(tab1);
	m_ToolBox->AddTab(tab2);
#if defined(SUPPORTS_TAB_CHILD_CONTROLS)
	m_ToolBox->AddTab(tab3);
#endif
	m_ToolBox->SetStyle(wxTB_STYLE_LIST);
	
	wxToolBoxItem item1;

	for(int i = 0; i < 60; i++)
	{
		item1.SetID(i+1);
		item1.SetCaption(wxString::Format(_("My Thumbnail #%i"), i));
		if(m_ToolBox->GetItemImageList().Count())
		{
//#if !defined(__WXWINCE__)
			icon = rand()%m_ToolBox->GetTabImageList().Count();
//#else
			//icon = -1;
//#endif;
			item1.SetImageIndex(icon);
		}
		if(i < 30)
		{
			tab1->AddItem(item1);
		}
		else
		{
			tab2->AddItem(item1);
		}
	}
#if !defined(SUPPORTS_TAB_CHILD_CONTROLS)
	return;
#endif
	wxToolBoxTab * tab4 = new wxToolBoxTab(wxT("Tab 4"));
#if defined(SUPPORTS_TAB_CHILD_CONTROLS)
	icon = rand()%m_ToolBox->GetTabImageList().Count();
#else
	icon = -1;
#endif;
	tab4->SetImageIndex(icon);
	wxPanel * tab4panel = new wxPanel(m_ToolBox, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	wxBoxSizer *tab4panel_sizer = new wxBoxSizer(wxVERTICAL);
	tab4panel->SetSizer(tab4panel_sizer);
	wxFlexGridSizer * flexgrid_sizer = new wxFlexGridSizer(5, 2, 5, 5);
	wxStaticText * fn_label = new wxStaticText(tab4panel, wxID_ANY, _("First Name:"));
	wxStaticText * ln_label = new wxStaticText(tab4panel, wxID_ANY, _("Last Name:"));
	wxStaticText * em_label = new wxStaticText(tab4panel, wxID_ANY, _("e-mail:"));
	wxTextCtrl * fn_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);
	wxTextCtrl * ln_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);
	wxTextCtrl * em_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);
	fn_label->SetForegroundColour(*wxWHITE);
	ln_label->SetForegroundColour(*wxWHITE);
	em_label->SetForegroundColour(*wxWHITE);
	flexgrid_sizer->AddGrowableCol(1, 1);
	flexgrid_sizer->Add(fn_label, 0, wxALIGN_LEFT);
	flexgrid_sizer->Add(fn_text, 0, wxGROW);
	flexgrid_sizer->Add(ln_label, 0, wxALIGN_LEFT);
	flexgrid_sizer->Add(ln_text, 0, wxGROW);
	flexgrid_sizer->Add(em_label, 0, wxALIGN_LEFT);
	flexgrid_sizer->Add(em_text, 0, wxGROW);
	m_tab4gauge = new wxGauge(tab4panel, wxID_ANY, 100,
		wxDefaultPosition, wxSize(-1,20),
		wxHORIZONTAL|wxGA_SMOOTH|wxNO_BORDER);
	long submit_id = wxNewId();
	wxButton * submit_btn = new wxButton(tab4panel, submit_id, _("Submit"));
	Connect(submit_id, wxEVT_COMMAND_BUTTON_CLICKED,
			wxObjectEventFunction(&wxToolBoxMinimalTestFrame::OnSubmit));
	tab4panel_sizer->Add(flexgrid_sizer, 0, wxGROW|wxALL, 5);
	tab4panel_sizer->Add(m_tab4gauge, 0, wxGROW|wxALL, 5);
	tab4panel_sizer->Add(submit_btn, 0, wxGROW|wxALL, 5);
	tab4->SetChildWindow(tab4panel);

	wxToolBoxTab * tab5 = new wxToolBoxTab(wxT("Tab 5"));
	wxTreeCtrl * treeCtrl = new wxTreeCtrl(m_ToolBox, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	wxTreeItemId root = treeCtrl->AddRoot(_("Root"));
	treeCtrl->AppendItem(root, _("Item 1"));
	treeCtrl->AppendItem(root, _("Item 2"));
	treeCtrl->AppendItem(root, _("Item 3"));
	treeCtrl->Expand(root);
	tab5->SetChildWindow(treeCtrl);

	m_ToolBox->AddTab(tab4);
	m_ToolBox->AddTab(tab5);

	tab2->SetSelected(true);

	m_ToolBox->RefreshTabs();
}

bool wxToolBoxMinimalTestApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxXPMHandler());
	wxImage::AddHandler(new wxJPEGHandler());
	wxToolBoxMinimalTestFrame * frame = new wxToolBoxMinimalTestFrame;
	SetTopWindow(frame);
	frame->Centre();
	frame->ShowFullScreen(true);
	wxYield();
	frame->CreateToolBox();
#if defined(__WXWINCE__)
	SHFullScreen((HWND)frame->GetHWND(), SHFS_HIDESIPBUTTON|SHFS_HIDETASKBAR|SHFS_HIDESTARTICON);
#endif
	return true;
}
