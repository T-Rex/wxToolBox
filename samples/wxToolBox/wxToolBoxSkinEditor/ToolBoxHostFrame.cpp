#include "wxToolBoxSkinEditorApp.h"
#include "ToolBoxHostFrame.h"
#include "ToolBoxTestFrame.h"

BEGIN_EVENT_TABLE(ToolBoxHostFrame, wxFrame)
EVT_MOVE(ToolBoxHostFrame::OnMove)
EVT_CLOSE(ToolBoxHostFrame::OnClose)

EVT_TB_TAB_ADDED(10001, ToolBoxHostFrame::OnToolBoxTabAdded)
EVT_TB_TAB_SELECTED(10001, ToolBoxHostFrame::OnToolBoxTabSelected)
EVT_TB_TAB_DELETED(10001, ToolBoxHostFrame::OnToolBoxTabDeleted)
EVT_TB_TABS_SWAPPED(10001, ToolBoxHostFrame::OnToolBoxTabsSwapped)

EVT_TB_ITEM_ADDED(10001, ToolBoxHostFrame::OnToolBoxItemAdded)
EVT_TB_ITEM_SELECTED(10001, ToolBoxHostFrame::OnToolBoxItemSelected)
EVT_TB_ITEM_DELETED(10001, ToolBoxHostFrame::OnToolBoxItemDeleted)
EVT_TB_ITEMS_SWAPPED(10001, ToolBoxHostFrame::OnToolBoxItemsSwapped)

EVT_TB_SKIN_LOADED(10001, ToolBoxHostFrame::OnToolBoxSkinLoaded)
EVT_TB_SKIN_SAVED(10001, ToolBoxHostFrame::OnToolBoxSkinSaved)
END_EVENT_TABLE()

ToolBoxHostFrame::ToolBoxHostFrame(wxWindow * parent)
: wxFrame(parent, -1, _("ToolBox parent window"), wxPoint(100,100), wxSize(200,500),
		  wxSYSTEM_MENU|wxCAPTION|wxCLOSE_BOX|wxDOUBLE_BORDER|wxTHICK_FRAME)
{
	m_ToolBox = new wxToolBox(this, 10001, wxDefaultPosition, wxSize(150,400), wxNO_BORDER);
	m_ToolBox->SetMinSize(wxSize(150,400));	
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
	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	sizer->Add(m_ToolBox, 1, wxEXPAND);		
	m_ToolBox->RefreshTabs();	
}

void ToolBoxHostFrame::OnMove(wxMoveEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	parent->dx = GetPosition().x-parent->GetPosition().x;
	parent->dy = GetPosition().y-parent->GetPosition().y;
}

void ToolBoxHostFrame::OnClose(wxCloseEvent & event)
{
	wxGetApp().GetTopWindow()->Close();
}

