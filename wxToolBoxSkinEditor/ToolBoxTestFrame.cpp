#include "ToolBoxTestFrame.h"
#include <wx/treectrl.h>
#include <wx/valgen.h>

#include "AdditionalProperties.h"

#include "res/fileopen.xpm"
#include "res/filesave.xpm"

enum
{
	ID_PROPERTY_GRID_MANAGER = 10001,
	ID_ITEM_NAME_TEXT_CTRL,
	ID_ITEM_ICON_SPIN_CTRL,
	ID_ITEM_TAB_SPIN_CTRL,
	ID_ITEM_INDEX_SPIN_CTRL,
	ID_ITEM_ADD,
	ID_ITEM_ADD_AND_EDIT,
	ID_ITEM_RENAME,
	ID_ITEM_DELETE,

	ID_TAB_NAME_TEXT_CTRL,
	ID_TAB_ICON_SPIN_CTRL,
	ID_TAB_INDEX_SPIN_CTRL,
	ID_TAB_ADD,
	ID_TAB_ADD_AND_EDIT,
	ID_TAB_RENAME,
	ID_TAB_DELETE,

	ID_TAB_INDEX_1_SPIN_CTRL,
	ID_TAB_INDEX_2_SPIN_CTRL,
	ID_SWAP_TABS,

	ID_ITEM_INDEX_1_SPIN_CTRL,
	ID_ITEM_INDEX_2_SPIN_CTRL,
	ID_SWAP_ITEMS,

	ID_TOGGLE_SKIN,

	ID_LOG_TEXT_CTRL,

	ID_TOGGLE_PROPERTYGRID,
	ID_TOGGLE_TOOLBOX,
	ID_TOGGLE_LOG_WINDOW,
	ID_TOGGLE_STATUSBAR,
	
	ID_TOOLBOX,

	ID_TEST_FRAME_UNUSED
};

IMPLEMENT_DYNAMIC_CLASS(ToolBoxTestFrame, wxFrame)

BEGIN_EVENT_TABLE(ToolBoxTestFrame, wxFrame)

EVT_PG_CHANGED(ID_PROPERTY_GRID_MANAGER, ToolBoxTestFrame::OnPropertyChanged)

EVT_CHECKBOX(ID_TOGGLE_SKIN, ToolBoxTestFrame::OnToggleSkin)
EVT_BUTTON(ID_ITEM_ADD, ToolBoxTestFrame::OnAddItem)
EVT_BUTTON(ID_ITEM_ADD_AND_EDIT, ToolBoxTestFrame::OnAddAndEditItem)
EVT_BUTTON(ID_ITEM_DELETE, ToolBoxTestFrame::OnDeleteItem)
EVT_BUTTON(ID_SWAP_ITEMS, ToolBoxTestFrame::OnSwapItems)
EVT_BUTTON(ID_ITEM_RENAME, ToolBoxTestFrame::OnRenameItem)

EVT_BUTTON(ID_TAB_ADD, ToolBoxTestFrame::OnAddTab)
EVT_BUTTON(ID_TAB_ADD_AND_EDIT, ToolBoxTestFrame::OnAddAndEditTab)
EVT_BUTTON(ID_TAB_DELETE, ToolBoxTestFrame::OnDeleteTab)
EVT_BUTTON(ID_SWAP_TABS, ToolBoxTestFrame::OnSwapTabs)
EVT_BUTTON(ID_TAB_RENAME, ToolBoxTestFrame::OnRenameTab)

EVT_BUTTON(wxID_OPEN, ToolBoxTestFrame::OnLoadSkin)
EVT_BUTTON(wxID_SAVE, ToolBoxTestFrame::OnSaveSkin)

EVT_TB_TAB_ADDED(10001, ToolBoxTestFrame::OnToolBoxTabAdded)
EVT_TB_TAB_SELECTED(10001, ToolBoxTestFrame::OnToolBoxTabSelected)
EVT_TB_TAB_DELETED(10001, ToolBoxTestFrame::OnToolBoxTabDeleted)
EVT_TB_TABS_SWAPPED(10001, ToolBoxTestFrame::OnToolBoxTabsSwapped)

EVT_TB_ITEM_ADDED(10001, ToolBoxTestFrame::OnToolBoxItemAdded)
EVT_TB_ITEM_SELECTED(10001, ToolBoxTestFrame::OnToolBoxItemSelected)
EVT_TB_ITEM_DELETED(10001, ToolBoxTestFrame::OnToolBoxItemDeleted)
EVT_TB_ITEMS_SWAPPED(10001, ToolBoxTestFrame::OnToolBoxItemsSwapped)

EVT_TB_SKIN_LOADED(10001, ToolBoxTestFrame::OnToolBoxSkinLoaded)
EVT_TB_SKIN_SAVED(10001, ToolBoxTestFrame::OnToolBoxSkinSaved)

EVT_MENU(wxID_OPEN, ToolBoxTestFrame::OnLoadSkin)
EVT_MENU(wxID_SAVE, ToolBoxTestFrame::OnSaveSkin)
EVT_MENU(wxID_EXIT, ToolBoxTestFrame::OnExit)

EVT_MENU(ID_TOGGLE_PROPERTYGRID, ToolBoxTestFrame::OnToggleProperties)
EVT_MENU(ID_TOGGLE_TOOLBOX, ToolBoxTestFrame::OnToggleToolBox)
EVT_MENU(ID_TOGGLE_LOG_WINDOW, ToolBoxTestFrame::OnToggleLogWindow)
EVT_MENU(ID_TOGGLE_STATUSBAR, ToolBoxTestFrame::OnToggleStatusBar)

EVT_UPDATE_UI(ID_TOGGLE_PROPERTYGRID, ToolBoxTestFrame::OnTogglePropertiesUpdateUI)
EVT_UPDATE_UI(ID_TOGGLE_TOOLBOX, ToolBoxTestFrame::OnToggleToolBoxUpdateUI)
EVT_UPDATE_UI(ID_TOGGLE_LOG_WINDOW, ToolBoxTestFrame::OnToggleLogWindowUpdateUI)
EVT_UPDATE_UI(ID_TOGGLE_STATUSBAR, ToolBoxTestFrame::OnToggleStatusBarUpdateUI)

END_EVENT_TABLE()

ToolBoxTestFrame::ToolBoxTestFrame()
: wxFrame(NULL, wxID_ANY, wxT(""))
{
}

