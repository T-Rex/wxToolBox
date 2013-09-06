#include "wxToolBoxSampleIDEMainFrame.h"
#include <wx/wxFlatNotebook/renderer.h>

enum
{
	ID_TOOLBOX = 10001,
	ID_NAVIGATION_NOTEBOOK,
	ID_CLASS_VIEW_TREE_CTRL,
	ID_RESOURCE_VIEW_TREE_CTRL,
	ID_SOLUTION_VIEW_TREE_CTRL,
	ID_INFO_NOTEBOOK,
	ID_OUTPUT_TEXT_CTRL,
	ID_TASK_LIST_CTRL,
	ID_CONTENT_NOTEBOOK,
	ID_SOURCE_EDITOR,
	ID_GUI_EDITOR,

	ID_TOGGLE_STANDARD_TOOLBAR,
	ID_TOGGLE_PROJECT_TOOLBAR,
	ID_TOGGLE_STATUSBAR,
	ID_TOGGLE_INFO_PANE,
	ID_TOGGLE_TOOLBOX,
	ID_TOGGLE_NAVIGATION_PANE,

	ID_ADD_CLASS,
	ID_ADD_RESOURCE,
	ID_ADD_NEW_ITEM,
	ID_ADD_EXISTING_ITEM,
	ID_NEW_FOLDER,

	ID_LAYOUT,
	ID_COLORS,
	ID_NEW_ITEM,
	ID_FONTS,
	ID_MOVE_UP,
	ID_MOVE_DOWN,
	ID_SETTINGS,
	ID_PUBLISH,

	ID_BUILD,

	ID_TOOL_NOTEPAD,

	ID_UNUSED
};

BEGIN_EVENT_TABLE(wxToolBoxSampleIDEMainFrame, wxFrame)
EVT_MENU(ID_TOGGLE_STATUSBAR, wxToolBoxSampleIDEMainFrame::OnToggleStatusBar)
EVT_MENU(ID_TOGGLE_TOOLBOX, wxToolBoxSampleIDEMainFrame::OnToggleToolBox)
EVT_MENU(ID_TOGGLE_NAVIGATION_PANE, wxToolBoxSampleIDEMainFrame::OnToggleNavigation)
EVT_MENU(ID_TOGGLE_INFO_PANE, wxToolBoxSampleIDEMainFrame::OnToggleInfo)
EVT_MENU(ID_TOGGLE_STANDARD_TOOLBAR, wxToolBoxSampleIDEMainFrame::OnToggleStandardToolBar)
EVT_MENU(ID_TOGGLE_PROJECT_TOOLBAR, wxToolBoxSampleIDEMainFrame::OnToggleProjectToolBar)
EVT_MENU(wxID_OPEN, wxToolBoxSampleIDEMainFrame::OnOpen)
EVT_MENU(wxID_SAVE, wxToolBoxSampleIDEMainFrame::OnSave)
EVT_MENU(wxID_EXIT, wxToolBoxSampleIDEMainFrame::OnExit)
EVT_UPDATE_UI(ID_TOGGLE_PROJECT_TOOLBAR, wxToolBoxSampleIDEMainFrame::OnMenuItemUpdateUI)
EVT_UPDATE_UI(ID_TOGGLE_STANDARD_TOOLBAR, wxToolBoxSampleIDEMainFrame::OnMenuItemUpdateUI)

EVT_MENU(ID_TOOL_NOTEPAD, wxToolBoxSampleIDEMainFrame::OnToolNotepad)

END_EVENT_TABLE()

