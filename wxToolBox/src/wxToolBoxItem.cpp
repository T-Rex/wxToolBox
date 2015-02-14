#include "stdwx.h"
#include "wx/wxToolBox/wxToolBox.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_USER_EXPORTED_OBJARRAY(wxToolBoxItemArray);

wxToolBoxItem::wxToolBoxItem()
: m_ID(wxID_ANY), m_Parent(NULL), m_Caption(wxEmptyString), m_ImageIndex(-1),
m_Selected(false), m_Enabled(true), m_MouseDown(false), m_MouseHover(false), 
m_AllowDrag(true), m_Renamable(true), m_Movable(true), m_Deletable(true),
m_ParentTab(NULL)
{
}

wxToolBoxItem::wxToolBoxItem(wxString caption, wxWindowID id, int imageindex)
: m_ID(id), m_Parent(NULL), m_Caption(caption), m_ImageIndex(imageindex), m_Selected(false), 
m_Enabled(true), m_MouseDown(false), m_MouseHover(false), m_AllowDrag(true), 
m_Renamable(true), m_Movable(true), m_Deletable(true), m_ParentTab(NULL)
{
}

wxToolBoxItem::wxToolBoxItem(const wxToolBoxItem & item)
: wxToolObject((wxToolObject &)item), m_ID(item.m_ID), m_Parent(item.m_Parent), m_Caption(item.m_Caption), 
m_ImageIndex(item.m_ImageIndex), m_Selected(item.m_Selected), m_Enabled(item.m_Enabled), 
m_MouseDown(item.m_MouseDown), m_MouseHover(item.m_MouseHover), m_AllowDrag(item.m_AllowDrag), 
m_IsDragging(item.m_IsDragging), m_Renamable(item.m_Renamable), m_Movable(item.m_Movable), 
m_Deletable(item.m_Deletable), m_ParentTab(item.m_ParentTab), m_DragSafeRect(item.m_DragSafeRect)
{
}

wxToolBoxItem wxToolBoxItem::operator = (wxToolBoxItem item)
{
	m_ID = item.m_ID;
	m_Parent = item.m_Parent;
	m_ParentTab = item.m_ParentTab;
	m_Rectangle = item.m_Rectangle;
	m_ToolTip = item.m_ToolTip;
	m_Caption = item.m_Caption;
	m_ImageIndex = item.m_ImageIndex;
	m_Selected = item.m_Selected;
	m_Enabled = item.m_Enabled;
	m_MouseDown = item.m_MouseDown;
	m_MouseHover = item.m_MouseHover;
	m_AllowDrag = item.m_AllowDrag;
	m_IsDragging = item.m_IsDragging;
	m_Renamable = item.m_Renamable;
	m_Movable = item.m_Movable;
	m_Deletable = item.m_Deletable;	
	m_DragSafeRect = item.m_DragSafeRect;
	return *this;
}

bool wxToolBoxItem::HitTest(wxPoint pt)
{
	bool bHit(false);
	wxRect rcTemp(0,0,0,0);	
	if(m_ParentTab)
	{
		rcTemp = m_Rectangle;
		rcTemp.y += m_ParentTab->GetItemArea().y;				
		bHit = rcTemp.Contains(pt);
	}
	else
	{
		bHit = m_Rectangle.Contains(pt);
	}
	return bHit;
}

void wxToolBoxItem::CancelHover()
{
	if(m_MouseHover)
	{
		m_MouseHover = false;
		Invalidate();
	}
}

void wxToolBoxItem::Rename()
{
	m_Parent->RenameItem(this);
}

bool wxToolBoxItem::CanStartDrag(wxPoint pt)
{
	return m_AllowDrag && m_DragSafeRect.Contains(pt);
}

void wxToolBoxItem::Invalidate()
{	
	if(m_Parent)
	{
		if(m_ParentTab)
		{
			wxRect rcTemp(m_Rectangle);			
			rcTemp.y += m_ParentTab->GetItemArea().y;
			rcTemp.Inflate(1,1);
			m_Parent->RefreshRect(rcTemp);
		}
		else
		{
			m_Parent->RefreshRect(m_Rectangle);
		}
	}
}

void wxToolBoxItem::SetSelected(bool selected) 
{
	if(m_Selected == selected) return;	
	if(m_Selected = selected)
	{
		wxToolBoxEvent evt( wxEVT_TB_ITEM_SELECTED, m_Parent->GetId());				
		evt.SetEventObject(GetParent());
		evt.SetTab(GetParentTab());
		evt.SetItem(this);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
	}
}