ToolBoxTestFrame::ToolBoxTestFrame(wxWindow * parent)
: 	wxFrame(parent, -1, _("wxToolBox SkinEditor"), wxPoint(100,100), wxSize(1000,750)),
m_PropGrid(NULL)
{	
	SetMinSize(GetSize());
	m_FrameManager.SetManagedWindow(this);
	wxPanel * toolbox_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	wxBoxSizer * toolbox_panel_sizer = new wxBoxSizer(wxVERTICAL);
	toolbox_panel->SetSizer(toolbox_panel_sizer);	
	CreateToolBox(toolbox_panel);
	m_Panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	tab_index_1_spin_ctrl = new wxSpinCtrl(m_Panel, ID_TAB_INDEX_1_SPIN_CTRL, _(""), wxDefaultPosition, wxSize(80, -1));
	tab_index_2_spin_ctrl = new wxSpinCtrl(m_Panel, ID_TAB_INDEX_2_SPIN_CTRL, _(""), wxDefaultPosition, wxSize(80, -1));
	item_index_1_spin_ctrl = new wxSpinCtrl(m_Panel, ID_ITEM_INDEX_1_SPIN_CTRL, _(""), wxDefaultPosition, wxSize(80, -1));
	item_index_2_spin_ctrl = new wxSpinCtrl(m_Panel, ID_ITEM_INDEX_2_SPIN_CTRL, _(""), wxDefaultPosition, wxSize(80, -1));

	m_LOGTextCtrl = new wxTextCtrl(this, ID_LOG_TEXT_CTRL, _("LOG Messages:\r\n"), wxDefaultPosition, wxSize(350, 100),
		wxTE_READONLY|wxTE_MULTILINE);	

	tab_name_text_ctrl = new wxTextCtrl(m_Panel, ID_TAB_NAME_TEXT_CTRL, _("Tab"));	
	tab_icon_spin_ctrl = new wxSpinCtrl(m_Panel, ID_TAB_ICON_SPIN_CTRL);	
	tab_index_spin_ctrl = new wxSpinCtrl(m_Panel, ID_TAB_INDEX_SPIN_CTRL);
	tab_rename_button = new wxButton(m_Panel, ID_TAB_RENAME, _("Rename"));
	tab_delete_button = new wxButton(m_Panel, ID_TAB_DELETE, _("Delete"));
	item_tab_spin_ctrl = new wxSpinCtrl(m_Panel, ID_ITEM_TAB_SPIN_CTRL, wxEmptyString);	

	wxBoxSizer * panelsizer = new wxBoxSizer(wxVERTICAL);
	m_Panel->SetSizer(panelsizer);	

	wxStaticBoxSizer * item_param_sizer = new wxStaticBoxSizer(wxVERTICAL, m_Panel, _("Items"));	
	wxGridBagSizer * item_param_gb_sizer = new wxGridBagSizer(5, 5);

	item_param_sizer->Add(item_param_gb_sizer, 1, wxEXPAND|wxALL, 5);
	
	wxStaticText * item_tab_label = new wxStaticText(m_Panel, wxID_ANY, _("Tab:"));			
	wxStaticText * item_icon_label = new wxStaticText(m_Panel, wxID_ANY, _("Icon:"));	
	wxStaticText * item_name_label = new wxStaticText(m_Panel, wxID_ANY, _("Name:"));	
	item_name_text_ctrl = new wxTextCtrl(m_Panel, ID_ITEM_NAME_TEXT_CTRL, _("Item"));			
	wxStaticText * item_index_label = new wxStaticText(m_Panel, wxID_ANY, _("Index:"));
	item_index_spin_ctrl = new wxSpinCtrl(m_Panel, ID_ITEM_INDEX_SPIN_CTRL);
	
	item_icon_spin_ctrl = new wxSpinCtrl(m_Panel, ID_ITEM_ICON_SPIN_CTRL);	

	item_add_button = new wxButton(m_Panel, ID_ITEM_ADD, _("Add"));
	item_add_and_edit_button = new wxButton(m_Panel, ID_ITEM_ADD_AND_EDIT, _("Add and Edit"));
	item_rename_button = new wxButton(m_Panel, ID_ITEM_RENAME, _("Rename"));
	item_delete_button = new wxButton(m_Panel, ID_ITEM_DELETE, _("Delete"));

	wxStaticText * tab_name_label = new wxStaticText(m_Panel, wxID_ANY, _("Name:"));		
	wxStaticText * tab_icon_label = new wxStaticText(m_Panel, wxID_ANY, _("Icon:"));		
	wxStaticText * tab_index_label = new wxStaticText(m_Panel, wxID_ANY, _("Index:"));
	wxButton * tab_add_button = new wxButton(m_Panel, ID_TAB_ADD, _("Add"));
	wxButton * tab_add_and_edit_button = new wxButton(m_Panel, ID_TAB_ADD_AND_EDIT, _("Add and Edit"));	
	
	wxButton * load_skin_button = new wxButton(m_Panel, wxID_OPEN, _("Load skin"));
	wxButton * save_skin_button = new wxButton(m_Panel, wxID_SAVE, _("Save skin"));

	wxButton * swap_tabs_button = new wxButton(m_Panel, ID_SWAP_TABS, _("Swap Tabs"));
	wxButton * swap_items_button = new wxButton(m_Panel, ID_SWAP_ITEMS, _("Swap Items"));

	toggle_skin_check_box = new wxCheckBox(m_Panel, ID_TOGGLE_SKIN, _("Toggle skin"));

	item_param_gb_sizer->Add(item_tab_label, wxGBPosition(0, 0), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_tab_spin_ctrl, wxGBPosition(0, 1), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_icon_label, wxGBPosition(1, 0), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_icon_spin_ctrl, wxGBPosition(1, 1), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_name_label, wxGBPosition(2, 0), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_name_text_ctrl, wxGBPosition(2, 1), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_index_label, wxGBPosition(3, 0), wxDefaultSpan, wxGROW);
	item_param_gb_sizer->Add(item_index_spin_ctrl, wxGBPosition(3, 1), wxDefaultSpan, wxGROW);	
	wxGridSizer * item_buttons_sizer = new wxGridSizer(2, 2, 5, 5);
	item_buttons_sizer->Add(item_add_button, 1, wxGROW);
	item_buttons_sizer->Add(item_add_and_edit_button, 1, wxGROW);
	item_buttons_sizer->Add(item_rename_button, 1, wxGROW);
	item_buttons_sizer->Add(item_delete_button, 1, wxGROW);
	item_param_gb_sizer->Add(item_buttons_sizer, wxGBPosition(4, 0), wxGBSpan(1,2), wxGROW);		
	item_param_gb_sizer->AddGrowableCol(1, 1);

	wxStaticBoxSizer * tab_param_sizer = new wxStaticBoxSizer(wxVERTICAL, m_Panel, _("Tabs"));
	wxGridBagSizer * tab_param_gb_sizer = new wxGridBagSizer(5, 5);

	tab_param_sizer->Add(tab_param_gb_sizer, 1, wxEXPAND|wxALL, 5);
	
	tab_param_gb_sizer->Add(tab_name_label, wxGBPosition(0, 0), wxDefaultSpan);
	tab_param_gb_sizer->Add(tab_name_text_ctrl, wxGBPosition(0, 1), wxDefaultSpan, wxGROW);
	tab_param_gb_sizer->Add(tab_icon_label, wxGBPosition(1, 0), wxDefaultSpan);
	tab_param_gb_sizer->Add(tab_icon_spin_ctrl, wxGBPosition(1, 1), wxDefaultSpan, wxGROW);
	tab_param_gb_sizer->Add(tab_index_label, wxGBPosition(2, 0), wxDefaultSpan);
	tab_param_gb_sizer->Add(tab_index_spin_ctrl, wxGBPosition(2, 1), wxDefaultSpan, wxGROW);
	wxGridSizer * tab_buttons_sizer = new wxGridSizer(2, 2, 5, 5);
	tab_buttons_sizer->Add(tab_add_button, 1, wxGROW);
	tab_buttons_sizer->Add(tab_add_and_edit_button, 1, wxGROW);
	tab_buttons_sizer->Add(tab_rename_button, 1, wxGROW);
	tab_buttons_sizer->Add(tab_delete_button, 1, wxGROW);
	tab_param_gb_sizer->Add(tab_buttons_sizer, wxGBPosition(3, 0), wxGBSpan(1,2), wxGROW);	
	tab_param_gb_sizer->AddGrowableCol(1, 1);

	wxStaticBoxSizer * toolbox_param_sizer = new wxStaticBoxSizer(wxHORIZONTAL, m_Panel, _("ToolBox"));
	wxGridSizer * toolbox_param_grid_sizer = new wxGridSizer(3, 3, 5, 5);
	
	toolbox_param_sizer->Add(toolbox_param_grid_sizer, 1, wxEXPAND|wxALL, 5);
	
	toolbox_param_grid_sizer->Add(load_skin_button, 1, wxGROW);
	toolbox_param_grid_sizer->Add(save_skin_button, 1, wxGROW);	
	toolbox_param_grid_sizer->Add(toggle_skin_check_box, 1, wxGROW);

	toolbox_param_grid_sizer->Add(tab_index_1_spin_ctrl, 1, wxGROW);
	toolbox_param_grid_sizer->Add(tab_index_2_spin_ctrl, 1, wxGROW);
	toolbox_param_grid_sizer->Add(swap_tabs_button, 1, wxGROW);

	toolbox_param_grid_sizer->Add(item_index_1_spin_ctrl, 1, wxGROW);
	toolbox_param_grid_sizer->Add(item_index_2_spin_ctrl, 1, wxGROW);
	toolbox_param_grid_sizer->Add(swap_items_button, 1, wxGROW);

	panelsizer->Add(tab_param_sizer, 0, wxGROW|wxALL, 5);
	panelsizer->Add(item_param_sizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);		
	panelsizer->Add(toolbox_param_sizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);	

	panelsizer->Fit(m_Panel);
	
	//-------------------------
	item_tab_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	item_tab_spin_ctrl->SetValue(_("0"));	
	item_icon_spin_ctrl->SetRange(-1, (int)m_ToolBox->GetItemImageList().Count()-1);
	item_icon_spin_ctrl->SetValue(_("0"));
	if(m_ToolBox->GetSelectedTab())
		item_index_spin_ctrl->SetRange(0, (int)m_ToolBox->GetSelectedTab()->GetItemCount()-1);
	item_index_spin_ctrl->SetValue(_("0"));
	tab_icon_spin_ctrl->SetRange(-1, (int)m_ToolBox->GetTabImageList().Count()-1);
	tab_icon_spin_ctrl->SetValue(_("0"));	
	tab_index_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab_index_spin_ctrl->SetValue(_("0"));
	tab_index_1_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab_index_2_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab_index_1_spin_ctrl->SetValue(0);
	tab_index_2_spin_ctrl->SetValue(0);	
	toggle_skin_check_box->SetValue(true);

	wxToolBoxTab * tab = m_ToolBox->GetSelectedTab();
	if(tab)
	{
		item_index_1_spin_ctrl->SetRange(0, (int)tab->GetItemCount()-1);
		item_index_2_spin_ctrl->SetRange(0, (int)tab->GetItemCount()-1);
	}
	item_index_1_spin_ctrl->SetValue(0);
	item_index_2_spin_ctrl->SetValue(0);

	m_PropGrid = new wxPropertyGrid(this, ID_PROPERTY_GRID_MANAGER,
        wxDefaultPosition, wxSize(330, 400),
        wxPG_BOLD_MODIFIED|
        wxPG_SPLITTER_AUTO_CENTER|
        wxPG_AUTO_SORT|        
        wxTAB_TRAVERSAL |
        wxPG_TOOLBAR
       );
	m_PropGrid->SetMinSize(wxSize(330, 400));
	m_PropGrid->GetGrid()->SetVerticalSpacing(2);		
    	
	m_FrameManager.AddPane(m_PropGrid, wxAuiPaneInfo().
                  Name(wxT("Properties")).Caption(wxT("Properties")).Left().
				  Layer(1).Position(1));
	m_FrameManager.AddPane(m_LOGTextCtrl, wxAuiPaneInfo().
                  Name(wxT("Event LOG")).Caption(wxT("Event LOG")).Bottom().
				  Layer(1).Position(1));
	m_FrameManager.AddPane(toolbox_panel, wxAuiPaneInfo().MinSize(wxSize(160,300)).
                  TopDockable(false).BottomDockable(false).Name(wxT("ToolBox")).
				  Caption(wxT("ToolBox")).Right().Layer(1).Position(1));
	m_FrameManager.AddPane(m_Panel, wxAuiPaneInfo().Name(wxT("Center Pane")).CenterPane());	
	m_FrameManager.Update();

	m_ToolBox->RefreshTabs();
		
	CreateProperties();
	CreateMenuBar();
	CreateStatusBar();

	Centre();
}