void ToolBoxHostFrame::OnToolBoxTabSelected(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxTab * tab = event.GetTab();
	parent->m_LOGTextCtrl->AppendText(_("TAB_SELECTED: "));
	if(tab)
	{
		parent->m_LOGTextCtrl->AppendText(tab->GetCaption());
		int cnt = (int)tab->GetItemCount();
		if(cnt)
		{
			parent->item_index_1_spin_ctrl->SetRange(0, cnt-1);
			parent->item_index_2_spin_ctrl->SetRange(0, cnt-1);
		}
		parent->item_index_1_spin_ctrl->Enable(cnt > 0);
		parent->item_index_2_spin_ctrl->Enable(cnt > 0);
		parent->tab_name_text_ctrl->SetValue(tab->GetCaption());
		parent->tab_index_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		parent->item_tab_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		parent->tab_icon_spin_ctrl->SetValue(wxString::Format(wxT("%i"), tab->GetImageIndex()));
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxTabAdded(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxTab * tab = event.GetTab();
	parent->m_LOGTextCtrl->AppendText(_("TAB_ADDED: "));
	if(tab)
	{
		parent->m_LOGTextCtrl->AppendText(tab->GetCaption());
		parent->tab_index_spin_ctrl->Enable(true);
		parent->tab_rename_button->Enable(true);
		parent->tab_delete_button->Enable(true);
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxItemAdded(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxItem * item = event.GetItem();
	parent->m_LOGTextCtrl->AppendText(_("ITEM_ADDED: "));
	if(item)
	{
		parent->m_LOGTextCtrl->AppendText(item->GetCaption());
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving item"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxItemsSwapped(wxToolBoxEvent & event)
{	
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxTab * tab = event.GetTab();
	wxToolBoxItem * item1;
	wxToolBoxItem * item2;
	parent->m_LOGTextCtrl->AppendText(_("ITEMS_SWAPPED: "));
	if(tab)
	{
		parent->m_LOGTextCtrl->AppendText(tab->GetCaption());
		item1 = (*tab)[event.GetToolObjectIndex1()];
		item2 = (*tab)[event.GetToolObjectIndex2()];
		parent->m_LOGTextCtrl->AppendText(wxT(" ("));
		if(item1)
		{
			parent->m_LOGTextCtrl->AppendText(item1->GetCaption());
		} else parent->m_LOGTextCtrl->AppendText(wxT("ERROR"));
		parent->m_LOGTextCtrl->AppendText(wxT(","));
		if(item2)
		{
			parent->m_LOGTextCtrl->AppendText(item2->GetCaption());
		} else parent->m_LOGTextCtrl->AppendText(wxT("ERROR"));
		parent->m_LOGTextCtrl->AppendText(wxT(")"));
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxItemSelected(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxItem * item = event.GetItem();
	parent->m_LOGTextCtrl->AppendText(_("ITEM_SELECTED: "));
	if(item)
	{
		wxToolBoxTab * tab = item->GetParentTab();
		if(tab)
		{
			parent->m_LOGTextCtrl->AppendText(tab->GetCaption());
			parent->m_LOGTextCtrl->AppendText(wxString::Format(wxT(":ID=%i"), tab->GetID()));
		}
		parent->m_LOGTextCtrl->AppendText(wxT(" ("));
		parent->m_LOGTextCtrl->AppendText(item->GetCaption());
		parent->m_LOGTextCtrl->AppendText(wxT(")"));		
		parent->m_LOGTextCtrl->AppendText(wxString::Format(wxT(":ID=%i"), item->GetID()));
		parent->item_tab_spin_ctrl->SetValue(wxString::Format(wxT("%i"), m_ToolBox->IndexOf(tab)));
		parent->item_index_spin_ctrl->SetValue(wxString::Format(wxT("%i"), tab->IndexOfItem(item)));
		parent->item_name_text_ctrl->SetValue(item->GetCaption());
		parent->item_icon_spin_ctrl->SetValue(wxString::Format(wxT("%i"), item->GetImageIndex()));
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving item"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxTabsSwapped(wxToolBoxEvent & event)
{	
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxTab * tab1 = (*m_ToolBox)[event.GetToolObjectIndex1()];
	wxToolBoxTab * tab2 = (*m_ToolBox)[event.GetToolObjectIndex2()];	
	parent->m_LOGTextCtrl->AppendText(_("TABS_SWAPPED: "));
	if(tab1 && tab2)
	{
		parent->m_LOGTextCtrl->AppendText(tab1->GetCaption());
		parent->m_LOGTextCtrl->AppendText(wxT(", "));
		parent->m_LOGTextCtrl->AppendText(tab2->GetCaption());
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving tabs"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxItemDeleted(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	wxToolBoxTab * tab = event.GetTab();
	parent->m_LOGTextCtrl->AppendText(_("ITEM_DELETED: "));
	if(tab)
	{
		int cnt = (int)tab->GetItemCount();
		parent->m_LOGTextCtrl->AppendText(wxString::Format(_("%s (Item index = %i)"), 
			tab->GetCaption(), event.GetToolObjectIndex()));		
		parent->item_index_spin_ctrl->Enable(cnt > 0);
		parent->item_rename_button->Enable(cnt > 0);
		parent->item_delete_button->Enable(cnt > 0);
	} else parent->m_LOGTextCtrl->AppendText(_("Error retrieving tab"));
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxTabDeleted(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;	
	parent->m_LOGTextCtrl->AppendText(_("TAB_DELETED: "));
	parent->m_LOGTextCtrl->AppendText(wxString::Format(_("Tab index = %i"), 			
		event.GetToolObjectIndex()));
	int cnt = (int)m_ToolBox->GetTabs().GetCount();
	if(cnt)
	{
		parent->tab_index_spin_ctrl->SetRange(0, cnt-1);
	}
	parent->tab_index_spin_ctrl->Enable(cnt > 0);
	parent->tab_rename_button->Enable(cnt > 0);
	parent->tab_delete_button->Enable(cnt > 0);
	parent->m_LOGTextCtrl->AppendText(_("\r\n"));
}

void ToolBoxHostFrame::OnToolBoxSkinSaved(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;	
	parent->m_LOGTextCtrl->AppendText(_("SKIN_SAVED\r\n"));
}

void ToolBoxHostFrame::OnToolBoxSkinLoaded(wxToolBoxEvent & event)
{
	wxWindow *win = GetParent();
    ToolBoxTestFrame * parent = wxDynamicCast(win, ToolBoxTestFrame);
	if(!parent) return;
	parent->m_LOGTextCtrl->AppendText(_("SKIN_LOADED\r\n"));
	parent->SynchronizeProperties();
}
