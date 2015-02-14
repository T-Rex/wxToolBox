#include "stdwx.h"
#include "wx/wxToolBox/wxToolBox.h"
#include <algorithm>
#include <wx/arrimpl.cpp>

WX_DEFINE_USER_EXPORTED_OBJARRAY(wxToolBoxIconArray)

enum wxToolBoxID
{
ID_TOOLBOX_LAYOUT_TIMER = 11001,

//Unused
ID_TOOBOX_UNUSED
};

using namespace std;

#define ID_TOOL_BOX_SCROLL_TIMER	10001

IMPLEMENT_DYNAMIC_CLASS(wxToolBox, wxControl)

BEGIN_EVENT_TABLE(wxToolBox, wxControl)
EVT_PAINT(wxToolBox::OnPaint)
EVT_ERASE_BACKGROUND(wxToolBox::OnEraseBackground)
EVT_SIZE(wxToolBox::OnSize)
EVT_TIMER(ID_TOOL_BOX_SCROLL_TIMER, wxToolBox::OnTimer)
EVT_CONTEXT_MENU(wxToolBox::OnContextMenu)
EVT_LEFT_DOWN(wxToolBox::OnMouseDown)
EVT_LEFT_UP(wxToolBox::OnMouseUp)
EVT_MOUSEWHEEL(wxToolBox::OnMouseWheel)
EVT_MOTION(wxToolBox::OnMouseMove)
EVT_LEAVE_WINDOW(wxToolBox::OnMouseLeave)
EVT_TIMER(ID_TOOLBOX_LAYOUT_TIMER, wxToolBox::OnTimer_LayoutElapsed)
#ifdef __WXGTK__
EVT_WINDOW_CREATE(wxToolBox::OnWindowCreate)
#endif
END_EVENT_TABLE()

wxToolBox::wxToolBox(wxWindow * parent, wxWindowID id, wxPoint pos, wxSize size, long style)
		: wxControl(parent, id, pos, size, style),
		m_Style(wxTB_STYLE_LIST),
		m_SelectedTab(NULL), m_OldSelectedTab(NULL),
		m_TabImageWidth(16), m_TabImageHeight(16),
		m_ItemImageWidth(16), m_ItemImageHeight(16),
		m_TabHeight(20), m_ItemHeight(20), m_IconSize(32), m_TabSpacing(1), m_ItemSpacing(1),
		m_TabCaptionSpacing(2), m_ItemCaptionSpacing(1),
		m_TextBox(NULL), m_UpScroll(NULL), m_DownScroll(NULL), 		
		m_UseBackground(true), m_UseTabBackground(false),
		m_UseItemNormalBackground(false), m_UseItemHoverBackground(false),
		m_UseItemSelectedBackground(false), m_UseButtonBackground(false),
		m_UseStdArrows(true), m_ItemsTransparent(false),
		m_MouseMoveFreezed(false), m_LayoutTimerActive(false),
		m_SelectNewTab(true), m_ShowSelectionAlways(true), m_PreDrawBackgrounds(false),
		m_LayoutTimer(NULL)
{
	m_LayoutTimer = new wxTimer(this, ID_TOOLBOX_LAYOUT_TIMER);
	m_BorderHighlightPen = wxPen(*wxWHITE, 1);	
	m_BorderShadowPen = wxPen(wxColour(128, 128, 128), 1);

	m_ToolBoxBackground = wxNullBitmap;
	m_ItemNormalColour = GetBackgroundColour();
	m_ItemHoverColour = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
	m_ItemSelectedColour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);

	m_TabNormalColour = GetBackgroundColour();
	m_TabHoverColour = GetBackgroundColour();
	m_TabSelectedColour = GetBackgroundColour();

	m_ButtonNormalColour = GetBackgroundColour();
	m_ButtonHoverColour = GetBackgroundColour();
	m_ButtonSelectedColour = GetBackgroundColour();

	m_TabNormalFont = GetFont();
	m_TabHoverFont = GetFont();
	m_TabSelectedFont = GetFont();

	m_ItemNormalFont = GetFont();
	m_ItemHoverFont = GetFont();
	m_ItemSelectedFont = GetFont();

	m_TabTextColour = *wxBLACK;
	m_ItemTextColour = *wxBLACK;

#if defined __WXMSW__ || __WXX11__
	m_TextBox = new wxToolItemEditorTextCtrl(this, NULL);
	m_TextBox->Show(false);
#endif

	m_UpScroll = new wxToolBoxScrollButton(this, Up, wxSize(m_TabHeight, m_TabHeight));
	m_DownScroll = new wxToolBoxScrollButton(this, Down, wxSize(m_TabHeight, m_TabHeight));

	m_Tabs.DeleteContents(true);
}
#ifdef __WXGTK__
void wxToolBox::OnWindowCreate(wxWindowCreateEvent & event)
{	
	m_TextBox = new wxToolItemEditorTextCtrl(this, NULL);
	m_TextBox->Show(false);
}
#endif

wxToolBox::~wxToolBox()
{
	if(m_LayoutTimer) delete m_LayoutTimer;
	if(m_UpScroll) delete m_UpScroll;
	if(m_DownScroll) delete m_DownScroll;
}

void wxToolBox::DrawBorders(wxDC & dc, const wxRect & rect, bool bSelected)
{
	wxPen const * p1;
    wxPen const *p2;
	if(bSelected)
	{
		p2 = &m_BorderHighlightPen;
		p1 = &m_BorderShadowPen;
	}
	else
	{
		p1 = &m_BorderHighlightPen;
		p2 = &m_BorderShadowPen;
	}
	dc.SetPen(*p1);
	dc.DrawLine(rect.GetLeft(), rect.GetTop(), rect.GetLeft(), rect.GetBottom());	
	dc.DrawLine(rect.GetLeft(), rect.GetTop(), rect.GetRight(), rect.GetTop());	
	dc.SetPen(*p2);
	dc.DrawLine(rect.GetRight(), rect.GetTop(), rect.GetRight(), rect.GetBottom());	
	dc.DrawLine(rect.GetLeft(), rect.GetBottom(), rect.GetRight()+1, rect.GetBottom());	
}

wxToolBoxItem * wxToolBox::GetEditingItem()
{
	if(!m_TextBox) return NULL;
	return m_TextBox->GetItem();
}

bool wxToolBox::EndRenameItem(bool bSave)
{
	bool bOK(true);	
	if(!m_TextBox) return true;
	if(m_TextBox->IsShown())
	{
		if(bSave)
		{
			if(OnRenameFinished(false))
			{
				UpdateItemFromTextBox();
			}
			else
			{
				m_TextBox->SetFocus();
				bOK = false;
			}
		}
		else
		{
			if(OnRenameFinished(true))
			{
				m_TextBox->Show(false);
			}
			else
			{
				m_TextBox->SetFocus();
				bOK = false;
			}
		}
	}
	return bOK;
}

bool wxToolBox::OnRenameFinished(bool bCancelled)
{
	bool bOK(true);
	return bOK;
}

void wxToolBox::UpdateItemFromTextBox()
{
	if(!m_TextBox) return;
	wxToolBoxItem * item = m_TextBox->GetItem();
	if(!item) return;
	if(m_TextBox->IsShown())
	{
		item->SetCaption(m_TextBox->GetValue());
		m_TextBox->Show(false);
		m_TextBox->SetItem(NULL);
	}
}

void wxToolBox::RefreshTabs()
{
	DoLayout(true, false, false);
	wxToolBoxTab * tab;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		tab = node->GetData();
		if(tab)
		{
			tab->UpdateItemRects(false);
		}
	}
	if(m_SelectedTab)
	{
		m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
		m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));
		m_SelectedTab->ShowChildWindow();
		if(m_SelectedTab->GetChildWindow())
		{
			wxLogTrace(wxTRACE_Messages, _("Selected tab %s [%ix%i] child (%i,%i) - (%ix%i)"),
				m_SelectedTab->GetCaption().GetData(),
				m_SelectedTab->GetItemArea().GetWidth(),
				m_SelectedTab->GetItemArea().GetWidth(),
				m_SelectedTab->GetChildWindow()->GetPosition().x,
				m_SelectedTab->GetChildWindow()->GetPosition().y,
				m_SelectedTab->GetChildWindow()->GetSize().GetWidth(),
				m_SelectedTab->GetChildWindow()->GetSize().GetHeight());
		}
	}
}