ToolBoxTestFrame::~ToolBoxTestFrame()
{
	m_FrameManager.UnInit();
}

void ToolBoxTestFrame::CreateToolBox(wxWindow * parent)
{
	wxBoxSizer * parent_sizer = (wxBoxSizer *)parent->GetSizer();
	m_ToolBox = new wxToolBox(parent, 10001, wxDefaultPosition, wxSize(200,300), wxNO_BORDER);
	m_ToolBox->SetMinSize(wxSize(160,300));	
	m_ToolBox->SetBackground(wxT("./res/Background.png"));
	m_ToolBox->SetItemsTransparent(true);
	m_ToolBox->SetTabHeight(26);	
	m_ToolBox->SetTabTextColour(*wxWHITE);
	m_ToolBox->SetItemTextColour(*wxWHITE);
	wxFont tabfont = m_ToolBox->GetFont();
	tabfont.SetWeight(wxFONTWEIGHT_BOLD);
	m_ToolBox->SetTabHoverFont(tabfont);
	
	m_ToolBox->CreateTabImageList(wxT("./res/TabIcons.png"), 16, 16, false);
	m_ToolBox->CreateItemImageList(wxT("./res/ItemIcons.png"), 16, 16, false);

	wxToolBoxBitmapSet normal(wxT("./res/TAB_Skin_Left.PNG"),
							  wxT("./res/TAB_Skin_Middle.PNG"),
							  wxT("./res/TAB_Skin_Right.PNG"));
	wxToolBoxBitmapSet hover(wxT("./res/TAB_Skin_Left_Hov.PNG"),
							  wxT("./res/TAB_Skin_Middle_Hov.PNG"),
							  wxT("./res/TAB_Skin_Right_Hov.PNG"));
	wxToolBoxBitmapSet selected(wxT("./res/TAB_Skin_Left_Sel.PNG"),
							  wxT("./res/TAB_Skin_Middle_Sel.PNG"),
							  wxT("./res/TAB_Skin_Right_Sel.PNG"));
	m_ToolBox->SetTabBackground(normal, hover, selected);
	m_ToolBox->SetUseTabBackground(true);
	m_ToolBox->SetButtonBackground(normal, hover, selected);
	m_ToolBox->SetUseButtonBackground(true);

	m_ToolBox->SetBackgroundColour(wxColour(35, 42, 68));
	m_ToolBox->SetItemNormalColour(wxColour(35, 42, 68));
	m_ToolBox->SetItemHoverColour(wxColour(79, 95, 121));
	m_ToolBox->SetItemSelectedColour(wxColour(146, 151, 164));
	m_ToolBox->SetTabNormalColour(wxColour(170, 170, 170));
	m_ToolBox->SetTabSelectedColour(wxColour(236, 233, 216));
	m_ToolBox->SetButtonNormalColour(wxColour(170, 170, 170));

	time_t t;
	time(&t);
	srand(t);

	wxToolBoxTab * tab1 = new wxToolBoxTab(wxT("Tab 1"), 1);
	if(m_ToolBox->GetTabImageList().Count()) tab1->SetImageIndex(rand()%(int)m_ToolBox->GetTabImageList().Count());
	tab1->SetToolTip(_("First tab"));
	wxToolBoxTab * tab2 = new wxToolBoxTab(wxT("Tab 2"), 2);
	tab2->SetToolTip(_("Second tab without icon"));
	wxToolBoxTab * tab3 = new wxToolBoxTab(wxT("Tab 3"), 3);
	if(m_ToolBox->GetTabImageList().Count()) tab3->SetImageIndex(rand()%(int)m_ToolBox->GetTabImageList().Count());
	wxToolBoxTab * tab4 = new wxToolBoxTab(wxT("Tab 4"), 4);	
	if(m_ToolBox->GetTabImageList().Count()) tab4->SetImageIndex(rand()%(int)m_ToolBox->GetTabImageList().Count());
	m_ToolBox->AddTab(tab1);
	m_ToolBox->AddTab(tab2);
	m_ToolBox->AddTab(tab3);

	wxToolBoxTab * tab5 = new wxToolBoxTab(wxT("wxTreeCtrl inside tab"));
	wxTreeCtrl * treeCtrl = new wxTreeCtrl(m_ToolBox, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
	wxTreeItemId root = treeCtrl->AddRoot(_("Root"));
	treeCtrl->AppendItem(root, _("Item 1"));
	treeCtrl->AppendItem(root, _("Item 2"));
	treeCtrl->AppendItem(root, _("Item 3"));
	treeCtrl->Expand(root);
	tab5->SetChildWindow(treeCtrl);
	m_ToolBox->AddTab(tab5);

	m_ToolBox->AddTab(tab4);	
	wxToolBoxItem item1(_("Item1"), 0);	

	for(int i = 0; i < 120; i++)
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
		else if(i >= 60 && i < 90)
		{
			tab3->AddItem(item1);
		}
		else 
		{			
			if(i == 107)
			{
				int z = 1;
			}
			tab4->AddItem(item1);
		}
	}	
	tab3->SetSelected(true);
	if(parent_sizer) parent_sizer->Add(m_ToolBox, 1, wxEXPAND);
}