wxToolBoxSampleIDEMainFrame::wxToolBoxSampleIDEMainFrame(wxWindow * parent, wxWindowID id, wxString title)
: wxFrame(parent, id, title, wxDefaultPosition, wxSize(780, 540))
{
#ifdef __WXMSW__
	SetIcon(wxIcon(wxT("MAINICON"), wxBITMAP_TYPE_ICO_RESOURCE));
#else
#endif
	//m_NavigationImageList.Create(16, 16);
	m_NavigationImageList.push_back(wxBitmap(wxT("tabs/class_view.xpm"), wxBITMAP_TYPE_XPM));
	m_NavigationImageList.push_back(wxBitmap(wxT("tabs/resource_view.xpm"), wxBITMAP_TYPE_XPM));
	m_NavigationImageList.push_back(wxBitmap(wxT("tabs/solution_explorer.xpm"), wxBITMAP_TYPE_XPM));
	m_NavigationImageList.push_back(wxBitmap(wxT("tabs/task_list.xpm"), wxBITMAP_TYPE_XPM));
	m_NavigationImageList.push_back(wxBitmap(wxT("tabs/output.xpm"), wxBITMAP_TYPE_XPM));

	m_FrameManager.SetManagedWindow(this);

	wxMenuBar * menuBar = new wxMenuBar;
	SetMenuBar(menuBar);

	wxMenu * fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, _("New\tCtrl+N"));
	fileMenu->Append(wxID_OPEN, _("Open\tCtrl+O"));
	fileMenu->Append(wxID_SAVE, _("Save\tCtrl+S"));
	fileMenu->Append(wxID_SAVEAS, _("Save as..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_PRINT_SETUP, _("Page Setup..."));
	fileMenu->Append(wxID_PRINT, _("Print\tCtrl+P"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit\tAlt+F4"));

	wxMenu * editMenu = new wxMenu;
	editMenu->Append(wxID_UNDO, _("Undo\tCtrl+Z"));
	editMenu->Append(wxID_REDO, _("redo\tCtrl+Y"));
	editMenu->AppendSeparator();
	editMenu->Append(wxID_CUT, _("Cut\tCtrl+X"));
	editMenu->Append(wxID_COPY, _("Copy\tCtrl+C"));
	editMenu->Append(wxID_PASTE, _("Paste\tCtrl+V"));
	editMenu->Append(wxID_DELETE, _("Delete\tDel"));
	editMenu->AppendSeparator();
	editMenu->Append(wxID_FIND, _("Find\tCtrl+F"));
	editMenu->Append(wxID_REPLACE, _("Replace\tCtrl+H"));

	wxMenu * viewMenu = new wxMenu;
	wxMenu * statusbarMenu = new wxMenu;
	statusbarMenu->AppendCheckItem(ID_TOGGLE_STANDARD_TOOLBAR, _("Standard"));
	statusbarMenu->AppendCheckItem(ID_TOGGLE_PROJECT_TOOLBAR, _("Project"));
	viewMenu->Append(wxID_ANY, _("Tool Bars"), statusbarMenu);
	viewMenu->AppendCheckItem(ID_TOGGLE_STATUSBAR, _("Status Bar"));
	viewMenu->AppendSeparator();
	viewMenu->AppendCheckItem(ID_TOGGLE_TOOLBOX, _("ToolBox"));
	viewMenu->AppendCheckItem(ID_TOGGLE_INFO_PANE, _("Information pane"));
	viewMenu->AppendCheckItem(ID_TOGGLE_NAVIGATION_PANE, _("Navigation pane"));
	viewMenu->Check(ID_TOGGLE_STANDARD_TOOLBAR, true);
	viewMenu->Check(ID_TOGGLE_PROJECT_TOOLBAR, true);
	viewMenu->Check(ID_TOGGLE_STATUSBAR, true);
	viewMenu->Check(ID_TOGGLE_TOOLBOX, true);
	viewMenu->Check(ID_TOGGLE_INFO_PANE, true);
	viewMenu->Check(ID_TOGGLE_NAVIGATION_PANE, true);

	wxMenu * projectMenu = new wxMenu;
	projectMenu->Append(ID_ADD_CLASS, _("Add Class..."));
	projectMenu->Append(ID_ADD_RESOURCE, _("Add Resource..."));
	projectMenu->AppendSeparator();
	projectMenu->Append(ID_ADD_NEW_ITEM, _("Add New Item..."));
	projectMenu->Append(ID_ADD_EXISTING_ITEM, _("Add Existing Item..."));
	projectMenu->AppendSeparator();
	projectMenu->Append(ID_NEW_FOLDER, _("New Folder"));

	wxMenu * buildMenu = new wxMenu;
	buildMenu->Append(ID_BUILD, _("Build\tF9"));

	wxMenu * toolsMenu = new wxMenu;
	toolsMenu->Append(ID_TOOL_NOTEPAD, _("Notepad"));

	wxMenu * helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, _("Index\tF1"));
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_ABOUT, _("About..."));

	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(projectMenu, _("Project"));
	menuBar->Append(buildMenu, _("Build"));
	menuBar->Append(toolsMenu, _("Tools"));
	menuBar->Append(helpMenu, _("Help"));

	CreateStatusBar(2);

	wxToolBar * stdToolBar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER|wxTB_TEXT);
	stdToolBar->SetToolBitmapSize(wxSize(24, 24));
	stdToolBar->AddTool(wxID_NEW, wxT("New"), wxBitmap(wxT("icons/new.ico"), wxBITMAP_TYPE_ICO));
	stdToolBar->AddTool(wxID_OPEN, wxT("Open"), wxBitmap(wxT("icons/open.ico"), wxBITMAP_TYPE_ICO));
	stdToolBar->AddTool(wxID_SAVE, wxT("Save"), wxBitmap(wxT("icons/save.ico"), wxBITMAP_TYPE_ICO));
	stdToolBar->Realize();

	wxToolBar * addToolBar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_NODIVIDER|wxTB_TEXT);
	addToolBar->SetToolBitmapSize(wxSize(24, 24));
	addToolBar->AddTool(ID_LAYOUT, wxT("Layout"), wxBitmap(wxT("icons/layout.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_COLORS, wxT("Colors"), wxBitmap(wxT("icons/colors.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_FONTS, wxT("Fonts"), wxBitmap(wxT("icons/fonts.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_FONTS, wxT("New Item"), wxBitmap(wxT("icons/newitem.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_MOVE_UP, wxT("Move Up"), wxBitmap(wxT("icons/moveup.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_MOVE_DOWN, wxT("Move Down"), wxBitmap(wxT("icons/movedown.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(wxID_DELETE, wxT("Delete"), wxBitmap(wxT("icons/delete.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_SETTINGS, wxT("Settings"), wxBitmap(wxT("icons/settings.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->AddTool(ID_PUBLISH, wxT("Publish"), wxBitmap(wxT("icons/publish.ico"), wxBITMAP_TYPE_ICO));
	addToolBar->Realize();

	CreateToolBox();
	CreateNavigationNotebook();
	CreateInfoNotebook();
	CreateContentNotebook();

	m_FrameManager.AddPane(stdToolBar, wxAuiPaneInfo().
                  Name(wxT("Standard")).Caption(wxT("Standard")).
                  ToolbarPane().Top().
                  LeftDockable(true).RightDockable(true));
	m_FrameManager.AddPane(addToolBar, wxAuiPaneInfo().
                  Name(wxT("Project")).Caption(wxT("Project")).
                  ToolbarPane().Top().Row(0).
                  LeftDockable(true).RightDockable(true));
	m_FrameManager.AddPane(m_NavigationNotebook, wxAuiPaneInfo().
                  Name(wxT("Navigation")).Caption(wxT("Navigation")).
                  Left().Layer(1).Position(1));
	m_FrameManager.AddPane(m_InfoNotebook, wxAuiPaneInfo().
                  Name(wxT("Info")).Caption(wxT("Info")).
                  Bottom().Layer(1).Position(1));
	m_FrameManager.AddPane(m_ToolBox, wxAuiPaneInfo().
                  Name(wxT("ToolBox")).Caption(wxT("ToolBox")).
                  Right().Layer(1).Position(1));
	m_FrameManager.AddPane(m_ContentNotebook, wxAuiPaneInfo().Name(wxT("Center Pane")).
                  CenterPane());
	m_FrameManager.Update();

	Centre();
}

void wxToolBoxSampleIDEMainFrame::CreateContentNotebook()
{
	m_ContentNotebook = new wxFlatNotebook(this, ID_CONTENT_NOTEBOOK, wxDefaultPosition,
		wxSize(200, 400), wxNO_BORDER);
	m_ContentNotebook->SetWindowStyleFlag(wxFNB_VC71);
	m_SourceEditor = new wxTextCtrl(m_ContentNotebook, ID_SOURCE_EDITOR, wxEmptyString,
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	m_GUIEditor = new wxPanel(m_ContentNotebook, ID_GUI_EDITOR, wxDefaultPosition,
		wxDefaultSize, wxNO_BORDER);
	m_ContentNotebook->AddPage(m_SourceEditor, _("Source"), true);
	m_ContentNotebook->AddPage(m_GUIEditor, _("Design"), false);
}

void wxToolBoxSampleIDEMainFrame::CreateNavigationNotebook()
{	
	m_NavigationNotebook = new wxFlatNotebook(this, ID_NAVIGATION_NOTEBOOK, wxDefaultPosition,
		wxSize(200, 400), wxNO_BORDER);
	m_NavigationNotebook->SetWindowStyleFlag(wxFNB_VC71|wxFNB_TABS_BORDER_SIMPLE|wxFNB_BOTTOM);
	m_NavigationNotebook->SetImageList(&m_NavigationImageList);
	m_NavigationNotebook->SetMinSize(wxSize(200, 400));
	m_ClassViewTreeCtrl = new wxTreeCtrl(m_NavigationNotebook, ID_CLASS_VIEW_TREE_CTRL);
	m_ResourceViewTreeCtrl = new wxTreeCtrl(m_NavigationNotebook, ID_RESOURCE_VIEW_TREE_CTRL);
	m_SolutionViewTreeCtrl = new wxTreeCtrl(m_NavigationNotebook, ID_SOLUTION_VIEW_TREE_CTRL);
	m_NavigationNotebook->AddPage(m_ClassViewTreeCtrl, _("Class View"), true, 0);
	m_NavigationNotebook->AddPage(m_ResourceViewTreeCtrl, _("Resource View"), false, 1);
	m_NavigationNotebook->AddPage(m_SolutionViewTreeCtrl, _("Solution Explorer"), false, 2);
}

void wxToolBoxSampleIDEMainFrame::CreateInfoNotebook()
{
	m_InfoNotebook = new wxFlatNotebook(this, ID_INFO_NOTEBOOK, wxDefaultPosition,
		wxSize(400, 150), wxNO_BORDER);
	m_InfoNotebook->SetWindowStyleFlag(wxFNB_VC71|wxFNB_TABS_BORDER_SIMPLE|wxFNB_BOTTOM);
	m_InfoNotebook->SetImageList(&m_NavigationImageList);
	m_OutputTextCtrl = new wxTextCtrl(m_InfoNotebook, ID_OUTPUT_TEXT_CTRL, wxEmptyString, 
		wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	m_TaskListCtrl = new wxListCtrl(m_InfoNotebook, ID_TASK_LIST_CTRL, wxDefaultPosition,
		wxDefaultSize, wxLC_REPORT|wxLC_HRULES|wxLC_VRULES);	
	m_TaskListCtrl->InsertColumn(0, wxT("!"), wxLIST_FORMAT_LEFT, 16);
	m_TaskListCtrl->InsertColumn(1, wxEmptyString, wxLIST_FORMAT_LEFT, 16);
	m_TaskListCtrl->InsertColumn(2, wxEmptyString, wxLIST_FORMAT_LEFT, 16);
	m_TaskListCtrl->InsertColumn(3, wxT("Description"), wxLIST_FORMAT_LEFT, 300);
	m_TaskListCtrl->InsertColumn(4, wxT("File"), wxLIST_FORMAT_LEFT, 300);
	m_TaskListCtrl->InsertItem(0, wxEmptyString);
	m_TaskListCtrl->SetItem(0, 3, _("Test"));
	m_InfoNotebook->AddPage(m_TaskListCtrl, _("Task List"), false, 3);
	m_InfoNotebook->AddPage(m_OutputTextCtrl, _("Output"), true, 4);
}

void wxToolBoxSampleIDEMainFrame::CreateToolBox()
{
	m_ToolBox = new wxToolBox(this, ID_TOOLBOX, wxDefaultPosition, wxSize(200,400), wxNO_BORDER);	
	m_ToolBox->SetMinSize(wxSize(200,400));	
	m_ToolBox->CreateTabImageList(wxT("res/TabIcons.png"), 16, 16, false);
	m_ToolBox->CreateItemImageList(wxT("res/ItemIcons.png"), 16, 16, false);
	m_ToolBox->SetItemsTransparent(true);
	m_ToolBox->SetTabHeight(22);
	m_ToolBox->SetItemHeight(22);	
	wxFont tabfont = m_ToolBox->GetFont();
	tabfont.SetWeight(wxFONTWEIGHT_BOLD);
	m_ToolBox->SetTabHoverFont(tabfont);

	m_ToolBox->SetItemNormalColour(wxColour(35, 42, 68));
	m_ToolBox->SetItemHoverColour(wxColour(79, 95, 121));
	m_ToolBox->SetItemSelectedColour(wxColour(146, 151, 164));	

	wxToolBoxTab * tab1 = new wxToolBoxTab(wxT("Tab 1"));
	if(m_ToolBox->GetTabImageList().Count()) tab1->SetImageIndex(rand()%(int)m_ToolBox->GetTabImageList().Count());
	tab1->SetToolTip(_("First tab"));
	wxToolBoxTab * tab2 = new wxToolBoxTab(wxT("Tab 2"));
	tab2->SetToolTip(_("Second tab without icon"));
	m_ToolBox->AddTab(tab1);
	m_ToolBox->AddTab(tab2);

	wxToolBoxItem item1;

	for(int i = 0; i < 60; i++)
	{
		item1.SetID(i+1);
		item1.SetCaption(wxString::Format(_("Item %i"), i));
		item1.SetToolTip(wxString::Format(_("Item %i"), i));
		if(m_ToolBox->GetItemImageList().Count())
		{
			item1.SetImageIndex(rand()%(int)m_ToolBox->GetItemImageList().Count());
		}
		if(i < 30)
		{
			tab1->AddItem(item1);
		}
		else if(i >= 30 && i < 60)
		{
			tab2->AddItem(item1);
		}
	}
}

wxToolBoxSampleIDEMainFrame::~wxToolBoxSampleIDEMainFrame()
{
	m_FrameManager.UnInit();
	wxFNBRendererMgrST::Free();
}

void wxToolBoxSampleIDEMainFrame::OnToggleStatusBar(wxCommandEvent & event)
{
	wxStatusBar * statusBar = GetStatusBar();
	if(!statusBar)
	{
		CreateStatusBar(2);
	}
	else
	{
		SetStatusBar(NULL);
		delete statusBar;
	}
	Layout();
}

void wxToolBoxSampleIDEMainFrame::OnToggleToolBox(wxCommandEvent & event)
{
	TogglePaneVisibility(_("ToolBox"));
}

void wxToolBoxSampleIDEMainFrame::OnToggleNavigation(wxCommandEvent & event)
{
	TogglePaneVisibility(_("Navigation"));
}

void wxToolBoxSampleIDEMainFrame::OnToggleInfo(wxCommandEvent & event)
{
	TogglePaneVisibility(_("Info"));
}

void wxToolBoxSampleIDEMainFrame::OnToggleStandardToolBar(wxCommandEvent & event)
{
	TogglePaneVisibility(_("Standard"));
}

void wxToolBoxSampleIDEMainFrame::OnToggleProjectToolBar(wxCommandEvent & event)
{
	TogglePaneVisibility(_("Project"));
}

void wxToolBoxSampleIDEMainFrame::TogglePaneVisibility(wxString pane_name)
{
	wxAuiPaneInfoArray& all_panes = m_FrameManager.GetAllPanes();
	size_t i, count;
	for (i = 0, count = all_panes.GetCount(); i < count; ++i)
	{		
		if(all_panes.Item(i).name == pane_name)
		{
			if(all_panes.Item(i).IsFloating())
			{
				all_panes.Item(i).frame->Show(!all_panes.Item(i).frame->IsShown());
			}
			else
			{
				all_panes.Item(i).Show(!all_panes.Item(i).IsShown());
			}
			m_FrameManager.Update();
			break;
		}
	}
}

void wxToolBoxSampleIDEMainFrame::OnOpen(wxCommandEvent & event)
{
	wxFileDialog * dlg = new wxFileDialog(this);	
	dlg->SetWildcard(_("Some Project (*.someprj)|*.someprj|All Files (*.*)|*.*"));
	dlg->Centre();
	if(dlg->ShowModal() == wxID_OK)
	{
	}
	dlg->Destroy();
}

void wxToolBoxSampleIDEMainFrame::OnSave(wxCommandEvent & event)
{
	wxFileDialog * dlg = new wxFileDialog(this);
	dlg->SetWildcard(_("Some Project (*.someprj)|*.someprj|All Files (*.*)|*.*"));
	dlg->Centre();
	if(dlg->ShowModal() == wxID_OK)
	{
	}
	dlg->Destroy();
}

void wxToolBoxSampleIDEMainFrame::OnExit(wxCommandEvent & event)
{
	Close();
}

void wxToolBoxSampleIDEMainFrame::OnToolNotepad(wxCommandEvent & event)
{	
#ifdef __WXMSW__
	wxExecute(wxT("notepad.exe"));
#else
#endif
}

void wxToolBoxSampleIDEMainFrame::OnMenuItemUpdateUI(wxUpdateUIEvent & event)
{
	switch(event.GetId())
	{
	case ID_TOGGLE_PROJECT_TOOLBAR:
		GetMenuBar()->Check(ID_TOGGLE_PROJECT_TOOLBAR, GetPaneVisibility(_("Project")));
		break;
	case ID_TOGGLE_STANDARD_TOOLBAR:
		GetMenuBar()->Check(ID_TOGGLE_STANDARD_TOOLBAR, GetPaneVisibility(_("Standard")));
		break;
	case ID_TOGGLE_STATUSBAR:
		GetMenuBar()->Check(ID_TOGGLE_STATUSBAR, GetStatusBar() != NULL);
		break;
	}
}

bool wxToolBoxSampleIDEMainFrame::GetPaneVisibility(wxString pane_name)
{
	wxAuiPaneInfoArray& all_panes = m_FrameManager.GetAllPanes();
	size_t i, count;
	for (i = 0, count = all_panes.GetCount(); i < count; ++i)
	{		
		if(all_panes.Item(i).name == pane_name)
		{
			if(all_panes.Item(i).IsFloating())
			{
				return all_panes.Item(i).frame->IsShown();
			}
			else
			{
				return all_panes.Item(i).IsShown();
			}		
		}
	}
	return false;
}