void wxToolBox::DoLayout(bool bInitial, bool bUpdateItems, bool bRepaint)
{
	wxToolBoxTab * tab1(NULL);
	wxToolBoxTab * tab2(NULL);
	wxRect rect, rcItem;
	bool bOK(true), bScrollUpSet(false), bScrollDnSet(false);
	int iLoop(0);
	rect.SetHeight(m_TabHeight);
	rect.SetWidth(GetSize().GetWidth());
	rect.SetX(0);
	rect.SetY(0);
	m_DownScroll->SetY(GetSize().GetHeight()-m_DownScroll->GetSize().GetHeight());	

	m_UpScroll->SetSize(m_TabHeight, m_TabHeight);
	m_DownScroll->SetSize(m_TabHeight, m_TabHeight);	
	
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); bOK && node; node = node->GetNext())
	{
		tab1 = node->GetData();		
		if(tab1)
		{
			if(!bInitial && (int)(m_TabHeight + (m_Tabs.GetCount()*rect.GetHeight())) > GetSize().GetHeight())
			{
				bOK = false;
				bScrollUpSet=true;
				bScrollDnSet=true;
				int afterSelTab = IndexOf(m_SelectedTab)+1;
				if(afterSelTab < (int)m_Tabs.GetCount())
				{
					wxToolBoxTab * tab = GetTab(afterSelTab);
					if(tab)
					{
						m_DownScroll->SetY(tab->GetRectangle().GetTop());
					}
				}
			}
			if(bOK)
			{
				tab1->SetRectangle(rect);
				tab1->SetItemArea(wxRect(0,0,0,0));

				if(tab2)
				{
					tab2        = NULL;				
					tab1->SetSize(tab1->GetSize().GetWidth()-(m_DownScroll->GetSize().GetWidth()),
						tab1->GetSize().GetHeight());
					m_DownScroll->SetX(tab1->GetRectangle().GetRight()+1);
					m_DownScroll->SetY(tab1->GetRectangle().GetTop());
					wxLogTrace(wxTRACE_Messages, wxT("DownScroll Y = %i"), tab1->GetRectangle().GetTop());
					bScrollDnSet=true;
				}
				else if(tab1->GetSelected())
				{
					int newY    = 0; 
					tab1->SetSize(GetSize().GetWidth()-(m_UpScroll->GetSize().GetWidth()),
						tab1->GetSize().GetHeight());
					m_UpScroll->SetX(tab1->GetRectangle().GetRight()+1);
					m_UpScroll->SetY(tab1->GetRectangle().GetTop());								
					tab2        = tab1;				
					bScrollUpSet= true;

					if(tab2)
					{
						newY   = (GetSize().GetHeight() - ((m_TabHeight + m_TabSpacing) * (m_Tabs.GetCount() - (iLoop))));
						newY   = (newY <= rect.GetY()+(int)(m_ItemHeight + m_ItemSpacing) ) ? rect.GetY() + (m_ItemHeight + m_ItemSpacing) : newY; 
						rect.SetY(newY);
					}
					else
					{
						rect.SetY(m_TabHeight + m_TabSpacing + m_ItemSpacing);
					}
				}
				rect.SetY(rect.GetY() + m_TabHeight + m_TabSpacing);
			}
			iLoop++;
		}
	}
	
	if(0 < m_Tabs.GetCount())
	{
		// Scroll button positions
		if(!bScrollUpSet)
		{
			m_UpScroll->SetX(GetSize().GetWidth() - (m_UpScroll->GetSize().GetWidth()+1));
			m_UpScroll->SetY(1);
		}

		if(!bScrollDnSet)
		{
			rect.SetY(wxMax((int)(rect.GetBottom()),(int)(GetSize().GetHeight())));
			m_DownScroll->SetX(GetSize().GetWidth() - (m_DownScroll->GetSize().GetWidth()));
			m_DownScroll->SetY(wxMin((int)(rect.GetY()), (int)(GetSize().GetHeight())) - (m_TabHeight));			
		}

		if(m_SelectedTab)
		{
			rcItem.SetX(m_SelectedTab->GetRectangle().GetX()+1);
			rcItem.SetY(m_SelectedTab->GetRectangle().GetBottom());
			rcItem.SetWidth(GetSize().GetWidth()-2);
			rcItem.SetHeight(m_DownScroll->GetRectangle().GetY()-rcItem.GetY());
			m_SelectedTab->SetItemArea(rcItem);
		}
	}
	if(m_SelectedTab)
	{
		if(bUpdateItems)
		{
			m_SelectedTab->UpdateItemRects(false);
		}

		//_selectedTab.DoItemLayout();
		m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
		m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));
	}
	if(m_OldSelectedTab)
	{
		//_oldselectedTab.DoItemLayout();
	}
	if(bRepaint && IsShown())
	{
		Refresh();
	}
}

int wxToolBox::AddTab(wxToolBoxTab * tab)
{		
	int index(-1);
	if(!tab) return index;	
	if(EndRenameItem(true))
	{
		/*for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
		{
			node->GetData()->SetSelected(false);
		}*/
		m_Tabs.Append(tab);
		tab->SetParent(this);		
		if(!m_SelectedTab) 
		{
			tab->SetSelected(true);
		}
		index = m_Tabs.IndexOf(tab);		
		//RefreshTabs();
		DoLayout(false, true, true);
		Refresh();		
		wxToolBoxEvent evt( wxEVT_TB_TAB_ADDED, GetId());				
		evt.SetEventObject(this);
		evt.SetTab(tab);
		GetEventHandler()->AddPendingEvent(evt);
		if(m_SelectNewTab && !tab->GetSelected())
		{
			tab->SetSelected(true);			
		}
	}	
	return index;
}

void wxToolBox::OnPaint(wxPaintEvent & event)
{	
	int i,j;
	wxBufferedPaintDC mdc(this);	
	wxRect r(0,0, GetClientSize().GetWidth(), GetClientSize().GetHeight());
	if(!m_UseBackground || !m_ToolBoxBackground.Ok())
	{
		mdc.SetBackground(GetBackgroundColour());
		mdc.Clear();
	}
	else
	{
		wxCoord w, h;
		mdc.GetSize(&w, &h);
		for(i = 0; i < w; i+= m_ToolBoxBackground.GetWidth())
		{
			for(j = 0; j < h; j+= m_ToolBoxBackground.GetHeight())
			{
				mdc.DrawBitmap(m_ToolBoxBackground, i, j, true);
			}
		}
	}
	wxToolBoxTab * tab;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		tab = node->GetData();
		if(tab)
		{
			tab->OnPaint(mdc, r);
		}
	}
	m_UpScroll->Paint(mdc, r);	
	if(m_DownScroll->GetRectangle().GetY() >= m_UpScroll->GetRectangle().GetBottom())
	{		
		m_DownScroll->Paint(mdc, r);		
	}
	//dc.Blit(0, 0, r.GetWidth(), r.GetHeight(), &mdc, 0, 0);
}

void wxToolBox::OnEraseBackground(wxEraseEvent & event)
{
}

void wxToolBox::OnSize(wxSizeEvent & event)
{
	if(m_PreDrawBackgrounds)
	{
		PreDrawBackgrounds();
	} else if(m_UseItemNormalBackground || m_UseItemHoverBackground || m_UseItemSelectedBackground)
	{
		PreDrawItemBackground();
	}
	DoLayout(false, true, false);
	wxToolBoxTab * tab;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		tab = node->GetData();
		if(tab)
		{
			tab->UpdateItemRects(false, true, false);						
		}
	}
	if(m_SelectedTab)
	{
		m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
		m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));
		m_SelectedTab->ShowChildWindow();
		if(m_SelectedTab->GetChildWindow())
		{
			wxLogTrace(wxTRACE_Messages, _("Selected tab %s [%ix%i] child (%i,%i) - (%ix%i)"),
				m_SelectedTab->GetCaption().GetData(),
				m_SelectedTab->GetItemArea().GetWidth(),
				m_SelectedTab->GetItemArea().GetWidth(),
				m_SelectedTab->GetChildWindow()->GetPosition().x,
				m_SelectedTab->GetChildWindow()->GetPosition().y,
				m_SelectedTab->GetChildWindow()->GetSize().GetWidth(),
				m_SelectedTab->GetChildWindow()->GetSize().GetHeight());
		}
		wxLogTrace(wxTRACE_Messages, wxT("Selected tab rect [%i,%i], [w=%i,h=%i]"),
			m_SelectedTab->GetRectangle().GetX(), m_SelectedTab->GetRectangle().GetY(),
			m_SelectedTab->GetRectangle().GetWidth(), m_SelectedTab->GetRectangle().GetHeight());
	}
	if(!m_Tabs.GetCount())
	{
		m_UpScroll->GetRectangle().SetX(GetSize().GetWidth()-m_UpScroll->GetRectangle().GetWidth());
		m_UpScroll->GetRectangle().SetY(0);
		m_DownScroll->GetRectangle().SetX(GetSize().GetWidth()-m_DownScroll->GetRectangle().GetWidth());
		m_DownScroll->GetRectangle().SetY(GetSize().GetHeight()-m_DownScroll->GetRectangle().GetHeight());
	}	
	Refresh();	
}

void wxToolBox::OnTimer(wxTimerEvent & event)
{
}

void wxToolBox::OnContextMenu(wxContextMenuEvent & event)
{
}