void ToolBoxTestFrame::CreateMenuBar()
{
	wxMenuBar * menuBar = GetMenuBar();
	if(menuBar)
	{
		SetMenuBar(NULL);
		menuBar->Destroy();
	}
	menuBar = new wxMenuBar;
	SetMenuBar(menuBar);

	wxMenu * fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, _("Load skin\tCtrl+O"));
	fileMenu->Append(wxID_SAVE, _("Save skin\tCtrl+S"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("Exit\tAlt+F4"));

	wxMenu * viewMenu = new wxMenu;
	viewMenu->AppendCheckItem(ID_TOGGLE_PROPERTYGRID, _("Properties"));
	viewMenu->AppendCheckItem(ID_TOGGLE_TOOLBOX, _("ToolBox"));
	viewMenu->AppendCheckItem(ID_TOGGLE_LOG_WINDOW, _("Log Window"));
	viewMenu->AppendCheckItem(ID_TOGGLE_STATUSBAR, _("Status Bar"));	

	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(viewMenu, _("View"));
}

void ToolBoxTestFrame::CreateProperties()
{
	static const wxChar* styles[] = {wxT("List"),
                                      wxT("Thumbnails"),                                      
                                      (const wxChar*) NULL};
	m_PropGrid->Append(new wxPropertyCategory(wxT("Appearance"),wxPG_LABEL));
	m_PropGrid->Append(new wxEnumProperty(wxT("Style"), wxPG_LABEL, styles));
	m_PropGrid->Append(new wxBoolProperty(wxT("ItemsTransparent"), wxPG_LABEL, m_ToolBox->GetItemsTransparent()));
	m_PropGrid->Append(new wxBoolProperty(wxT("SelectedItemTransparent"), wxPG_LABEL, m_ToolBox->GetSelectedItemTransparent()));
	m_PropGrid->Append(new wxBoolProperty(wxT("UseBackground"), wxPG_LABEL, m_ToolBox->GetUseBackground()));
	m_PropGrid->Append(new wxBoolProperty(wxT("UseStdArrows"), wxPG_LABEL, m_ToolBox->GetUseStdArrows()));
	m_PropGrid->Append(new wxBoolProperty(wxT("PreDrawBackgrounds"), wxPG_LABEL, m_ToolBox->GetPreDrawBackgrounds()));
	m_PropGrid->Append(new wxBoolProperty(wxT("SelectNewTab"), wxPG_LABEL, m_ToolBox->GetSelectNewTab()));
	m_PropGrid->Append(new wxBoolProperty(wxT("ShowSelectionAlways"), wxPG_LABEL, m_ToolBox->GetShowSelectionAlways()));
	m_PropGrid->Append(new wxColourProperty(wxT("BackgroundColour"), wxPG_LABEL,
		m_ToolBox->GetBackgroundColour()));
	m_PropGrid->Append(new wxColourProperty(wxT("BorderHighlightColour"), wxPG_LABEL,
		m_ToolBox->GetBorderHighlightColour()));
	m_PropGrid->Append(new wxColourProperty(wxT("BorderShadowColour"), wxPG_LABEL,
		m_ToolBox->GetBorderShadowColour()));
	
	m_PropGrid->Append(new wxImageFileProperty(wxT("Background")));
	
	m_PropGrid->Append(new wxPropertyCategory(wxT("Items"), wxPG_LABEL));

	//m_PropGrid->Append(wxToolObjectBackgroundInfoProperty(wxT("Item Background"),wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("ItemNormalFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("ItemHoverFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("ItemSelectedFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxColourProperty(wxT("ItemTextColour"), wxPG_LABEL,
		m_ToolBox->GetItemTextColour()));

	m_PropGrid->Append(new wxIntProperty(wxT("ItemHeight"), wxPG_LABEL, m_ToolBox->GetItemHeight()));
	m_PropGrid->Append(new wxIntProperty(wxT("ItemSpacing"), wxPG_LABEL, m_ToolBox->GetItemSpacing()));
	m_PropGrid->Append(new wxIntProperty(wxT("ItemCaptionSpacing"), wxPG_LABEL, m_ToolBox->GetItemCaptionSpacing()));

	m_PropGrid->Append(new wxColourProperty(wxT("ItemNormalColour"), wxPG_LABEL,
		m_ToolBox->GetItemNormalColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("ItemHoverColour"), wxPG_LABEL,
		m_ToolBox->GetItemHoverColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("ItemSelectedColour"), wxPG_LABEL,
		m_ToolBox->GetItemSelectedColour()));	

	m_PropGrid->Append(new wxBoolProperty(wxT("UseItemNormalBackground"), wxPG_LABEL, m_ToolBox->GetUseItemNormalBackground()));
	m_PropGrid->Append(new wxBoolProperty(wxT("UseItemHoverBackground"), wxPG_LABEL, m_ToolBox->GetUseItemHoverBackground()));
	m_PropGrid->Append(new wxBoolProperty(wxT("UseItemSelectedBackground"), wxPG_LABEL, m_ToolBox->GetUseItemSelectedBackground()));

	m_PropGrid->Append(new wxPropertyCategory(wxT("Tabs"), wxPG_LABEL));

	//m_PropGrid->Append(wxToolObjectBackgroundInfoProperty(wxT("Tab Background"),wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("TabNormalFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("TabHoverFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxFontProperty(wxT("TabSelectedFont"), wxPG_LABEL));
	m_PropGrid->Append(new wxColourProperty(wxT("TabTextColour"), wxPG_LABEL,
		m_ToolBox->GetTabTextColour()));

	m_PropGrid->Append(new wxIntProperty(wxT("TabHeight"), wxPG_LABEL, m_ToolBox->GetTabHeight()));
	m_PropGrid->Append(new wxIntProperty(wxT("TabSpacing"), wxPG_LABEL, m_ToolBox->GetTabSpacing()));
	m_PropGrid->Append(new wxIntProperty(wxT("TabCaptionSpacing"), wxPG_LABEL, m_ToolBox->GetTabCaptionSpacing()));

	m_PropGrid->Append(new wxColourProperty(wxT("TabNormalColour"), wxPG_LABEL,
		m_ToolBox->GetTabNormalColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("TabHoverColour"), wxPG_LABEL,
		m_ToolBox->GetTabHoverColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("TabSelectedColour"), wxPG_LABEL,
		m_ToolBox->GetTabSelectedColour()));	

	m_PropGrid->Append(new wxBoolProperty(wxT("UseTabBackground"), wxPG_LABEL, m_ToolBox->GetUseTabBackground()));

	m_PropGrid->Append(new wxPropertyCategory(wxT("Buttons"), wxPG_LABEL));

	//m_PropGrid->Append(wxToolObjectBackgroundInfoProperty(wxT("Button Background"),wxPG_LABEL));
	m_PropGrid->Append(new wxColourProperty(wxT("ButtonNormalColour"), wxPG_LABEL,
		m_ToolBox->GetButtonNormalColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("ButtonHoverColour"), wxPG_LABEL,
		m_ToolBox->GetButtonHoverColour()));	
	m_PropGrid->Append(new wxColourProperty(wxT("ButtonSelectedColour"), wxPG_LABEL,
		m_ToolBox->GetButtonSelectedColour()));	

	m_PropGrid->Append(new wxBoolProperty(wxT("UseButtonBackground"), wxPG_LABEL, m_ToolBox->GetUseButtonBackground()));
}

void ToolBoxTestFrame::SynchronizeProperties()
{
	wxToolObjectBackgroundInfo bg;
	//wxToolObjectBackgroundInfoPropertyClass * prop(NULL);

	m_PropGrid->SetPropertyValue(wxT("Style"), (int)m_ToolBox->GetStyle());
	m_PropGrid->SetPropertyValue(wxT("Background"), m_ToolBox->GetBackgroundPath());
	m_PropGrid->SetPropertyValue(wxT("UseBackground"), m_ToolBox->GetUseBackground());
	m_PropGrid->SetPropertyValue(wxT("UseTabBackground"), m_ToolBox->GetUseTabBackground());
	m_PropGrid->SetPropertyValue(wxT("UseItemNormalBackground"), m_ToolBox->GetUseItemNormalBackground());
	m_PropGrid->SetPropertyValue(wxT("UseItemHoverBackground"), m_ToolBox->GetUseItemHoverBackground());
	m_PropGrid->SetPropertyValue(wxT("UseItemSelectedBackground"), m_ToolBox->GetUseItemSelectedBackground());
	m_PropGrid->SetPropertyValue(wxT("UseButtonBackground"), m_ToolBox->GetUseButtonBackground());
	m_PropGrid->SetPropertyValue(wxT("UseStdArrows"), m_ToolBox->GetUseStdArrows());
	m_PropGrid->SetPropertyValue(wxT("SelectNewTab"), m_ToolBox->GetSelectNewTab());
	m_PropGrid->SetPropertyValue(wxT("ShowSelectionAlways"), m_ToolBox->GetShowSelectionAlways());
	m_PropGrid->SetPropertyValue(wxT("PreDrawBackgrounds"), m_ToolBox->GetPreDrawBackgrounds());
	m_PropGrid->SetPropertyValue(wxT("ItemsTransparent"), m_ToolBox->GetItemsTransparent());
	m_PropGrid->SetPropertyValue(wxT("SelectedItemTransparent"), m_ToolBox->GetSelectedItemTransparent());
	m_PropGrid->SetPropertyValue(wxT("BackgroundColour"), m_ToolBox->GetBackgroundColour());
	m_PropGrid->SetPropertyValue(wxT("BorderHighlightColour"), m_ToolBox->GetBorderHighlightColour());
	m_PropGrid->SetPropertyValue(wxT("BorderShadowColour"), m_ToolBox->GetBorderShadowColour());
	
	bg.SetFileNames(		
			m_ToolBox->GetTabBackground().GetNormalBackground().GetLeftPath(),
			m_ToolBox->GetTabBackground().GetNormalBackground().GetMiddlePath(),
			m_ToolBox->GetTabBackground().GetNormalBackground().GetRightPath(),						
			m_ToolBox->GetTabBackground().GetHoverBackground().GetLeftPath(),
			m_ToolBox->GetTabBackground().GetHoverBackground().GetMiddlePath(),
			m_ToolBox->GetTabBackground().GetHoverBackground().GetRightPath(),					
			m_ToolBox->GetTabBackground().GetSelectedBackground().GetLeftPath(),
			m_ToolBox->GetTabBackground().GetSelectedBackground().GetMiddlePath(),
			m_ToolBox->GetTabBackground().GetSelectedBackground().GetRightPath()
					);		
	/*id = m_PropGrid->GetPropertyByLabel(wxT("Tab Background"));
	prop = (wxToolObjectBackgroundInfoPropertyClass *)id.GetPropertyPtr();
	if(prop) prop->DoSetValue((void*)&bg);*/
	
	m_PropGrid->SetPropertyValue(wxT("TabNormalColour"), wxColour(m_ToolBox->GetTabNormalColour()));
	m_PropGrid->SetPropertyValue(wxT("TabHoverColour"), wxColour(m_ToolBox->GetTabHoverColour()));
	m_PropGrid->SetPropertyValue(wxT("TabSelectedColour"), wxColour(m_ToolBox->GetTabSelectedColour()));
	m_PropGrid->SetPropertyValue(wxT("TabTextColour"), wxColour(m_ToolBox->GetTabTextColour()));
	m_PropGrid->SetPropertyValue(wxT("TabNormalFont"), wxFont(m_ToolBox->GetTabNormalFont()));
	m_PropGrid->SetPropertyValue(wxT("TabHoverFont"), wxFont(m_ToolBox->GetTabHoverFont()));
	m_PropGrid->SetPropertyValue(wxT("TabSelectedFont"), wxFont(m_ToolBox->GetTabSelectedFont()));
	
	bg.SetFileNames(		
			m_ToolBox->GetItemBackground().GetNormalBackground().GetLeftPath(),
			m_ToolBox->GetItemBackground().GetNormalBackground().GetMiddlePath(),
			m_ToolBox->GetItemBackground().GetNormalBackground().GetRightPath(),						
			m_ToolBox->GetItemBackground().GetHoverBackground().GetLeftPath(),
			m_ToolBox->GetItemBackground().GetHoverBackground().GetMiddlePath(),
			m_ToolBox->GetItemBackground().GetHoverBackground().GetRightPath(),					
			m_ToolBox->GetItemBackground().GetSelectedBackground().GetLeftPath(),
			m_ToolBox->GetItemBackground().GetSelectedBackground().GetMiddlePath(),
			m_ToolBox->GetItemBackground().GetSelectedBackground().GetRightPath()
					);		
	/*id = m_PropGrid->GetPropertyByLabel(wxT("Item Background"));
	/*prop = (wxToolObjectBackgroundInfoPropertyClass *)id.GetPropertyPtr();
	if(prop) prop->DoSetValue((void*)&bg);*/
	m_PropGrid->SetPropertyValue(wxT("ItemNormalColour"), wxColour(m_ToolBox->GetItemNormalColour()));
	m_PropGrid->SetPropertyValue(wxT("ItemHoverColour"), wxColour(m_ToolBox->GetItemHoverColour()));
	m_PropGrid->SetPropertyValue(wxT("ItemSelectedColour"), wxColour(m_ToolBox->GetItemSelectedColour()));
	m_PropGrid->SetPropertyValue(wxT("ItemTextColour"), wxColour(m_ToolBox->GetItemTextColour()));
	m_PropGrid->SetPropertyValue(wxT("ItemNormalFont"), wxFont(m_ToolBox->GetItemNormalFont()));
	m_PropGrid->SetPropertyValue(wxT("ItemHoverFont"), wxFont(m_ToolBox->GetItemHoverFont()));
	m_PropGrid->SetPropertyValue(wxT("ItemSelectedFont"), wxFont(m_ToolBox->GetItemSelectedFont()));
	
	bg.SetFileNames(		
			m_ToolBox->GetButtonBackground().GetNormalBackground().GetLeftPath(),
			m_ToolBox->GetButtonBackground().GetNormalBackground().GetMiddlePath(),
			m_ToolBox->GetButtonBackground().GetNormalBackground().GetRightPath(),						
			m_ToolBox->GetButtonBackground().GetHoverBackground().GetLeftPath(),
			m_ToolBox->GetButtonBackground().GetHoverBackground().GetMiddlePath(),
			m_ToolBox->GetButtonBackground().GetHoverBackground().GetRightPath(),					
			m_ToolBox->GetButtonBackground().GetSelectedBackground().GetLeftPath(),
			m_ToolBox->GetButtonBackground().GetSelectedBackground().GetMiddlePath(),
			m_ToolBox->GetButtonBackground().GetSelectedBackground().GetRightPath()
					);		
	/*id = m_PropGrid->GetPropertyByLabel(wxT("Button Background"));
	prop = (wxToolObjectBackgroundInfoPropertyClass *)id.GetPropertyPtr();
	if(prop) prop->DoSetValue((void*)&bg);*/
	m_PropGrid->SetPropertyValue(wxT("ButtonNormalColour"), wxColour(m_ToolBox->GetButtonNormalColour()));
	m_PropGrid->SetPropertyValue(wxT("ButtonHoverColour"), wxColour(m_ToolBox->GetButtonHoverColour()));
	m_PropGrid->SetPropertyValue(wxT("ButtonSelectedColour"), wxColour(m_ToolBox->GetButtonSelectedColour()));

	m_PropGrid->SetPropertyValue(wxT("TabHeight"), (int)m_ToolBox->GetTabHeight());
	m_PropGrid->SetPropertyValue(wxT("TabSpacing"), (int)m_ToolBox->GetTabSpacing());
	m_PropGrid->SetPropertyValue(wxT("TabCaptionSpacing"), (int)m_ToolBox->GetTabCaptionSpacing());
	m_PropGrid->SetPropertyValue(wxT("ItemHeight"), (int)m_ToolBox->GetItemHeight());
	m_PropGrid->SetPropertyValue(wxT("ItemSpacing"), (int)m_ToolBox->GetItemSpacing());
	m_PropGrid->SetPropertyValue(wxT("ItemCaptionSpacing"), (int)m_ToolBox->GetItemCaptionSpacing());
	toggle_skin_check_box->SetValue(m_ToolBox->GetUseBackground() ||
		m_ToolBox->GetUseTabBackground() ||
		m_ToolBox->GetUseButtonBackground() ||
		m_ToolBox->GetUseItemNormalBackground()||
		m_ToolBox->GetUseItemHoverBackground() ||
		m_ToolBox->GetUseItemSelectedBackground());
}