void wxToolBox::OnMouseDown(wxMouseEvent & event)
{	
	EndRenameItem(true);
	SetFocus();
	if(m_LayoutTimer->IsRunning()) 
	{		
		return;
	}
	bool btn_event(false), bOldState;
	if(m_SelectedTab)
	{
		if(m_UpScroll->GetRectangle().Contains(event.GetPosition()))
		{		
			m_UpScroll->SetMouseDown(true);
			bOldState = m_DownScroll->GetEnabled();
			m_SelectedTab->ScrollItems(Up);
			m_SelectedTab->bDebug = true;
			if(bOldState != m_DownScroll->GetEnabled())
			{
				m_DownScroll->SetMouseDown(false);			
			}
			btn_event = true;
		}
		if(m_DownScroll->GetRectangle().Contains(event.GetPosition()))
		{
			m_DownScroll->SetMouseDown(true);
			bOldState = m_UpScroll->GetEnabled();			
			m_SelectedTab->ScrollItems(Down);			
			m_SelectedTab->bDebug = true;
			m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
			m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));			
			if(bOldState != m_UpScroll->GetEnabled())
			{
				m_UpScroll->SetMouseDown(false);			
			}
			btn_event = true;
		}
	}
	if(!btn_event)
	{
		wxToolBoxTab * tab;
		for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
		{
			tab = node->GetData();
			if(tab)
			{
				tab->OnMouseDown(event);			
			}
		}
	}
	Refresh();
}

void wxToolBox::OnMouseUp(wxMouseEvent & event)
{
	m_UpScroll->SetMouseDown(false);
	m_DownScroll->SetMouseDown(false);
	wxToolBoxTab * tab;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		tab = node->GetData();
		if(tab)
		{
			tab->OnMouseUp(event);
		}
	}
	Refresh();
}

void wxToolBox::OnTabSelectionChanged(wxToolBoxTab * newtab, wxToolBoxTab * oldtab)
{
	wxToolBoxTab * oldTab = m_SelectedTab;	
	if(oldTab != newtab)
	{
		if(oldTab)
		{			
			oldTab->SetSelected(false);
			oldTab->HideChildWindow();
		}		
		m_SelectedTab = newtab;		
		m_OldSelectedTab = oldTab;		
		m_LayoutTimer->Start(10);
	}
}

void wxToolBox::OnTabMouseDown(wxToolBoxTab * tab, wxMouseEvent & event) 
{
	wxToolBoxTab * t;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		t = node->GetData();
		if(t == tab)
		{			
			t->SetSelected(true);			
			break;
		}
	}
}

void wxToolBox::CreateImageListFromBitmap(wxToolBoxIconArray & image_list, wxBitmap & bmp, 
										  int width, int height, bool clear_existing)
{
	if(clear_existing)
	{
		//m_ImageList.RemoveAll();
		image_list.Clear();
	}
	if(!bmp.GetWidth() || !bmp.GetHeight() || !width || !height) return;				
	wxBitmap tmpIcon;
	for(int i = 0; i < bmp.GetWidth(); i+=width)
	{					
		tmpIcon = bmp.GetSubBitmap(
			wxRect(i, 0, wxMin((int)width, (int)(bmp.GetWidth()-i)), wxMin((int)height, (int)bmp.GetHeight())));
		image_list.Add(tmpIcon);		
	}
}

void wxToolBox::OnMouseWheel(wxMouseEvent & event)
{
	if(!m_SelectedTab) return;
	if(event.GetWheelDelta())
	{
		double delta = (double)event.GetWheelRotation()/(double)event.GetWheelDelta();	
		if(delta > 0)
		{		
			m_SelectedTab->ScrollItems(Up);
			m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
			m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));
		}
		else
		{
			m_SelectedTab->ScrollItems(Down);
			m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
			m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));
		}
	}
	Refresh();
}

void wxToolBox::SetBackground(wxString background)
{
	m_ToolBoxBackgroundPath = background;
	if(background.IsEmpty()) 
	{
		m_ToolBoxBackground = wxNullBitmap;
		return;
	}
	m_ToolBoxBackground.LoadFile(background, wxBITMAP_TYPE_ANY);
	Refresh();
}

const wxBitmap & wxToolBox::GetBackground()
{
	return m_ToolBoxBackground;
}

void wxToolBox::SetItemBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected)
{
	m_ItemBackground.SetNormalBackground(normal);
	m_ItemBackground.SetHoverBackground(hover);
	m_ItemBackground.SetSelectedBackground(selected);
}

void wxToolBox::SetNormalItemBackground(wxToolBoxBitmapSet & background)
{
	m_ItemBackground.SetNormalBackground(background);
}

void wxToolBox::SetHoverItemBackground(wxToolBoxBitmapSet & background)
{
	m_ItemBackground.SetHoverBackground(background);
}

void wxToolBox::SetSelectedlItemBackground(wxToolBoxBitmapSet & background)
{
	m_ItemBackground.SetSelectedBackground(background);
}
void wxToolBox::SetTabBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected)
{
	m_TabBackground.SetNormalBackground(normal);
	m_TabBackground.SetHoverBackground(hover);
	m_TabBackground.SetSelectedBackground(selected);
}

void wxToolBox::SetNormalTabBackground(wxToolBoxBitmapSet & background)
{
	m_TabBackground.SetNormalBackground(background);
}

void wxToolBox::SetHoverTabBackground(wxToolBoxBitmapSet & background)
{
	m_TabBackground.SetHoverBackground(background);
}

void wxToolBox::SetSelectedlTabBackground(wxToolBoxBitmapSet & background)
{
	m_TabBackground.SetSelectedBackground(background);
}

void wxToolBox::SetButtonBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected)
{
	m_ButtonBackground.SetNormalBackground(normal);
	m_ButtonBackground.SetHoverBackground(hover);
	m_ButtonBackground.SetSelectedBackground(selected);
}

void wxToolBox::SetNormalButtonBackground(wxToolBoxBitmapSet & background)
{
	m_ButtonBackground.SetNormalBackground(background);
}

void wxToolBox::SetHoverButtonBackground(wxToolBoxBitmapSet & background)
{
	m_ButtonBackground.SetHoverBackground(background);
}

void wxToolBox::SetSelectedlButtonBackground(wxToolBoxBitmapSet & background)
{
	m_ButtonBackground.SetSelectedBackground(background);
}

void wxToolBox::UseSkin(bool use_skin)
{
	SetUseBackground(use_skin);
	SetUseTabBackground(use_skin);
	SetUseItemBackground(use_skin);
	SetUseButtonBackground(use_skin);
}

void wxToolBox::OnMouseMove(wxMouseEvent & event)
{
	if(m_LayoutTimer->IsRunning()) return;
	wxToolBoxTab * t;
	m_UpScroll->SetMouseHover(false);
	m_DownScroll->SetMouseHover(false);
	if(m_UpScroll->HitTest(event.GetPosition().x, event.GetPosition().y))
	{
		m_UpScroll->SetMouseHover(true);		
		if(m_SelectedTab)
		{
			m_SelectedTab->CancelHotItemHover();
			m_SelectedTab->CancelHover();
		}
		UpdateToolTip(m_UpScroll);
	}
	else if(m_DownScroll->HitTest(event.GetPosition().x, event.GetPosition().y))
	{
		m_DownScroll->SetMouseHover(true);		
		if(m_SelectedTab)
		{
			m_SelectedTab->CancelHotItemHover();
			m_SelectedTab->CancelHover();
		}
		UpdateToolTip(m_DownScroll);
	}
	m_UpScroll->Invalidate();
	m_DownScroll->Invalidate();

	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		t = node->GetData();
		t->OnMouseMove(event);
	}		
}

void wxToolBox::OnMouseLeave(wxMouseEvent & event)
{
	m_UpScroll->SetMouseHover(false);
	m_DownScroll->SetMouseHover(false);
	m_UpScroll->SetMouseDown(false);
	m_DownScroll->SetMouseDown(false);
	wxToolBoxTab * t;	
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		t = node->GetData();
		t->SetMouseDown(false);
		t->SetMouseHover(false);
		t->CancelHotItemHover();
	}
	Refresh();
}