void ToolBoxTestFrame::OnPropertyChanged(wxPropertyGridEvent & event)
{
	const wxString& name = event.GetPropertyName();
	wxVariant value = event.GetPropertyValue();
	if(name == wxT("Style"))
	{
		m_ToolBox->SetStyle(event.GetPropertyValue().GetInteger());
	}
	if(name == wxT("Background"))
	{
		m_ToolBox->SetBackground(event.GetPropertyValue().GetString());
	}
	if(name == wxT("UseBackground"))
	{
		m_ToolBox->SetUseBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseTabBackground"))
	{
		m_ToolBox->SetUseTabBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseItemNormalBackground"))
	{
		m_ToolBox->SetUseItemNormalBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseItemHoverBackground"))
	{
		m_ToolBox->SetUseItemHoverBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseItemSelectedBackground"))
	{
		m_ToolBox->SetUseItemSelectedBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseButtonBackground"))
	{
		m_ToolBox->SetUseButtonBackground(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("UseStdArrows"))
	{
		m_ToolBox->SetUseStdArrows(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("SelectNewTab"))
	{
		m_ToolBox->SetSelectNewTab(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("ShowSelectionAlways"))
	{
		m_ToolBox->SetShowSelectionAlways(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("PreDrawBackgrounds"))
	{
		m_ToolBox->SetPreDrawBackgrounds(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("ItemsTransparent"))
	{
		m_ToolBox->SetItemsTransparent(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("SelectedItemTransparent"))
	{
		m_ToolBox->SetSelectedItemTransparent(!!event.GetPropertyValue().GetBool());
	}
	if(name == wxT("BackgroundColour"))
	{		
		m_ToolBox->SetBackgroundColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
		m_ToolBox->Refresh();
	}
	if(name == wxT("BorderHighlightColour"))
	{		
		m_ToolBox->SetBorderHighlightColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
		m_ToolBox->Refresh();
	}
	if(name == wxT("BorderShadowColour"))
	{		
		m_ToolBox->SetBorderShadowColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
		m_ToolBox->Refresh();
	}

	if(name == wxT("TabNormalColour"))
	{		
		m_ToolBox->SetTabNormalColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabHoverColour"))
	{		
		m_ToolBox->SetTabHoverColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabSelectedColour"))
	{		
		m_ToolBox->SetTabSelectedColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabNormalFont"))
	{
		m_ToolBox->SetTabNormalFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabHoverFont"))
	{
		m_ToolBox->SetTabHoverFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabSelectedFont"))
	{
		m_ToolBox->SetTabSelectedFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabTextColour"))
	{		
		m_ToolBox->SetTabTextColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("TabHeight"))
	{
		m_PropGrid->SetPropertyValue(name, abs(event.GetPropertyValue().GetInteger()));
		m_ToolBox->SetTabHeight(abs(event.GetPropertyValue().GetInteger()));
	}
	if(name == wxT("TabSpacing"))
	{
		m_PropGrid->SetPropertyValue(name, abs(event.GetPropertyValue().GetInteger()));
		m_ToolBox->SetTabSpacing(abs(event.GetPropertyValue().GetInteger()));
	}

	if(name == wxT("ItemNormalColour"))
	{
		m_ToolBox->SetItemNormalColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemHoverColour"))
	{		
		m_ToolBox->SetItemHoverColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemSelectedColour"))
	{		
		m_ToolBox->SetItemSelectedColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemNormalFont"))
	{
		m_ToolBox->SetItemNormalFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemHoverFont"))
	{
		m_ToolBox->SetItemHoverFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemSelectedFont"))
	{
		m_ToolBox->SetItemSelectedFont(*((wxFont*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemTextColour"))
	{		
		m_ToolBox->SetItemTextColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ItemHeight"))
	{
		m_PropGrid->SetPropertyValue(name, abs(event.GetPropertyValue().GetInteger()));
		m_ToolBox->SetItemHeight(abs(event.GetPropertyValue().GetInteger()));
	}
	if(name == wxT("ItemSpacing"))
	{
		m_PropGrid->SetPropertyValue(name, abs(event.GetPropertyValue().GetInteger()));
		m_ToolBox->SetItemSpacing(abs(event.GetPropertyValue().GetInteger()));
	}

	if(name == wxT("ButtonNormalColour"))
	{		
		m_ToolBox->SetButtonNormalColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ButtonHoverColour"))
	{		
		m_ToolBox->SetButtonHoverColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	if(name == wxT("ButtonSelectedColour"))
	{		
		m_ToolBox->SetButtonSelectedColour(*((wxColour*)event.GetPropertyValue().GetWxObjectPtr()));
	}
	
}

void ToolBoxTestFrame::OnToggleSkin(wxCommandEvent & event)
{
	bool bUseSkin = toggle_skin_check_box->GetValue();
	m_ToolBox->UseSkin(bUseSkin);		
	m_PropGrid->SetPropertyValue(wxT("UseBackground"), bUseSkin);	
	m_PropGrid->SetPropertyValue(wxT("UseItemNormalBackground"), bUseSkin);
	m_PropGrid->SetPropertyValue(wxT("UseItemHoverBackground"), bUseSkin);
	m_PropGrid->SetPropertyValue(wxT("UseItemSelectedBackground"), bUseSkin);
	m_PropGrid->SetPropertyValue(wxT("UseButtonBackground"), bUseSkin);
}

void ToolBoxTestFrame::OnAddItem(wxCommandEvent & event)
{		
	wxToolBoxTab * tab = m_ToolBox->GetTab(item_tab_spin_ctrl->GetValue());	
	if(!tab) return;	
	wxToolBoxItem item(item_name_text_ctrl->GetValue(), rand(), item_icon_spin_ctrl->GetValue());	
	tab->AddItem(item);
}

void ToolBoxTestFrame::OnAddAndEditItem(wxCommandEvent & event)
{		
	wxToolBoxTab * tab = m_ToolBox->GetTab(item_tab_spin_ctrl->GetValue());	
	if(!tab) return;	
	wxToolBoxItem item(item_name_text_ctrl->GetValue(), rand(), item_icon_spin_ctrl->GetValue());	
	int index = tab->AddItem(item);
	m_ToolBox->RenameItem((*tab)[index]);
}

void ToolBoxTestFrame::OnSaveSkin(wxCommandEvent & event)
{
	wxFileDialog * save_dialog = new wxFileDialog(this);
	save_dialog->SetWildcard(_("wxToolBox XML Skin-files (*.xml)|*.xml"));
	if(save_dialog->ShowModal() == wxID_OK)
	{
		m_ToolBox->SaveSkin(save_dialog->GetPath());
	}
	save_dialog->Destroy();
}

void ToolBoxTestFrame::OnLoadSkin(wxCommandEvent & event)
{
	wxFileDialog * save_dialog = new wxFileDialog(this);
	save_dialog->SetWildcard(_("wxToolBox XML Skin-files (*.xml)|*.xml"));
	if(save_dialog->ShowModal() == wxID_OK)
	{
		m_ToolBox->LoadSkin(save_dialog->GetPath());		
		m_ToolBox->RefreshTabs();
	}
	wxToolBoxTab * tab(NULL);	
	for(wxToolBoxTabList::Node * node = m_ToolBox->GetTabs().GetFirst(); node; node = node->GetNext())
	{
		tab = node->GetData();
		if(!tab) continue;
		if(m_ToolBox->GetItemImageList().Count())
		{
			for(unsigned int i = 0; i < tab->GetItemCount(); i++)
			{
				tab->GetToolItems()[i].SetImageIndex((int)(rand()%m_ToolBox->GetItemImageList().Count()));
			}
		}
	}
	save_dialog->Destroy();
}

void ToolBoxTestFrame::OnSwapTabs(wxCommandEvent & event)
{		
	m_ToolBox->SwapTabs(tab_index_1_spin_ctrl->GetValue(), 
		tab_index_2_spin_ctrl->GetValue());
}

void ToolBoxTestFrame::OnSwapItems(wxCommandEvent & event)
{		
	wxToolBoxTab * tab = m_ToolBox->GetSelectedTab();
	if(!tab) return;
	tab->SwapItems(item_index_1_spin_ctrl->GetValue(), 
		item_index_2_spin_ctrl->GetValue());
}

void ToolBoxTestFrame::OnDeleteItem(wxCommandEvent & event)
{
	wxToolBoxTab * tab = m_ToolBox->GetTab(item_tab_spin_ctrl->GetValue());
	if(tab)
	{
		tab->DeleteItem(item_index_spin_ctrl->GetValue());
	}
}

void ToolBoxTestFrame::OnDeleteTab(wxCommandEvent & event)
{
	m_ToolBox->DeleteTab(tab_index_spin_ctrl->GetValue());
}

void ToolBoxTestFrame::OnAddTab(wxCommandEvent & event)
{
	wxToolBoxTab * tab = new wxToolBoxTab(tab_name_text_ctrl->GetValue(), rand(), tab_icon_spin_ctrl->GetValue());
	m_ToolBox->AddTab(tab);
	item_tab_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab_index_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	//tab->SetSelected(true);
}

void ToolBoxTestFrame::OnAddAndEditTab(wxCommandEvent & event)
{
	wxToolBoxTab * tab = new wxToolBoxTab(tab_name_text_ctrl->GetValue(), rand(), tab_icon_spin_ctrl->GetValue());
	m_ToolBox->AddTab(tab);
	item_tab_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab_index_spin_ctrl->SetRange(0, (int)m_ToolBox->GetTabs().GetCount()-1);
	tab->SetSelected(true);
	m_ToolBox->RenameItem(tab);
}

void ToolBoxTestFrame::OnRenameItem(wxCommandEvent & event)
{
	wxToolBoxTab * tab = m_ToolBox->GetTab(item_tab_spin_ctrl->GetValue());
	if(!tab) return;
	wxToolBoxItem * item = (*tab)[item_index_spin_ctrl->GetValue()];
	if(!item) return;
	m_ToolBox->RenameItem(item);
}

void ToolBoxTestFrame::OnRenameTab(wxCommandEvent & event)
{
	wxToolBoxTab * tab = m_ToolBox->GetTab(tab_index_spin_ctrl->GetValue());
	if(!tab) return;	
	m_ToolBox->RenameItem(tab);
}

void ToolBoxTestFrame::OnExit(wxCommandEvent & event)
{
	Close();
}

void ToolBoxTestFrame::OnToggleProperties(wxCommandEvent & event)
{
	TogglePaneVisibility(wxT("Properties"));
}

void ToolBoxTestFrame::OnToggleToolBox(wxCommandEvent & event)
{
	TogglePaneVisibility(wxT("ToolBox"));
}

void ToolBoxTestFrame::OnToggleLogWindow(wxCommandEvent & event)
{
	TogglePaneVisibility(wxT("Event LOG"));
}

void ToolBoxTestFrame::OnToggleStatusBar(wxCommandEvent & event)
{
	wxStatusBar * statusBar = GetStatusBar();
	if(statusBar)
	{
		SetStatusBar(NULL);
		statusBar->Destroy();
	}
	else
	{
		CreateStatusBar();
	}
}

void ToolBoxTestFrame::OnTogglePropertiesUpdateUI(wxUpdateUIEvent & event)
{
	event.Check(GetPaneVisibility(wxT("Properties")));
}

void ToolBoxTestFrame::OnToggleToolBoxUpdateUI(wxUpdateUIEvent & event)
{
	event.Check(GetPaneVisibility(wxT("ToolBox")));
}

void ToolBoxTestFrame::OnToggleLogWindowUpdateUI(wxUpdateUIEvent & event)
{
	event.Check(GetPaneVisibility(wxT("Event LOG")));
}

void ToolBoxTestFrame::OnToggleStatusBarUpdateUI(wxUpdateUIEvent & event)
{
	event.Check(GetStatusBar() != NULL);
}

bool ToolBoxTestFrame::GetPaneVisibility(wxString pane_name)
{
	wxAuiPaneInfoArray& all_panes = m_FrameManager.GetAllPanes();
	size_t i, count;
	for (i = 0, count = all_panes.GetCount(); i < count; ++i)
	{		
		if(all_panes.Item(i).name == pane_name)
		{
			wxLogTrace(wxTRACE_Messages, wxT("%s: %i"), pane_name, (int)all_panes.Item(i).IsShown());
			return all_panes.Item(i).IsShown();
		}
	}
	return false;
}

void ToolBoxTestFrame::TogglePaneVisibility(wxString pane_name)
{
	wxAuiPaneInfoArray& all_panes = m_FrameManager.GetAllPanes();
	size_t i, count;
	for (i = 0, count = all_panes.GetCount(); i < count; ++i)
	{		
		if(all_panes.Item(i).name == pane_name)
		{
			//if(all_panes.Item(i).IsFloating())
			//{
			//	all_panes.Item(i).frame->Show(!all_panes.Item(i).frame->IsShown());
			//}
			//else
			//{
				all_panes.Item(i).Show(!all_panes.Item(i).IsShown());
			//}
			m_FrameManager.Update();
			break;
		}
	}
}

// wxToolBox Event Handlers

void ToolBoxTestFrame::OnToolBoxTabSelected(wxToolBoxEvent & event)
{	
	wxToolBoxTab * tab = event.GetTab();
	m_LOGTextCtrl->AppendText(_("TAB_SELECTED: "));
	if(tab)
	{
		m_LOGTextCtrl->AppendText(tab->GetCaption());
		int cnt = (int)tab->GetItemCount();
		if(cnt)
		{
			item_index_1_spin_ctrl->SetRange(0, cnt-1);
			item_index_2_spin_ctrl->SetRange(0, cnt-1);
		}
		item_index_1_spin_ctrl->Enable(cnt > 0);
		item_index_2_spin_ctrl->Enable(cnt > 0);
		tab_name_text_ctrl->SetValue(tab->GetCaption());
		tab_index_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		item_tab_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		tab_icon_spin_ctrl->SetValue(wxString::Format(wxT("%i"), tab->GetImageIndex()));
	} else m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxTabAdded(wxToolBoxEvent & event)
{	
	wxToolBoxTab * tab = event.GetTab();
	m_LOGTextCtrl->AppendText(_("TAB_ADDED: "));
	if(tab)
	{
		m_LOGTextCtrl->AppendText(tab->GetCaption());
		tab_index_spin_ctrl->Enable(true);
		tab_rename_button->Enable(true);
		tab_delete_button->Enable(true);
	} else m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxItemAdded(wxToolBoxEvent & event)
{	
	wxToolBoxItem * item = event.GetItem();
	m_LOGTextCtrl->AppendText(_("ITEM_ADDED: "));
	if(item)
	{
		m_LOGTextCtrl->AppendText(item->GetCaption());
	} else m_LOGTextCtrl->AppendText(_("Error retrieving item"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxItemsSwapped(wxToolBoxEvent & event)
{		
	wxToolBoxTab * tab = event.GetTab();
	wxToolBoxItem * item1;
	wxToolBoxItem * item2;
	m_LOGTextCtrl->AppendText(_("ITEMS_SWAPPED: "));
	if(tab)
	{
		m_LOGTextCtrl->AppendText(tab->GetCaption());
		item1 = (*tab)[event.GetToolObjectIndex1()];
		item2 = (*tab)[event.GetToolObjectIndex2()];
		m_LOGTextCtrl->AppendText(wxT(" ("));
		if(item1)
		{
			m_LOGTextCtrl->AppendText(item1->GetCaption());
		} else m_LOGTextCtrl->AppendText(wxT("ERROR"));
		m_LOGTextCtrl->AppendText(wxT(","));
		if(item2)
		{
			m_LOGTextCtrl->AppendText(item2->GetCaption());
		} else m_LOGTextCtrl->AppendText(wxT("ERROR"));
		m_LOGTextCtrl->AppendText(wxT(")"));
	} else m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxItemSelected(wxToolBoxEvent & event)
{	
	wxToolBoxItem * item = event.GetItem();
	m_LOGTextCtrl->AppendText(_("ITEM_SELECTED: "));
	if(item)
	{
		wxToolBoxTab * tab = item->GetParentTab();
		if(tab)
		{
			m_LOGTextCtrl->AppendText(tab->GetCaption());
			m_LOGTextCtrl->AppendText(wxString::Format(wxT(":ID=%i"), tab->GetID()));
		}
		m_LOGTextCtrl->AppendText(wxT(" ("));
		m_LOGTextCtrl->AppendText(item->GetCaption());
		m_LOGTextCtrl->AppendText(wxT(")"));		
		m_LOGTextCtrl->AppendText(wxString::Format(wxT(":ID=%i"), item->GetID()));
		item_tab_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		item_index_spin_ctrl->SetValue(wxString::Format(wxT("%i"), tab->IndexOfItem(item)));
		item_name_text_ctrl->SetValue(item->GetCaption());
		item_icon_spin_ctrl->SetValue(wxString::Format(wxT("%i"), item->GetImageIndex()));
	} else m_LOGTextCtrl->AppendText(_("Error retrieving item"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxTabsSwapped(wxToolBoxEvent & event)
{		
	wxToolBoxTab * tab1 = (*m_ToolBox)[event.GetToolObjectIndex1()];
	wxToolBoxTab * tab2 = (*m_ToolBox)[event.GetToolObjectIndex2()];	
	m_LOGTextCtrl->AppendText(_("TABS_SWAPPED: "));
	if(tab1 && tab2)
	{
		m_LOGTextCtrl->AppendText(tab1->GetCaption());
		m_LOGTextCtrl->AppendText(wxT(", "));
		m_LOGTextCtrl->AppendText(tab2->GetCaption());
	} else m_LOGTextCtrl->AppendText(_("Error retrieving tabs"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxItemDeleted(wxToolBoxEvent & event)
{	
	wxToolBoxTab * tab = event.GetTab();
	m_LOGTextCtrl->AppendText(_("ITEM_DELETED: "));
	if(tab)
	{
		int cnt = (int)tab->GetItemCount();
		m_LOGTextCtrl->AppendText(wxString::Format(_("%s (Item index = %i)"), 
			tab->GetCaption(), event.GetToolObjectIndex()));		
		item_index_spin_ctrl->Enable(cnt > 0);
		item_rename_button->Enable(cnt > 0);
		item_delete_button->Enable(cnt > 0);
	} else m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxTabDeleted(wxToolBoxEvent & event)
{	
	m_LOGTextCtrl->AppendText(_("TAB_DELETED: "));
	m_LOGTextCtrl->AppendText(wxString::Format(_("Tab index = %i"), 			
		event.GetToolObjectIndex()));
	int cnt = (int)m_ToolBox->GetTabs().GetCount();
	if(cnt)
	{
		tab_index_spin_ctrl->SetRange(0, cnt-1);
	}
	tab_index_spin_ctrl->Enable(cnt > 0);
	tab_rename_button->Enable(cnt > 0);
	tab_delete_button->Enable(cnt > 0);
	m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxTestFrame::OnToolBoxSkinSaved(wxToolBoxEvent & event)
{	
	m_LOGTextCtrl->AppendText(_("SKIN_SAVED\r\n"));
}

void ToolBoxTestFrame::OnToolBoxSkinLoaded(wxToolBoxEvent & event)
{	
	m_LOGTextCtrl->AppendText(_("SKIN_LOADED\r\n"));
	SynchronizeProperties();
}