bool wxToolBox::DoTimedLayout(wxToolBoxTab * oldSelectedTab)
{
	bool bFinished = false;

	if(!oldSelectedTab)
	{
		return true;
	}

	int        oldTabIndex;
	int        selTabIndex;
	int        iLoop = 0;
	wxToolBoxTab * tab(NULL);
	wxRect oldItemArea;
	wxRect selItemArea;
	wxRect paintRect = wxRect(GetSize());

	oldTabIndex = m_Tabs.IndexOf(m_OldSelectedTab);
	if(oldTabIndex == wxNOT_FOUND) return true;
	selTabIndex = m_Tabs.IndexOf(m_SelectedTab);
	if(selTabIndex == wxNOT_FOUND) return true;

	oldItemArea = m_OldSelectedTab->GetItemArea();
	selItemArea = m_SelectedTab->GetItemArea();

	int maxY = 0;
	int inc  = 0;
	
	inc = ((int)m_TabHeight/2 >= inc) ? (int)(oldItemArea.GetHeight()/6)-2	: inc;		
	inc = ((int)m_TabHeight/2 >= inc) ? (int)(oldItemArea.GetHeight()/3)-2	: inc;
	inc = ((int)m_TabHeight/2 >= inc) ? (int)(oldItemArea.GetHeight()/2)-2	: inc;
	inc = ((int)m_TabHeight/2 >= inc) ? (int)m_TabHeight/2					: inc;
	if(inc < 0) inc = m_TabHeight/2;
	

	wxToolBoxTab * tmpTab;
	if(oldTabIndex < selTabIndex)
	{
		// Selected tab is below the old tab.
		tab = (*this)[oldTabIndex+1];
		if(!tab) return true;

		maxY = m_OldSelectedTab->GetRectangle().GetBottom() + m_TabSpacing;

		if((tab->GetRectangle().GetTop()-inc) <= maxY)
		{
			inc = tab->GetRectangle().GetTop() - maxY;
		}

		paintRect.SetY(m_OldSelectedTab->GetRectangle().GetTop());

		for(iLoop = oldTabIndex+1; iLoop <= selTabIndex; iLoop++)
		{
			tmpTab = (*this)[iLoop];
			if(tmpTab)
			{
				tmpTab->GetRectangle().SetTop(tmpTab->GetRectangle().GetTop()-inc);				
			}
		}

		tab->GetRectangle().SetWidth(GetSize().GetWidth());

		oldItemArea.SetHeight((tab->GetRectangle().GetTop()-1) - (m_OldSelectedTab->GetRectangle().GetBottom()-1));
		m_SelectedTab->GetRectangle().SetWidth((GetSize().GetWidth()) - (m_UpScroll->GetRectangle().GetWidth()));
		m_OldSelectedTab->GetRectangle().SetWidth(GetSize().GetWidth());
		tab = (*this)[selTabIndex+1];
		if(tab)
		{
			tab->GetRectangle().SetWidth((GetSize().GetWidth()) - (m_UpScroll->GetRectangle().GetWidth()));
			m_DownScroll->SetY(tab->GetRectangle().GetTop());
			paintRect.SetHeight(m_DownScroll->GetRectangle().GetBottom()-paintRect.GetTop());
		}
		else
		{
			m_DownScroll->GetRectangle().SetTop(m_SelectedTab->GetRectangle().GetBottom()+m_TabSpacing+m_ItemHeight);
			m_DownScroll->GetRectangle().SetTop(m_DownScroll->GetRectangle().GetBottom()<GetSize().GetHeight() ? 
				GetSize().GetHeight()-m_TabHeight : m_DownScroll->GetRectangle().GetTop());
		}

		m_UpScroll->GetRectangle().SetTop(m_SelectedTab->GetRectangle().GetTop());
		tab = (*this)[oldTabIndex+1];
		if(tab)
		{
			if(tab->GetRectangle().GetTop() <= maxY)
			{
				oldItemArea.SetHeight(0);
				bFinished = true;
			}
		}
	}
	else
	{				
		maxY = GetSize().GetHeight()-((m_TabHeight + m_TabSpacing)*(m_Tabs.GetCount()-(selTabIndex+1)));
		//maxY = (maxY <= _selectedTab.Y + (_itemHeight + _itemSpacing) ) ? _selectedTab.Y + (2*_itemHeight) + 2*_itemSpacing : maxY; 
		maxY = (maxY <= m_SelectedTab->GetRectangle().GetTop()+(int)(m_ItemHeight+m_ItemSpacing)) ? 
			m_SelectedTab->GetRectangle().GetBottom()+(m_ItemHeight+m_ItemSpacing+m_TabSpacing) : maxY; 

		paintRect.SetTop(m_SelectedTab->GetRectangle().GetTop());

		tab = (*this)[selTabIndex+1];
		if(!tab) return true;				
		if((tab->GetRectangle().GetTop()+inc) >= maxY)
		{
			inc = maxY - tab->GetRectangle().GetTop();
		}

		for(iLoop = selTabIndex+1; iLoop <= oldTabIndex; iLoop++)
		{
			tmpTab = (*this)[iLoop];
			if(tmpTab)
			{				
				tmpTab->GetRectangle().SetTop(tmpTab->GetRectangle().GetTop()+inc);
			}			
		}

		m_SelectedTab->GetRectangle().SetWidth((GetSize().GetWidth())-(m_UpScroll->GetRectangle().GetWidth()+1));
		tab->GetRectangle().SetWidth(m_SelectedTab->GetRectangle().GetWidth());
		m_UpScroll->SetY(m_SelectedTab->GetRectangle().GetTop());
		m_DownScroll->SetY(tab->GetRectangle().GetY());

		if(selTabIndex+1 != oldTabIndex)
		{
			oldSelectedTab->GetRectangle().SetWidth(GetSize().GetWidth());
		}
		tab = (*this)[oldTabIndex+1];

		if(tab)
		{
			tab->GetRectangle().SetWidth(GetSize().GetWidth());
			paintRect.SetHeight(tab->GetRectangle().GetBottom()-paintRect.GetTop());
			oldItemArea.SetHeight((tab->GetRectangle().GetTop()-1)-(m_OldSelectedTab->GetRectangle().GetBottom()+1));
		}
		else
		{
			oldItemArea.SetHeight((GetSize().GetHeight())-((m_OldSelectedTab->GetRectangle().GetBottom())+m_ItemHeight));
		}

		tab = (*this)[selTabIndex+1];
		if(tab)
		{
			if(tab->GetRectangle().GetTop() >= maxY)
			{
				oldItemArea.SetHeight(0);
				bFinished = true;
			}
		}
	}
	selItemArea.SetLeft(m_SelectedTab->GetRectangle().GetLeft());
	selItemArea.SetTop(m_SelectedTab->GetRectangle().GetBottom());
	selItemArea.SetWidth(GetSize().GetWidth());

	oldItemArea.SetLeft(m_OldSelectedTab->GetRectangle().GetLeft());
	oldItemArea.SetTop(m_OldSelectedTab->GetRectangle().GetBottom());
	oldItemArea.SetWidth(GetSize().GetWidth());

	selItemArea.SetHeight(m_DownScroll->GetRectangle().GetTop()-m_SelectedTab->GetRectangle().GetBottom());

	m_OldSelectedTab->SetItemArea(oldItemArea);
	m_SelectedTab->SetItemArea(selItemArea);

	m_UpScroll->SetEnabled(m_SelectedTab->CanScroll(Up));
	m_DownScroll->SetEnabled(m_SelectedTab->CanScroll(Down));

	RefreshRect(paintRect);
	return bFinished;
}

wxToolBoxTab * wxToolBox::operator [] (int index)
{
	if(index < 0 || index >= (int)m_Tabs.GetCount()) return NULL;
	wxToolBoxTabList::Node * node = m_Tabs.Item(index);
	if(!node) return NULL;
	return node->GetData();
}

void wxToolBox::OnTimer_LayoutElapsed(wxTimerEvent & event)
{
	if(m_OldSelectedTab)
	{
		m_OldSelectedTab->HideChildWindow();
	}
	if(DoTimedLayout(m_OldSelectedTab))
	{
		m_LayoutTimer->Stop();
		if(m_SelectedTab) m_SelectedTab->ShowChildWindow();
		wxPoint pt = wxGetMousePosition();
		pt = this->ScreenToClient(pt);
		wxMouseEvent me(wxEVT_MOTION);
		me.m_x = pt.x;
		me.m_y = pt.y;
		ProcessEvent(me);
		RefreshTabs();
		//if(m_SelectedTab) m_SelectedTab->UpdateItemRects(true, false, true);
	}
}

void wxToolBox::LoadToolItemParameters(wxToolBoxItem * item, wxXmlNode * node)
{
	wxString tmp_str;
	int int_value;
	if(!item || !node) return;	
	tmp_str = node->GetAttribute(_("Caption"), wxEmptyString);
	item->SetCaption(tmp_str);
	tmp_str = node->GetAttribute(_("Deletable"), _("1"));
	int_value = wxAtoi(tmp_str);
	item->SetDeletable((bool)!!int_value);
	tmp_str = node->GetAttribute(_("Movable"), _("1"));
	int_value = wxAtoi(tmp_str);
	item->SetMovable((bool)!!int_value);
	tmp_str = node->GetAttribute(_("Renamable"), _("1"));
	int_value = wxAtoi(tmp_str);
	item->SetRenamable((bool)!!int_value);
	tmp_str = node->GetAttribute(_("Enabled"), _("1"));
	int_value = wxAtoi(tmp_str);
	item->SetEnabled((bool)!!int_value);
	tmp_str = node->GetAttribute(_("ImageIndex"), _("1"));
	int_value = wxAtoi(tmp_str);
	item->SetImageIndex(int_value);
}

bool wxToolBox::LoadStructure(wxString path_to_structure)
{
	wxString cwd = wxGetCwd();
	wxSetWorkingDirectory(wxFileName(path_to_structure).GetPath());	
	wxXmlDocument skin_xml_doc(path_to_structure);
	if(!skin_xml_doc.IsOk()) return false;
	wxXmlNode * root_node = skin_xml_doc.GetRoot();
	wxXmlNode * curnode = root_node->GetChildren();
	wxXmlNode * itemnode;
	wxString tmp_str;
	m_Tabs.Clear();
	wxToolBoxTab * new_tab;
	wxToolBoxItem new_item;
	int int_value(0);
	while(curnode)
	{
		if(curnode->GetName() == _("Tab"))
		{
			new_tab = new wxToolBoxTab;
			LoadToolItemParameters(new_tab, curnode);
			tmp_str = curnode->GetAttribute(_("Style"), wxT("0"));
			int_value = wxAtoi(tmp_str);
			new_tab->SetStyle(IntToToolBoxTabStyle(int_value), false);
			itemnode = curnode->GetChildren();
			while(itemnode)
			{
				if(itemnode->GetName() == _("Item"))
				{
					LoadToolItemParameters(&new_item, itemnode);
					new_tab->AddItem(new_item);
				}				
				itemnode = itemnode->GetNext();
			}
			AddTab(new_tab);
		}
		curnode = curnode->GetNext();
	}
	wxToolBoxEvent evt( wxEVT_TB_STRUCTURE_LOADED, GetId());
	evt.SetEventObject(this);
	GetEventHandler()->AddPendingEvent(evt);
	return true;
}

bool wxToolBox::LoadSkin(wxString path_to_skin)
{
	wxString cwd = wxGetCwd();
	wxSetWorkingDirectory(wxFileName(path_to_skin).GetPath());	
	wxXmlDocument skin_xml_doc(path_to_skin);
	if(!skin_xml_doc.IsOk()) return false;
	wxXmlNode * root_node = skin_xml_doc.GetRoot();
	wxXmlNode * curnode = root_node->GetChildren();
	wxXmlNode * paramnode;	
	int a,b,c;
	wxString tmp_str, left_path, middle_path, right_path, bg_path;
	wxToolBoxBitmapSet button_normal, button_hover, button_selected, 
		tab_normal, tab_hover, tab_selected, item_normal, item_hover,
		item_selected;
	int font_encoding, font_family, font_underlined, font_size, font_weight, font_style;
	wxFont font;
	wxColour colour;
	bool apply_style(false);
	while(curnode)
	{
		if(curnode->GetName() == _("Style"))
		{
			tmp_str = curnode->GetAttribute(_("StyleValue"), _("0"));
			m_Style = wxAtoi(tmp_str);
			apply_style = true;			
		}
		else if(curnode->GetName() == _("BackgroundColour"))
		{
			tmp_str = curnode->GetAttribute(_("Red"), _("192"));
			a = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Green"), _("192"));
			b = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Blue"), _("192"));
			c = wxAtoi(tmp_str);
			SetBackgroundColour(wxColour(a,b,c));			
		}
		else if(curnode->GetName() == _("BorderHighlightColour"))
		{
			tmp_str = curnode->GetAttribute(_("Red"), _("192"));
			a = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Green"), _("192"));
			b = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Blue"), _("192"));
			c = wxAtoi(tmp_str);
			colour = wxColour(a,b,c);
			SetBorderHighlightColour(colour);			
		}
		else if(curnode->GetName() == _("BorderShadowColour"))
		{
			tmp_str = curnode->GetAttribute(_("Red"), _("192"));
			a = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Green"), _("192"));
			b = wxAtoi(tmp_str);
			tmp_str = curnode->GetAttribute(_("Blue"), _("192"));
			c = wxAtoi(tmp_str);
   		    colour = wxColour(a,b,c);
			SetBorderShadowColour(colour);			
		}
		else if(curnode->GetName() == _("Background"))
		{
			tmp_str = curnode->GetAttribute(_("Path"), wxEmptyString);
			SetBackground(tmp_str);			
		}
		else if(curnode->GetName() == _("TabIcons"))
		{
			bg_path = curnode->GetAttribute(_("Path"), wxEmptyString);
			tmp_str = curnode->GetAttribute(_("ImageWidth"), _("16"));
			a = wxAtoi(tmp_str);
			SetTabImageWidth(a);
			tmp_str = curnode->GetAttribute(_("ImageHeight"), _("16"));
			b = wxAtoi(tmp_str);			
			SetTabImageHeight(b);
			CreateTabImageList(bg_path, a, b, true);			
		}
		else if(curnode->GetName() == _("ItemIcons"))
		{
			bg_path = curnode->GetAttribute(_("Path"), wxEmptyString);
			tmp_str = curnode->GetAttribute(_("ImageWidth"), _("16"));
			a = wxAtoi(tmp_str);
			SetItemImageWidth(a);
			tmp_str = curnode->GetAttribute(_("ImageHeight"), _("16"));
			b = wxAtoi(tmp_str);			
			SetItemImageHeight(b);
			CreateItemImageList(bg_path, a, b, true);			
		}
		else if(curnode->GetName() == _("Flags"))
		{
			tmp_str = curnode->GetAttribute(_("ItemsTransparent"), _("1"));
			SetItemsTransparent((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("SelectedItemTransparent"), _("1"));
			SetSelectedItemTransparent((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("UseBackground"), _("1"));
			SetUseBackground((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("UseStdArrows"), _("1"));
			SetUseStdArrows((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("SelectNewTab"), _("1"));
			SetSelectNewTab((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("PreDrawBackgrounds"), _("0"));
			SetPreDrawBackgrounds((bool)!!wxAtoi(tmp_str));
			tmp_str = curnode->GetAttribute(_("ShowSelectionAlways"), _("0"));
			SetShowSelectionAlways((bool)!!wxAtoi(tmp_str));			
		}
		else if(curnode->GetName() == _("Buttons"))
		{
			paramnode = curnode->GetChildren();
			while(paramnode)
			{
				if(paramnode->GetName() == _("ButtonNormalColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetButtonNormalColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("ButtonHoverColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetButtonHoverColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("ButtonSelectedColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetButtonSelectedColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("Flags"))
				{
					tmp_str = paramnode->GetAttribute(_("UseButtonBackground"), _("1"));
					SetUseButtonBackground((bool)!!wxAtoi(tmp_str));					
				}
				else if(paramnode->GetName() == _("NormalBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					button_normal.SetLeftBitmap(left_path);
					button_normal.SetMiddleBitmap(middle_path);
					button_normal.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("HoverBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					button_hover.SetLeftBitmap(left_path);
					button_hover.SetMiddleBitmap(middle_path);
					button_hover.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("SelectedBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					button_selected.SetLeftBitmap(left_path);
					button_selected.SetMiddleBitmap(middle_path);
					button_selected.SetRightBitmap(right_path);					
				}
				paramnode = paramnode->GetNext();
			}			
		}
		else if(curnode->GetName() == _("Tabs"))
		{
			paramnode = curnode->GetChildren();
			while(paramnode)
			{				
				if(paramnode->GetName() == _("TabNormalColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetTabNormalColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("TabHoverColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetTabHoverColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("TabSelectedColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetTabSelectedColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("TabTextColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetTabTextColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("Flags"))
				{
					tmp_str = paramnode->GetAttribute(_("UseTabBackground"), _("1"));
					SetUseTabBackground((bool)!!wxAtoi(tmp_str));					
				}
				else if(paramnode->GetName() == _("Sizes"))
				{
					tmp_str = paramnode->GetAttribute(_("TabHeight"), _("20"));
					SetTabHeight(wxAtoi(tmp_str));
					tmp_str = paramnode->GetAttribute(_("TabSpacing"), _("1"));
					SetTabSpacing(wxAtoi(tmp_str));					
				}
				else if(paramnode->GetName() == _("NormalBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					tab_normal.SetLeftBitmap(left_path);
					tab_normal.SetMiddleBitmap(middle_path);
					tab_normal.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("HoverBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					tab_hover.SetLeftBitmap(left_path);
					tab_hover.SetMiddleBitmap(middle_path);
					tab_hover.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("SelectedBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					tab_selected.SetLeftBitmap(left_path);
					tab_selected.SetMiddleBitmap(middle_path);
					tab_selected.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("TabNormalFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetTabNormalFont(font);					
				}
				else if(paramnode->GetName() == _("TabHoverFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetTabHoverFont(font);					
				}
				else if(paramnode->GetName() == _("TabSelectedFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetTabSelectedFont(font);					
				}
				paramnode = paramnode->GetNext();
			}			
		}
		else if(curnode->GetName() == _("Items"))
		{
			paramnode = curnode->GetChildren();
			while(paramnode)
			{
				if(paramnode->GetName() == _("ItemNormalColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetItemNormalColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("ItemHoverColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetItemHoverColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("ItemSelectedColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetItemSelectedColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("ItemTextColour"))
				{
					tmp_str = paramnode->GetAttribute(_("Red"), _("192"));
					a = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Green"), _("192"));
					b = wxAtoi(tmp_str);
					tmp_str = paramnode->GetAttribute(_("Blue"), _("192"));
					c = wxAtoi(tmp_str);
					SetItemTextColour(wxColour(a,b,c));					
				}
				else if(paramnode->GetName() == _("Flags"))
				{
					tmp_str = paramnode->GetAttribute(_("UseItemNormalBackground"), _("0"));
					SetUseItemNormalBackground((bool)!!wxAtoi(tmp_str));
					tmp_str = paramnode->GetAttribute(_("UseItemHoverBackground"), _("1"));
					SetUseItemHoverBackground((bool)!!wxAtoi(tmp_str));
					tmp_str = paramnode->GetAttribute(_("UseItemSelectedBackground"), _("1"));
					SetUseItemSelectedBackground((bool)!!wxAtoi(tmp_str));					
				}
				else if(paramnode->GetName() == _("Sizes"))
				{
					tmp_str = paramnode->GetAttribute(_("ItemHeight"), _("20"));
					SetItemHeight(wxAtoi(tmp_str));
					tmp_str = paramnode->GetAttribute(_("ItemSpacing"), _("1"));
					SetItemSpacing(wxAtoi(tmp_str));					
				}
				else if(paramnode->GetName() == _("NormalBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					item_normal.SetLeftBitmap(left_path);
					item_normal.SetMiddleBitmap(middle_path);
					item_normal.SetRightBitmap(right_path);						
				}
				else if(paramnode->GetName() == _("HoverBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					item_hover.SetLeftBitmap(left_path);
					item_hover.SetMiddleBitmap(middle_path);
					item_hover.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("SelectedBackground"))
				{
					left_path = paramnode->GetAttribute(_("Left"), wxEmptyString);
					middle_path = paramnode->GetAttribute(_("Middle"), wxEmptyString);
					right_path = paramnode->GetAttribute(_("Right"), wxEmptyString);
					item_selected.SetLeftBitmap(left_path);
					item_selected.SetMiddleBitmap(middle_path);
					item_selected.SetRightBitmap(right_path);					
				}
				else if(paramnode->GetName() == _("ItemNormalFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetItemNormalFont(font);
				}
				else if(paramnode->GetName() == _("ItemHoverFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetItemHoverFont(font);
				}
				else if(paramnode->GetName() == _("ItemSelectedFont"))
				{
					font_encoding = wxAtoi(paramnode->GetAttribute(_("DefaultEncoding"), _("-1")));
					tmp_str = paramnode->GetAttribute(_("FaceName"), _("MS Shell Dlg 2"));
					font_family = wxAtoi(paramnode->GetAttribute(_("Family"), _("74")));
					font_size = wxAtoi(paramnode->GetAttribute(_("PointSize"), _("8")));
					font_style = wxAtoi(paramnode->GetAttribute(_("Style"), _("90")));
					font_underlined = wxAtoi(paramnode->GetAttribute(_("Underlined"), _("0")));
					font_weight = wxAtoi(paramnode->GetAttribute(_("Weight"), _("90")));
					font = wxFont(font_size, font_family, font_style, font_weight,
						(bool)!!font_underlined, tmp_str, (wxFontEncoding)font_encoding);
					SetItemSelectedFont(font);
				}
				paramnode = paramnode->GetNext();
			}
		}
		curnode = curnode->GetNext();
	}
	SetButtonBackground(button_normal, button_hover, button_selected);
	SetTabBackground(tab_normal, tab_hover, tab_selected);
	SetItemBackground(item_normal, item_hover, item_selected);
	Refresh();
	//wxSetWorkingDirectory(cwd);
	PreDrawBackgrounds();		
	SetStyle(apply_style ? m_Style : wxTB_STYLE_LIST);
	wxToolBoxEvent evt( wxEVT_TB_SKIN_LOADED, GetId());
	evt.SetEventObject(this);
	GetEventHandler()->AddPendingEvent(evt);
	return true;
}

void wxToolBox::SaveToolItemParameters(wxToolBoxItem * item, wxXmlNode * node)
{
	if(!node || !item) return;
	node->AddAttribute(_("Caption"), item->GetCaption());
	node->AddAttribute(_("Deletable"), wxString::Format(wxT("%u"), !!item->GetDeletable()));
	node->AddAttribute(_("Movable"), wxString::Format(wxT("%u"), !!item->GetMovable()));
	node->AddAttribute(_("Renamable"), wxString::Format(wxT("%u"), !!item->GetRenamable()));
	node->AddAttribute(_("Enabled"), wxString::Format(wxT("%u"), !!item->GetEnabled()));
	node->AddAttribute(_("ImageIndex"), wxString::Format(wxT("%i"), item->GetImageIndex()));
}

bool wxToolBox::SaveStructure(wxString path_to_structure)
{
	wxXmlDocument skin_xml_doc;
	wxXmlNode * root_node = new wxXmlNode(wxXML_ELEMENT_NODE, _("wxToolBoxStructure"));
	skin_xml_doc.SetRoot(root_node);
	wxXmlNode * curnode;
	wxXmlNode * itemnode;
	unsigned int i;
	wxToolBoxTab * current_tab;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Tab"));
		current_tab = node->GetData();
		SaveToolItemParameters(current_tab, curnode);		
		curnode->AddAttribute(_("Style"), wxString::Format(wxT("%i"), current_tab->GetStyle()));
		for(i = 0; i < current_tab->GetToolItems().Count(); i++)
		{
			itemnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Item"));
			SaveToolItemParameters(&(current_tab->GetToolItems()[i]), itemnode);
			curnode->AddChild(itemnode);
		}
		root_node->AddChild(curnode);
	}
	skin_xml_doc.Save(path_to_structure);
	wxToolBoxEvent evt( wxEVT_TB_STRUCTURE_SAVED, GetId());
	evt.SetEventObject(this);
	GetEventHandler()->AddPendingEvent(evt);
	return true;
}

bool wxToolBox::SaveSkin(wxString path_to_skin)
{
	wxXmlDocument skin_xml_doc;
	wxXmlNode * root_node = new wxXmlNode(wxXML_ELEMENT_NODE, _("wxToolBoxSkin"));
	skin_xml_doc.SetRoot(root_node);
	wxXmlNode * curnode;
	wxXmlNode * paramnode;

	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Style"));	
	curnode->AddAttribute(_("StyleValue"), wxString::Format(_("%u"), m_Style));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("BackgroundColour"));
	curnode->AddAttribute(_("Red"), wxString::Format(_("%u"), GetBackgroundColour().Red()));
	curnode->AddAttribute(_("Green"), wxString::Format(_("%u"), GetBackgroundColour().Green()));
	curnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), GetBackgroundColour().Blue()));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("BorderHighlightColour"));
	curnode->AddAttribute(_("Red"), wxString::Format(_("%u"), GetBorderHighlightColour().Red()));
	curnode->AddAttribute(_("Green"), wxString::Format(_("%u"), GetBorderHighlightColour().Green()));
	curnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), GetBorderHighlightColour().Blue()));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("BorderShadowColour"));
	curnode->AddAttribute(_("Red"), wxString::Format(_("%u"), GetBorderShadowColour().Red()));
	curnode->AddAttribute(_("Green"), wxString::Format(_("%u"), GetBorderShadowColour().Green()));
	curnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), GetBorderShadowColour().Blue()));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Background"));
	curnode->AddAttribute(_("Path"), m_ToolBoxBackgroundPath);	
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabIcons"));
	curnode->AddAttribute(_("Path"), m_TabImageListBitmapPath);
	curnode->AddAttribute(_("ImageWidth"), wxString::Format(_("%u"), m_TabImageWidth));
	curnode->AddAttribute(_("ImageHeight"), wxString::Format(_("%u"), m_TabImageHeight));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemIcons"));
	curnode->AddAttribute(_("Path"), m_ItemImageListBitmapPath);
	curnode->AddAttribute(_("ImageWidth"), wxString::Format(_("%u"), m_ItemImageWidth));
	curnode->AddAttribute(_("ImageHeight"), wxString::Format(_("%u"), m_ItemImageHeight));
	root_node->AddChild(curnode);
	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Flags"));
	curnode->AddAttribute(_("ItemsTransparent"), wxString::Format(_("%u"), !!m_ItemsTransparent));
	curnode->AddAttribute(_("SelectedItemTransparent"), wxString::Format(_("%u"), !!m_SelectedItemTransparent));
	curnode->AddAttribute(_("UseBackground"), wxString::Format(_("%u"), !!m_UseBackground));
	curnode->AddAttribute(_("UseStdArrows"), wxString::Format(_("%u"), !!m_UseStdArrows));
	curnode->AddAttribute(_("SelectNewTab"), wxString::Format(_("%u"), !!m_SelectNewTab));
	curnode->AddAttribute(_("PreDrawBackgrounds"), wxString::Format(_("%u"), !!m_PreDrawBackgrounds));
	curnode->AddAttribute(_("ShowSelectionAlways"), wxString::Format(_("%u"), !!m_ShowSelectionAlways));
	root_node->AddChild(curnode);

	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Buttons"));	
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ButtonHoverColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ButtonHoverColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ButtonHoverColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ButtonHoverColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ButtonNormalColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ButtonNormalColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ButtonNormalColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ButtonNormalColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ButtonSelectedColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ButtonSelectedColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ButtonSelectedColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ButtonSelectedColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Flags"));
	paramnode->AddAttribute(_("UseButtonBackground"), wxString::Format(_("%u"), !!m_UseButtonBackground));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("NormalBackground"));
	paramnode->AddAttribute(_("Left"), m_ButtonBackground.GetNormalBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ButtonBackground.GetNormalBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ButtonBackground.GetNormalBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("HoverBackground"));
	paramnode->AddAttribute(_("Left"), m_ButtonBackground.GetHoverBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ButtonBackground.GetHoverBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ButtonBackground.GetHoverBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("SelectedBackground"));
	paramnode->AddAttribute(_("Left"), m_ButtonBackground.GetSelectedBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ButtonBackground.GetSelectedBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ButtonBackground.GetSelectedBackground().GetRightPath());
	curnode->AddChild(paramnode);
	root_node->AddChild(curnode);

	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Tabs"));	
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabHoverColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_TabHoverColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_TabHoverColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_TabHoverColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabNormalColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_TabNormalColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_TabNormalColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_TabNormalColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabSelectedColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_TabSelectedColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_TabSelectedColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_TabSelectedColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabTextColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_TabTextColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_TabTextColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_TabTextColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Flags"));
	paramnode->AddAttribute(_("UseTabBackground"), wxString::Format(_("%u"), !!m_UseTabBackground));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Sizes"));
	paramnode->AddAttribute(_("TabHeight"), wxString::Format(_("%u"), m_TabHeight));
	paramnode->AddAttribute(_("TabSpacing"), wxString::Format(_("%u"), m_TabSpacing));	
	paramnode->AddAttribute(_("TabCaptionSpacing"), wxString::Format(_("%u"), m_TabCaptionSpacing));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("NormalBackground"));
	paramnode->AddAttribute(_("Left"), m_TabBackground.GetNormalBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_TabBackground.GetNormalBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_TabBackground.GetNormalBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("HoverBackground"));
	paramnode->AddAttribute(_("Left"), m_TabBackground.GetHoverBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_TabBackground.GetHoverBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_TabBackground.GetHoverBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("SelectedBackground"));
	paramnode->AddAttribute(_("Left"), m_TabBackground.GetSelectedBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_TabBackground.GetSelectedBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_TabBackground.GetSelectedBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabNormalFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_TabNormalFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_TabNormalFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_TabNormalFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_TabNormalFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_TabNormalFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_TabNormalFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_TabNormalFont.GetWeight()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabHoverFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_TabHoverFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_TabHoverFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_TabHoverFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_TabHoverFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_TabHoverFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_TabHoverFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_TabHoverFont.GetWeight()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("TabSelectedFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_TabSelectedFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_TabSelectedFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_TabSelectedFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_TabSelectedFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_TabSelectedFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_TabSelectedFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_TabSelectedFont.GetWeight()));
	curnode->AddChild(paramnode);
	root_node->AddChild(curnode);

	curnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Items"));	
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemHoverColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ItemHoverColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ItemHoverColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ItemHoverColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemNormalColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ItemNormalColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ItemNormalColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ItemNormalColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemSelectedColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ItemSelectedColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ItemSelectedColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ItemSelectedColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemTextColour"));
	paramnode->AddAttribute(_("Red"), wxString::Format(_("%u"), m_ItemTextColour.Red()));
	paramnode->AddAttribute(_("Green"), wxString::Format(_("%u"), m_ItemTextColour.Green()));
	paramnode->AddAttribute(_("Blue"), wxString::Format(_("%u"), m_ItemTextColour.Blue()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Flags"));
	paramnode->AddAttribute(_("UseItemNormalBackground"), wxString::Format(_("%u"), !!m_UseItemNormalBackground));	
	paramnode->AddAttribute(_("UseItemHoverBackground"), wxString::Format(_("%u"), !!m_UseItemHoverBackground));		
	paramnode->AddAttribute(_("UseItemSelectedBackground"), wxString::Format(_("%u"), !!m_UseItemSelectedBackground));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("Sizes"));
	paramnode->AddAttribute(_("ItemHeight"), wxString::Format(_("%u"), m_ItemHeight));
	paramnode->AddAttribute(_("ItemSpacing"), wxString::Format(_("%u"), m_ItemSpacing));
	paramnode->AddAttribute(_("ItemCaptionSpacing"), wxString::Format(_("%u"), m_ItemCaptionSpacing));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("NormalBackground"));
	paramnode->AddAttribute(_("Left"), m_ItemBackground.GetNormalBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ItemBackground.GetNormalBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ItemBackground.GetNormalBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("HoverBackground"));
	paramnode->AddAttribute(_("Left"), m_ItemBackground.GetHoverBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ItemBackground.GetHoverBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ItemBackground.GetHoverBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("SelectedBackground"));
	paramnode->AddAttribute(_("Left"), m_ItemBackground.GetSelectedBackground().GetLeftPath());
	paramnode->AddAttribute(_("Middle"), m_ItemBackground.GetSelectedBackground().GetMiddlePath());
	paramnode->AddAttribute(_("Right"), m_ItemBackground.GetSelectedBackground().GetRightPath());
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemNormalFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_ItemNormalFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_ItemNormalFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_ItemNormalFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_ItemNormalFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_ItemNormalFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_ItemNormalFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_ItemNormalFont.GetWeight()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemHoverFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_ItemHoverFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_ItemHoverFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_ItemHoverFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_ItemHoverFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_ItemHoverFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_ItemHoverFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_ItemHoverFont.GetWeight()));
	curnode->AddChild(paramnode);
	paramnode = new wxXmlNode(wxXML_ELEMENT_NODE, _("ItemSelectedFont"));
	paramnode->AddAttribute(_("DefaultEncoding"), wxString::Format(_("%i"), m_ItemSelectedFont.GetDefaultEncoding()));
	paramnode->AddAttribute(_("FaceName"), m_ItemSelectedFont.GetFaceName());
	paramnode->AddAttribute(_("Family"), wxString::Format(_("%u"), m_ItemSelectedFont.GetFamily()));	
	paramnode->AddAttribute(_("PointSize"), wxString::Format(_("%u"), m_ItemSelectedFont.GetPointSize()));
	paramnode->AddAttribute(_("Style"), wxString::Format(_("%u"), m_ItemSelectedFont.GetStyle()));
	paramnode->AddAttribute(_("Underlined"), wxString::Format(_("%u"), !!m_ItemSelectedFont.GetUnderlined()));
	paramnode->AddAttribute(_("Weight"), wxString::Format(_("%u"), m_ItemSelectedFont.GetWeight()));
	curnode->AddChild(paramnode);
	root_node->AddChild(curnode);

	skin_xml_doc.Save(path_to_skin);
	wxToolBoxEvent evt( wxEVT_TB_SKIN_SAVED, GetId());
	evt.SetEventObject(this);
	GetEventHandler()->AddPendingEvent(evt);
	return true;
}

bool wxToolBox::SwapTabs(int index1, int index2)
{
	wxToolBoxTabList::Node * node1 = m_Tabs.Item(index1);
	wxToolBoxTabList::Node * node2 = m_Tabs.Item(index2);
	if(!node1 || !node2) return false;
	wxToolBoxTab * tab1 = node1->GetData();
	wxToolBoxTab * tab2 = node2->GetData();	
	if(!tab1 || !tab2) return false;
	node1->SetData(tab2);
	node2->SetData(tab1);
	DoLayout(false, true, true);
	wxToolBoxEvent evt( wxEVT_TB_TABS_SWAPPED, GetId());				
	evt.SetEventObject(this);	
	evt.SetToolObjectIndex1(index1);
	evt.SetToolObjectIndex2(index2);
	GetEventHandler()->AddPendingEvent(evt);
	return true;
}

bool wxToolBox::SwapTabs(wxToolBoxTab * tab1, wxToolBoxTab * tab2)
{
	int index1 = IndexOf(tab1);
	int index2 = IndexOf(tab2);
	if(index1 < 0 || index2 < 0) return false;
	return SwapTabs(index1, index2);
}

int wxToolBox::IndexOf(wxToolBoxTab * tab)
{
	int cnt(0);
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		if(tab == node->GetData()) return cnt;
		cnt++;
	}
	return -1;
}

bool wxToolBox::DeleteTab(int index)
{
	if(index < 0 || index >= (int)m_Tabs.GetCount()) return false;
	wxToolBoxTabList::Node * node = m_Tabs.Item(index);
	if(node)
	{
		wxToolBoxTab * tab = node->GetData();
		if(tab)
		{
			if(tab->GetDeletable())
			{				
				m_SelectedTab = NULL;
				m_Tabs.DeleteNode(node);
				if(m_Tabs.GetCount())
				{
					node = m_Tabs.Item(index+(index ? -1 : 0));
					if(node) m_SelectedTab = node->GetData();
					if(m_SelectedTab) 
					{
						m_SelectedTab->SetSelected(true);
					}
				}			
				DoLayout(false, true, false);
				RefreshTabs();
				Refresh();
				wxToolBoxEvent evt( wxEVT_TB_TAB_DELETED, GetId());				
				evt.SetEventObject(this);	
				evt.SetToolObjectIndex(index);
				GetEventHandler()->AddPendingEvent(evt);
				return true;
			}
		}
	}
	return false;
}

void wxToolBox::RenameItem(wxToolBoxItem * item)
{	
	if(!item) return;
	bool bIsTab(false);
	wxToolBoxTab * tab(NULL);
	int dx = GetItemImageWidth()+3;
	wxPoint pt;
	wxSize sz;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		if((wxToolBoxItem *)node->GetData() == item)
		{
			bIsTab = true;
		}
	}
	if(!bIsTab)
	{
		bIsTab = false;	
		tab = item->GetParentTab();
		if(!tab) return;
		if(tab != m_SelectedTab) 
		{
			tab->SetSelected(true);			
			m_MouseMoveFreezed = false;
			m_LayoutTimer->Stop();
			RefreshTabs();
			DoLayout(false, true, false);			
		}
		tab->ScrollTo(tab->IndexOfItem(item));
		pt = wxPoint(item->GetRectangle().GetLeft()+dx, item->GetRectangle().GetTop());
		sz = wxSize(item->GetRectangle().GetWidth()-dx, item->GetRectangle().GetHeight());
		pt.y += tab->GetItemArea().GetTop();
	}
	else
	{
		pt = wxPoint(item->GetRectangle().GetLeft()+dx, item->GetRectangle().GetTop());
		sz = wxSize(item->GetRectangle().GetWidth()-dx, item->GetRectangle().GetHeight());		
	}
	m_TextBox->Move(pt);
	m_TextBox->SetSize(sz);
	m_TextBox->SetValue(item->GetCaption());
	m_TextBox->SetItem(item);
	Refresh();
	m_TextBox->Show();
	m_TextBox->SetFocus();
}

void wxToolBox::PreDrawBackgrounds()
{
	m_TabNormalBG = wxBitmap(GetSize().GetWidth(), m_TabHeight);
	m_TabHoverBG = wxBitmap(GetSize().GetWidth(), m_TabHeight);
	m_TabSelectedBG = wxBitmap(GetSize().GetWidth(), m_TabHeight);
	m_TabNormalBGSel = wxBitmap(wxMax(0, (int)(GetSize().GetWidth()-m_TabHeight)), m_TabHeight);
	m_TabHoverBGSel = wxBitmap(wxMax(0, (int)(GetSize().GetWidth()-m_TabHeight)), m_TabHeight);
	m_TabSelectedBGSel = wxBitmap(wxMax(0, (int)(GetSize().GetWidth()-m_TabHeight)), m_TabHeight);
	wxBitmap * left_bmp, * middle_bmp, *right_bmp;
	wxMemoryDC dc;
	wxBitmap tmp(GetSize().GetWidth(), m_TabHeight);
	wxLogDebug(wxT("w = %i; h = %i"), tmp.GetWidth(), tmp.GetHeight());
	dc.SelectObject(m_TabNormalBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetNormalBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetNormalBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetNormalBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
	tmp.SetMask(new wxMask(m_TabNormalBG, wxColour(255, 0, 255)));
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabNormalBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabNormalBG = tmp;

	dc.SelectObject(m_TabHoverBG);	
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetHoverBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetHoverBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetHoverBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_TabHoverBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabHoverBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabHoverBG = tmp;

	dc.SelectObject(m_TabSelectedBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetSelectedBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetSelectedBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetSelectedBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_TabSelectedBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabSelectedBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabSelectedBG = tmp;

	tmp = wxBitmap(GetSize().GetWidth()-m_TabHeight, m_TabHeight);

	dc.SelectObject(m_TabNormalBGSel);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetNormalBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetNormalBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetNormalBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_TabNormalBGSel, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabNormalBGSel, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabNormalBGSel = tmp;

	dc.SelectObject(m_TabHoverBGSel);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetHoverBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetHoverBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetHoverBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_TabHoverBGSel, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabHoverBGSel, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabHoverBGSel = tmp;

	dc.SelectObject(m_TabSelectedBGSel);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetTabBackground().GetSelectedBackground().GetLeftBitmap();
	middle_bmp = &GetTabBackground().GetSelectedBackground().GetMiddleBitmap();
	right_bmp = &GetTabBackground().GetSelectedBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_TabSelectedBGSel, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_TabSelectedBGSel, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_TabSelectedBGSel = tmp;

	PreDrawItemBackground();

	/*tmp = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);

	m_ItemNormalBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
	m_ItemHoverBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
	m_ItemSelectedBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);	
	dc.SelectObject(m_ItemNormalBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetNormalBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetNormalBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetNormalBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_ItemNormalBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemNormalBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemNormalBG = tmp;

	dc.SelectObject(m_ItemHoverBG);	
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetHoverBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetHoverBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetHoverBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_ItemHoverBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemHoverBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemHoverBG = tmp;

	dc.SelectObject(m_ItemSelectedBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetSelectedBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetSelectedBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetSelectedBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_ItemSelectedBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth(), m_TabHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemSelectedBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemSelectedBG = tmp;*/
}

void wxToolBox::PreDrawItemBackground()
{	
	wxBitmap * left_bmp, * middle_bmp, *right_bmp;
	wxMemoryDC dc;
	wxBitmap tmp(GetSize().GetWidth()-2, m_ItemHeight);
		
	m_ItemNormalBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
	m_ItemHoverBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
	m_ItemSelectedBG = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);	
	dc.SelectObject(m_ItemNormalBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetNormalBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetNormalBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetNormalBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
	tmp.SetMask(new wxMask(m_ItemNormalBG, wxColour(255, 0, 255)));
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemNormalBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemNormalBG = tmp;

	dc.SelectObject(m_ItemHoverBG);	
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetHoverBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetHoverBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetHoverBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_ItemHoverBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemHoverBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemHoverBG = tmp;

	dc.SelectObject(m_ItemSelectedBG);
	dc.SetBackground(wxBrush(wxColour(255, 0, 255), wxSOLID));
	dc.Clear();
	left_bmp = &GetItemBackground().GetSelectedBackground().GetLeftBitmap();
	middle_bmp = &GetItemBackground().GetSelectedBackground().GetMiddleBitmap();
	right_bmp = &GetItemBackground().GetSelectedBackground().GetRightBitmap();	
	if(dc.GetSize().GetWidth() > 0 && dc.GetSize().GetHeight() > 0)
	{
		DrawBG(dc, left_bmp, middle_bmp, right_bmp, 0, 0);
	}

	dc.SelectObject(wxNullBitmap);	
#if !defined(__WXWINCE__)
	tmp.SetMask(new wxMask(m_ItemSelectedBG, wxColour(255, 0, 255)));
#else
	tmp = wxBitmap(GetSize().GetWidth()-2, m_ItemHeight);
#endif
	dc.SelectObject(tmp);
	dc.DrawBitmap(m_ItemSelectedBG, 0, 0);
	dc.SelectObject(wxNullBitmap);	
	m_ItemSelectedBG = tmp;
}

void wxToolBox::DrawBG(wxDC & dc, wxBitmap * left_bmp, wxBitmap * middle_bmp, wxBitmap * right_bmp,
		int left, int top)
{
	int lw, rw, i;
	lw = left_bmp->GetWidth();
	rw = right_bmp->GetWidth();
	if(left_bmp->Ok()) dc.DrawBitmap(*left_bmp, left, top, true);
	if(middle_bmp->Ok())
	{		
		for(i = left+lw; i <= dc.GetSize().GetWidth()-rw; i+= middle_bmp->GetWidth())
		{
			dc.DrawBitmap(*middle_bmp, i, top, true);
		}	
	}
	if(right_bmp->Ok()) dc.DrawBitmap(*right_bmp, dc.GetSize().GetWidth()-rw+1, top, true);
}
void wxToolBox::SetPreDrawBackgrounds(bool value) 
{
	m_PreDrawBackgrounds = value; 
	if(m_PreDrawBackgrounds) PreDrawBackgrounds();
	Refresh();
}

void wxToolBox::UpdateToolTip(wxToolObject * item)
{
	if(!item) return;
	SetToolTip(item->GetToolTip());
}

void wxToolBox::CreateTabImageList(wxString filename, int image_width, int image_height, bool clear_existing)
{
	wxBitmap bitmap(filename, wxBITMAP_TYPE_ANY);
	SetTabImageWidth(image_width);
	SetTabImageHeight(image_height);	
	CreateImageListFromBitmap(m_TabImageList, bitmap, image_width, image_height, clear_existing);
	m_TabImageListBitmapPath = filename;
}

void wxToolBox::CreateItemImageList(wxString filename, int image_width, int image_height, bool clear_existing)
{
	wxBitmap bitmap(filename, wxBITMAP_TYPE_ANY);
	SetItemImageWidth(image_width);
	SetItemImageHeight(image_height);
	CreateImageListFromBitmap(m_ItemImageList, bitmap, image_width, image_height, clear_existing);
	m_ItemImageListBitmapPath = filename;
}

void wxToolBox::SetUseItemBackground(bool value) 
{
	m_UseItemNormalBackground = value;
	m_UseItemHoverBackground = value;
	m_UseItemSelectedBackground = value;
	Refresh();
}

void wxToolBox::SetStyle(unsigned int value)
{
	m_Style = value;
	for(wxToolBoxTabList::Node * node = m_Tabs.GetFirst(); node; node = node->GetNext())
	{
		node->GetData()->SetStyle(IntToToolBoxTabStyle(value), false);
	}
	RefreshTabs();
	Refresh();
}

bool CoordInRange(int coord, int mincoord, int maxcoord)
{
	return (coord >= mincoord) && (coord <= maxcoord);
}


#ifdef WX_TOOLBOX_DLL

class nameDLLApp : public wxApp	
{								
	bool OnInit() {return true;}
};								

IMPLEMENT_APP_NO_MAIN(nameDLLApp)


BOOL APIENTRY DllMain(HINSTANCE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            return wxEntry(hModule);

        case DLL_PROCESS_DETACH:
            wxEntryCleanup();
            break;
    }
	return true;
}

#endif
