#include "stdwx.h"
#include "wx/wxToolBox/wxToolBox.h"
#include <wx/listimpl.cpp>
#include <algorithm>

using namespace std;

WX_DEFINE_USER_EXPORTED_LIST(wxToolBoxTabList);

wxToolBoxTabStyle IntToToolBoxTabStyle(int value)
{
	switch(value)
	{
	case wxTB_STYLE_ICONS:
		return wxTB_STYLE_ICONS;
	case wxTB_STYLE_THUMBNAILS:
		return wxTB_STYLE_THUMBNAILS;
	default:
		break;
	}
	return wxTB_STYLE_LIST;
}

wxToolBoxTab::wxToolBoxTab(wxString caption, wxWindowID id, int imageindex)
:wxToolBoxItem(caption, id, imageindex), m_SelItemIndex(-1),
m_Style(wxTB_STYLE_LIST), m_ChildWindow(NULL), bDebug(false)
{
}

wxToolBoxTab::~wxToolBoxTab()
{
	if(m_ChildWindow)
	{
		m_ChildWindow->Destroy();
	}
}

wxToolBoxItem * wxToolBoxTab::operator [] (int index)
{
	if(index >= 0 && index < (int)m_ToolItems.Count())
	{
		return &m_ToolItems[index];
	}
	return NULL;
}

void wxToolBoxTab::SetEnabled(bool enabled)
{
	if(enabled != m_Enabled)
	{
		m_Enabled = enabled;
		if(m_Parent)
		{
			m_Parent->RefreshRect(m_Rectangle);
			m_Parent->RefreshRect(m_ItemArea);
		}
	}
}

void wxToolBoxTab::SetSelected(bool selected)
{
	if(selected != m_Selected)
	{
		m_Selected = selected;
		if(m_Parent)
		{
			if(m_Parent->GetSelectedTab() != this)
			{
				m_Parent->OnTabSelectionChanged(this, NULL);
				Invalidate();
				wxToolBoxEvent evt( wxEVT_TB_TAB_SELECTED, m_Parent->GetId());
				evt.SetEventObject(m_Parent);
				evt.SetTab(this);
				m_Parent->GetEventHandler()->AddPendingEvent(evt);
			}
		}
	}
}

void wxToolBoxTab::OnMouseDown(wxMouseEvent & event)
{
	//wxMessageBox(wxString::Format(_("'%s' OnMouseDown"), m_Caption));
	wxPoint mousePt(event.GetPosition());
	if(HitTest(mousePt))
	{
		if(event.LeftDown())
		{
			m_MouseDown = true;
			if(m_Parent)
			{
				m_Parent->LockMouseMove();
				Invalidate();
			}
			m_Parent->OnTabMouseDown(this, event);
			/*wxMessageBox(wxString::Format(_("Selected '%s' - [%i,%i,%i,%i] - [%i,%i,%i,%i]"), m_Caption,
				m_Rectangle.GetLeft(), m_Rectangle.GetTop(),
				m_Rectangle.GetRight(), m_Rectangle.GetBottom(),
				m_ItemArea.GetLeft(), m_ItemArea.GetTop(),
				m_ItemArea.GetRight(), m_ItemArea.GetBottom()));*/
		}
	}
	else
	{
		if(!m_Selected) return;
		//if(m_HotItemIndex != -1)
		//{
			m_HotItemIndex = HitTestItem(mousePt);
		//}
			if(m_HotItemIndex != -1 && (m_HotItemIndex != m_SelItemIndex))
		{
			m_OldItemIndex = m_SelItemIndex;
			wxToolBoxItem * item = (*this)[m_OldItemIndex];
			if(item)
			{
				if((m_OldItemIndex != -1) && item->GetSelected())
				{
					item->SetMouseDown(false);
					item->SetSelected(false);
					item->Invalidate();
				}
			}
			m_SelItemIndex = m_HotItemIndex;
			item = (*this)[m_SelItemIndex];
			if(item)
			{
				if(!item->GetMouseDown())
				{
					item->SetMouseDown(true);
					item->SetSelected(true);
					item->SetMouseDownLocation(mousePt);
				}
				item->Invalidate();
			}
			if(m_Parent) m_Parent->Refresh();
		}
	}
}

void wxToolBoxTab::OnMouseUp(wxMouseEvent & event)
{
	bool bHit = HitTest(event.GetPosition());
	if(m_MouseDown || bHit)
	{
		if(m_MouseDown)
		{
			m_MouseDown = false;
			if(event.LeftDown() && bHit)
			{
				if(m_Parent) m_Parent->OnTabSelectionChanged(this, NULL);
			}
			if(m_Parent) m_Parent->UnlockMouseMove();
			Invalidate();
		}
		if(bHit)
		{
			if(m_Parent) m_Parent->OnTabMouseUp(this, event);
		}
	}
	else if(m_Selected && (m_SelItemIndex != -1))
	{
		wxToolBoxItem * item = (*this)[m_SelItemIndex];
		if(item)
		{
			if(item->GetMouseDown())
			{
				item->SetMouseDown(false);
				EnsureItemVisible(m_SelItemIndex);
				if(m_OldItemIndex != m_SelItemIndex)
				{
					if(m_Parent) m_Parent->OnItemSelectionChanged(item, this);
				}
			}
		}
		m_OldItemIndex = m_SelItemIndex;
		if(m_HotItemIndex != -1)
		{
			if(m_Parent) m_Parent->OnItemMouseUp((*this)[m_HotItemIndex], this, event);
		}
	}
}

void wxToolBoxTab::OnMouseMove(wxMouseEvent & event)
{
	int hover = m_HotItemIndex;
	bool mh = m_MouseHover;
	if(HitTest(event.GetPosition()))
	{
		m_Parent->UpdateToolTip(this);
		CancelHotItemHover();
		m_MouseHover = true;
		if(m_MouseDown)
		{
			if(m_AllowDrag)
			{
				/*
				DragDropEffects effect = DragDropEffects.None;

				_parent.PatBltOnItem(this);
				_parent.DoDragDrop(this,DragDropEffects.All);
				_mouseDown = false;
				Invalidate();

				if(null != _parent.DragDropFinished)
				{
				_parent.DragDropFinished(this,effect);
				}
				*/
			}
		}
		if(mh != m_MouseHover)
		{
			Invalidate();
		}
	}
	else
	{
		m_MouseHover = false;
		if(m_Selected)
		{
			CheckMouseMoveForItems(event);
		}
		if((m_HotItemIndex != hover) || (mh != m_MouseHover))
		{
			Invalidate();
		}
	}
}

int wxToolBoxTab::HitTestItem(wxPoint pt)
{
	int i, index(-1);
	wxToolBoxItem * item(NULL);
	//if(m_Selected) wxMessageBox("Searching for selected items");
	for(i = m_VisibleTopIndex; i < m_VisibleBottomIndex; i++)
	{
		item = (*this)[i];
		if(item)
		{
			if(item->HitTest(pt))
			{
				index = i;
				break;
			}
		}
	}
	return index;
}

bool wxToolBoxTab::EnsureItemVisible(int index)
{
	bool bScrolled(false), bLoop(true), bUpState(true), bDnState(true);
	wxScrollDirection dir = (wxScrollDirection)Up;
	wxToolBox * parent = (wxToolBox *)m_Parent;
	if(!parent) return false;
	bUpState = parent->GetUpScroll()->GetEnabled();
	bDnState = parent->GetDownScroll()->GetEnabled();
	bLoop = ItemVisible(index);
	wxToolBoxItem * item = (*this)[index];
	if(!item) return false;
	if(bLoop)
	{
		if(item->GetRectangle().GetBottom()+m_ItemArea.GetTop() > m_ItemArea.GetBottom())
		{
			dir = (wxScrollDirection)6;
		}
	}
	while(bLoop)
	{
		if(dir == (wxScrollDirection)5)
		{
			if(item->GetRectangle().GetTop()+m_ItemArea.GetTop() < m_ItemArea.GetTop()+(int)parent->GetItemSpacing())
			{
				bLoop = ScrollItems(dir);
			}
			else
			{
				bLoop = false;
			}
		}
		else if(dir == (wxScrollDirection)6)
		{
			if(item->GetRectangle().GetBottom()+m_ItemArea.GetTop() > m_ItemArea.GetBottom())
			{
				bLoop = ScrollItems(dir);
			}
			else
			{
				bLoop = false;
			}
		}
		bScrolled |= bLoop;
	}
	if(bScrolled)
	{
		item->Invalidate();
	}
	parent->GetUpScroll()->SetEnabled(CanScroll((wxScrollDirection)5));
	parent->GetDownScroll()->SetEnabled(CanScroll((wxScrollDirection)6));
	return true;
}

void wxToolBoxTab::CancelHotItemHover()
{
	if(m_HotItemIndex != -1)
	{
		wxToolBoxItem * item = (*this)[m_HotItemIndex];
		if(item) item->CancelHover();
	}
	m_HotItemIndex = -1;
}

void wxToolBoxTab::CheckMouseMoveForItems(wxMouseEvent & event)
{
	bool bFound(false);
	int index(-1);
	wxToolBoxItem * item(NULL);
	if(m_ItemArea.Contains(event.GetPosition()))
	{
		index = HitTestItem(event.GetPosition());
		bFound = (index != -1);
		if(bFound)
		{
			if(m_HotItemIndex != index)
			{
				CancelHotItemHover();
				m_HotItemIndex = index;
				item = (*this)[m_HotItemIndex];
				if(item)
				{
					item->SetMouseHover(true);
					m_Parent->Refresh();
					//item->Invalidate();
					if(m_Parent) m_Parent->UpdateToolTip(item);
				}
			}
		}
		else
		{
			CancelHotItemHover();
			if(m_Parent) m_Parent->UpdateToolTip(NULL);
		}
	}
	else
	{
		CancelHotItemHover();
		if(HitTest(event.GetPosition()))
		{
			if(m_Parent) m_Parent->UpdateToolTip(this);
		}
	}
}

void wxToolBoxTab::PaintItems(wxDC & dc, const wxRect & clipRect)
{
	int iLoop(0), offset(0);
	wxRect rect;
	int imgX, imgY, imgindex, txtW, txtH;
	wxToolBoxItem * item(NULL);
	if((m_ItemArea.GetHeight() >= 0) && m_ToolItems.Count())
	{
		for(iLoop = m_VisibleTopIndex; iLoop < m_VisibleBottomIndex; iLoop++)
		{
			item = (*this)[iLoop];
			if(!item) continue;
			/*wxLogTrace(wxTraceMask(), _("Item pos = %i,%i"),
				item->GetRectangle().GetLeft(),
				item->GetRectangle().GetTop());*/
			rect = item->GetRectangle();
			rect.y += m_ItemArea.y;
			rect.x += m_ItemArea.x;
			if(rect.GetTop() <= m_ItemArea.GetTop()) continue;
			if(!rect.Intersects(clipRect)) continue;
			dc.SetPen(*wxTRANSPARENT_PEN);
			if(item->GetMouseDown() || item->GetSelected())
			{
				dc.SetFont(m_Parent->GetItemSelectedFont());
				if(m_Parent->GetUseItemSelectedBackground())
				{
					if(m_Parent->GetItemSelectedBGPre().Ok())
					{
						dc.DrawBitmap(m_Parent->GetItemSelectedBGPre(), rect.GetLeft(), rect.GetTop(), true);
					}
				}
				else
				{
					if(!m_Parent->GetSelectedItemTransparent())
					{
						dc.SetBrush(wxBrush(m_Parent->GetItemSelectedColour()));
						dc.DrawRectangle(rect);
					}
					m_Parent->DrawBorders(dc, rect, true);
				}
			}
			else if(item->GetMouseHover())
			{
				dc.SetFont(m_Parent->GetItemHoverFont());
				if(m_Parent->GetUseItemHoverBackground())
				{
					if(m_Parent->GetItemHoverBGPre().Ok())
					{
						dc.DrawBitmap(m_Parent->GetItemHoverBGPre(), rect.GetLeft(), rect.GetTop(), true);
					}
				}
				else
				{
					if(!m_Parent->GetItemsTransparent())
					{
						dc.SetBrush(wxBrush(m_Parent->GetItemHoverColour()));
						dc.DrawRectangle(rect);
					}
					m_Parent->DrawBorders(dc, rect, false);
				}
			}
			else
			{
				dc.SetFont(m_Parent->GetItemNormalFont());
				if(!m_Parent->GetItemsTransparent())
				{
					if(m_Parent->GetPreDrawBackgrounds())
					{
						if(m_Parent->GetItemNormalBGPre().Ok())
						{
							dc.DrawBitmap(m_Parent->GetItemNormalBGPre(), rect.GetLeft(), rect.GetTop(), true);
						}
					}
					else
					{
						dc.SetBrush(wxBrush(m_Parent->GetItemNormalColour()));
						dc.DrawRectangle(rect);
					}
				}
			}
			if(item->GetMouseDown()) rect.Offset(1,1);
			imgindex = item->GetImageIndex();
			rect.x += (2+m_Parent->GetItemCaptionSpacing());
			dc.GetTextExtent(item->GetCaption(), &txtW, &txtH);
			if(imgindex >= 0 && imgindex < (int)m_Parent->GetItemImageList().Count())
			{
				switch(GetStyle())
				{
				case wxTB_STYLE_LIST:
					offset = (rect.height-m_Parent->GetItemImageHeight())/2;
					imgX = rect.GetLeft();
					imgY = rect.GetTop()+offset;
					break;
				case wxTB_STYLE_THUMBNAILS:
					imgX = rect.GetX()+(rect.GetWidth()-m_Parent->GetItemImageWidth())/2;
					imgY = rect.GetY()+wxMax((int)0, (int)(rect.GetHeight()-m_Parent->GetItemImageHeight()-txtH)/2);
					break;
				case wxTB_STYLE_ICONS:
					imgX = rect.GetX()+(rect.GetWidth()-m_Parent->GetItemImageWidth())/2;
					imgY = rect.GetY()+(rect.GetHeight()-m_Parent->GetItemImageHeight())/2;
					break;
				}
				dc.DrawBitmap(m_Parent->GetItemImageList()[imgindex],imgX, imgY, true);
				rect.x++;
			}
			dc.SetTextForeground(m_Parent->GetItemTextColour());
			switch(GetStyle())
			{
			case wxTB_STYLE_LIST:
				dc.DrawText(item->GetCaption(),
					rect.GetX()+m_Parent->GetItemImageWidth()+2,
					rect.GetY()+(wxMax((int)0, (int)(m_Parent->GetItemHeight()- m_Parent->GetItemImageHeight())))/2);
				break;
			case wxTB_STYLE_THUMBNAILS:
				dc.DrawText(item->GetCaption(),
					rect.GetX()+(rect.GetWidth()-txtW)/2,
					rect.GetY()+wxMax((int)0, (int)(rect.GetHeight()+m_Parent->GetItemImageHeight()-txtH)/2));
				break;
			}
			if(rect.GetBottom() >= m_ItemArea.GetBottom()) break;
		}
	}
}

void wxToolBoxTab::OnPaint(wxDC & dc, wxRect r)
{
	wxRect clipRect = r;
	if(m_Rectangle.GetY() >= m_Parent->GetClientSize().GetHeight())
	{
		return;
	}
	int lw, rw, i;
	wxBitmap * left_bmp(NULL), * middle_bmp(NULL), *right_bmp(NULL), * bg_bmp(NULL);
	if(m_Rectangle.Intersects(r))
	{
		r = m_Rectangle;
		if(m_MouseDown)
		{
			dc.SetFont(m_Parent->GetTabSelectedFont());
			if(m_Parent->GetUseTabBackground())
			{
				if(m_Parent->GetPreDrawBackgrounds())
				{
					if(CoordInRange(m_Rectangle.GetTop(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom())||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetTop(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()))
					{
						bg_bmp = &m_Parent->GetTabSelectedBGSelPre();
					} else bg_bmp = &m_Parent->GetTabSelectedBGPre();
					dc.DrawBitmap(*bg_bmp, r.GetLeft(), r.GetTop(), true);
				}
				else
				{
					left_bmp = &m_Parent->GetTabBackground().GetSelectedBackground().GetLeftBitmap();
					middle_bmp = &m_Parent->GetTabBackground().GetSelectedBackground().GetMiddleBitmap();
					right_bmp = &m_Parent->GetTabBackground().GetSelectedBackground().GetRightBitmap();
				}
			}
			else
			{
				dc.SetBrush(wxBrush(m_Parent->GetTabSelectedColour(), wxSOLID));
			}
		}
		else if(m_MouseHover)
		{
			dc.SetFont(m_Parent->GetTabHoverFont());
			if(m_Parent->GetUseTabBackground())
			{
				if(m_Parent->GetPreDrawBackgrounds())
				{
					if(CoordInRange(m_Rectangle.GetTop(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom())||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetTop(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()))
					{
						bg_bmp = &m_Parent->GetTabHoverBGSelPre();
					} else bg_bmp = &m_Parent->GetTabHoverBGPre();
					dc.DrawBitmap(*bg_bmp, r.GetLeft(), r.GetTop(), true);
				}
				else
				{
					left_bmp = &m_Parent->GetTabBackground().GetHoverBackground().GetLeftBitmap();
					middle_bmp = &m_Parent->GetTabBackground().GetHoverBackground().GetMiddleBitmap();
					right_bmp = &m_Parent->GetTabBackground().GetHoverBackground().GetRightBitmap();
				}
			}
			else
			{
				dc.SetBrush(wxBrush(m_Parent->GetTabHoverColour(), wxSOLID));
			}
		}
		else
		{
			dc.SetFont(m_Parent->GetTabNormalFont());
			if(m_Parent->GetUseTabBackground())
			{
				if(m_Parent->GetPreDrawBackgrounds())
				{
					if(CoordInRange(m_Rectangle.GetTop(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom())||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetUpScroll()->GetRectangle().GetTop(),m_Parent->GetUpScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetTop(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()) ||
						CoordInRange(m_Rectangle.GetBottom(), m_Parent->GetDownScroll()->GetRectangle().GetTop(),m_Parent->GetDownScroll()->GetRectangle().GetBottom()))
					{
						bg_bmp = &m_Parent->GetTabNormalBGSelPre();
					} else bg_bmp = &m_Parent->GetTabNormalBGPre();
					dc.DrawBitmap(*bg_bmp, r.GetLeft(), r.GetTop(), true);
				}
				else
				{
					left_bmp = &m_Parent->GetTabBackground().GetNormalBackground().GetLeftBitmap();
					middle_bmp = &m_Parent->GetTabBackground().GetNormalBackground().GetMiddleBitmap();
					right_bmp = &m_Parent->GetTabBackground().GetNormalBackground().GetRightBitmap();
				}
			}
			else
			{
				dc.SetBrush(wxBrush(m_Parent->GetTabNormalColour(), wxSOLID));
			}
		}
		if(m_Parent->GetUseTabBackground())
		{
			if(!m_Parent->GetPreDrawBackgrounds())
			{
				lw = left_bmp->GetWidth();
				rw = right_bmp->GetWidth();
				if(left_bmp->Ok()) dc.DrawBitmap(*left_bmp, r.GetLeft(), r.GetTop(), true);
				if(middle_bmp->Ok())
				{
					dc.SetClippingRegion(lw, r.GetTop(), r.GetRight()-rw-lw+1, r.GetHeight());
					for(i = lw; i <= r.GetRight()-rw; i+= middle_bmp->GetWidth())
					{
						dc.DrawBitmap(*middle_bmp, i, r.GetTop(), true);
					}
					dc.DestroyClippingRegion();
				}
				if(right_bmp->Ok()) dc.DrawBitmap(*right_bmp, r.GetRight()-rw+1, r.GetTop(), true);
			}
		}
		else
		{
			if(m_MouseDown) r.Offset(1,1);
			dc.DrawRectangle(r);
			m_Parent->DrawBorders(dc, r, m_MouseDown);
		}
	}
	if((GetImageIndex() >= 0)/* && (GetImageIndex() < parent->GetImageList().GetImageCount())*/)
	{
		//wxRect rImage;
		int offset(0);
		r.x += (2+m_Parent->GetTabCaptionSpacing());
		r.width -= 2;
		offset = (r.height-m_Parent->GetTabImageHeight())/2;
		/*rImage = r;
		rImage.SetWidth(m_Parent->GetImageWidth());
		rImage.SetHeight(m_Parent->GetImageHeight());
		rImage.y += offset;*/
		if(GetImageIndex() < (int)m_Parent->GetTabImageList().Count())
		{
			//if(GetEnabled())
			//{
			//	dc.DrawBitmap(m_Parent->GetImageList()[GetImageIndex()], r.GetX(), r.GetY()+offset, true);
			//}
			//else
			//{
				dc.DrawBitmap(m_Parent->GetTabImageList()[GetImageIndex()], r.GetX(), r.GetY()+offset, true);
			//}
		}
		r.x += m_Parent->GetTabImageWidth()+1;
		r.width -= m_Parent->GetTabImageWidth()+1;
	}
	r.x++;
	r.width--;
	int dx, dy, dist;
	dc.GetTextExtent(m_Caption, &dx, &dy);
	dist = wxMax((int)((m_Parent->GetTabHeight()-dy)/2), (int)0);
	dc.SetTextForeground(m_Parent->GetTabTextColour());
	dc.DrawText(m_Caption, r.GetX()+3, r.GetY()+dist);
	if(m_Selected && (m_ChildWindow == NULL)) PaintItems(dc, clipRect);
}

bool wxToolBoxTab::ItemVisible(unsigned int index)
{
	wxToolBoxItem * item = (*this)[index];
	if(!item) return false;
	wxRect rcTemp = item->GetRectangle();
	rcTemp.x = m_ItemArea.x;
	rcTemp.y += m_ItemArea.y;
	return m_ItemArea.Contains(rcTemp.GetTopLeft()) && m_ItemArea.Contains(rcTemp.GetBottomRight());
}

bool wxToolBoxTab::ItemVisible(wxToolBoxItem * item)
{
	if(!item) return false;
	wxRect rcTemp = item->GetRectangle();
	rcTemp.x = m_ItemArea.x;
	rcTemp.y += m_ItemArea.y;
	return m_ItemArea.Contains(rcTemp.GetTopLeft()) && m_ItemArea.Contains(rcTemp.GetBottomRight());
}

bool wxToolBoxTab::Contains(wxToolBoxItem * item)
{
	for(unsigned int i = 0; i < m_ToolItems.Count(); i++)
	{
		if(item == &m_ToolItems[i]) return true;
	}
	return false;
}

int wxToolBoxTab::IndexOfItem(wxToolBoxItem * item)
{
	for(unsigned int i = 0; i < m_ToolItems.Count(); i++)
	{
		if(item == &m_ToolItems[i]) return (int)i;
	}
	return -1;
}

bool wxToolBoxTab::DeleteItem(int index)
{
	wxToolBox * parent = (wxToolBox *)m_Parent;
	if(!parent) return false;
	wxToolBoxItem * item = (*this)[index];
	if(!item) return false;
	bool bOK(false), bDeleted(false);
	wxRect rcItem, rcTemp;
	if(parent->GetEditingItem() != (*this)[index])
	{
		bOK = parent->EndRenameItem(true);
	}
	if(bOK) bOK = item->GetDeletable();
	if(bOK)
	{
		rcItem = item->GetRectangle();
		if(index == m_SelItemIndex)
		{
			m_SelItemIndex = m_OldItemIndex = -1;
		}
		m_ToolItems.RemoveAt((size_t)index);
		wxToolBoxEvent evt( wxEVT_TB_ITEM_DELETED, m_Parent->GetId());
		evt.SetEventObject(m_Parent);
		evt.SetTab(this);
		evt.SetToolObjectIndex(index);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
		for(int iLoop = index; iLoop < (int)m_ToolItems.Count(); iLoop++)
		{
			item = (*this)[iLoop];
			if(item)
			{
				if(item->GetSelected())
				{
					m_OldItemIndex = m_SelItemIndex;
					m_SelItemIndex = iLoop;
				}
				rcTemp = item->GetRectangle();
				item->SetRectangle(rcItem);
				rcItem = rcTemp;
			}
		}
		m_VisibleTopIndex = 0;
		m_VisibleBottomIndex = (int)m_ToolItems.Count();
		rcTemp = m_ItemArea;
		rcTemp.SetX(0);
		rcTemp.SetWidth(parent->GetSize().GetWidth());
		if(!m_ToolItems.Count())
		{
			m_OldItemIndex = m_SelItemIndex = m_VisibleTopIndex = m_VisibleBottomIndex = -1;
		}
		else if(m_Parent->GetShowSelectionAlways())
		{
			index = (index >= (int)m_ToolItems.Count()) ? index-1 : index;
			index = (-1 > index) ? (index+1) : index;
			item = (*this)[m_SelItemIndex];
			if(!item)
			{
				m_OldItemIndex = -1;
				m_SelItemIndex = index;
				item = (*this)[m_SelItemIndex];
				item->SetSelected(true);
			}
			if(!EnsureItemVisible(index))
			{
				UpdateItemLoopIndexes();
				//parent->Refresh(true);
			}
		}
		else
		{
			m_OldItemIndex = -1;
			m_SelItemIndex = -1;
		}
		bDeleted = true;
		m_Parent->Refresh();
	}
	return bDeleted;
}

void wxToolBoxTab::UpdateItemLoopIndexes(int index, bool * bUpdates)
{
	wxToolBoxItem * item = (*this)[index];
	if(!item) return;
	if(!bUpdates[0] && (item->GetRectangle().GetTop()+m_ItemArea.GetTop() >= m_ItemArea.GetTop()))
	{
		m_VisibleTopIndex = index;
		bUpdates[0] = true;
	}
	if(!bUpdates[1] && (item->GetRectangle().GetTop()+item->GetRectangle().GetHeight()+
		m_ItemArea.GetTop() >= m_ItemArea.GetBottom()))
	{
		m_VisibleBottomIndex = index;
		bUpdates[1] = true;
	}
}

void wxToolBoxTab::UpdateItemLoopIndexes()
{
	int iItem(0);
	bool bUpdates[2];
	bUpdates[0] = false;
	bUpdates[1] = false;
	if(m_ToolItems.Count())
	{
		m_VisibleTopIndex = 0;
		m_VisibleBottomIndex = (int)m_ToolItems.Count();
		for(iItem = 0; iItem < (int)m_ToolItems.Count(); iItem++)
		{
			UpdateItemLoopIndexes(iItem, bUpdates);
			if(bUpdates[0] && bUpdates[1])
			{
				iItem = (int)m_ToolItems.Count()+1;
			}
		}
	}
}

bool wxToolBoxTab::DeleteItem(wxToolBoxItem * item)
{
	return DeleteItem(IndexOfItem(item));
}

bool wxToolBoxTab::ScrollItems(wxScrollDirection scrollDir)
{
	return ScrollItems(scrollDir, true);
}

bool wxToolBoxTab::ScrollItems(wxScrollDirection scrollDir, bool bRedraw)
{
	//wxToolBoxItem * item;
	if(!m_Parent) return false;
	int scrollOffset = m_Parent->GetItemHeight()+m_Parent->GetItemSpacing();
	bool bScrolled(false);
	if(CanScroll(scrollDir))
	{
		if(scrollDir == Up)
		{
			ScrollItemsO(+scrollOffset, bRedraw);
		}
		else if(scrollDir == Down)
		{
			ScrollItemsO(-scrollOffset, bRedraw);
		}
		bScrolled = true;
	}
	/*wxToolBoxItem * item;
	for(int i = 0; i < m_ToolItems.Count(); i++)
	{
		item = (*this)[i];
		if(item)
		{
			wxMessageBox(wxString::Format("%s - [%i,%i,%i,%i]", item->GetCaption(),
				item->GetRectangle().GetLeft(), item->GetRectangle().GetTop(),
				item->GetRectangle().GetRight(), item->GetRectangle().GetBottom()));
		}

	}*/
	return bScrolled;
}

void wxToolBoxTab::ScrollItemsO(int offset)
{
	ScrollItemsO(offset, true);
}

void wxToolBoxTab::ScrollItemsO(int offset, bool bRedraw)
{
	int iItem(0);
	bool bUpdates[2];
	bUpdates[0] = false;
	bUpdates[1] = false;
	wxToolBoxItem * item(NULL);
	//wxMessageBox("ScrollItemsO");
	if(m_ToolItems.Count())
	{
		m_VisibleTopIndex = 0;
		m_VisibleBottomIndex = (int)m_ToolItems.Count();
		for(iItem = 0; iItem < (int)m_ToolItems.Count(); iItem++)
		{
			item = (*this)[iItem];
			if(!item) continue;
			item->GetRectangle().y += offset;
			/*wxMessageBox(wxString::Format("%s - [%i,%i,%i,%i]",
				item->GetCaption(), item->GetRectangle().GetLeft(), item->GetRectangle().GetTop(),
				item->GetRectangle().GetRight(), item->GetRectangle().GetBottom()));*/
			if(!bUpdates[0] || !bUpdates[1])
			{
				UpdateItemLoopIndexes(iItem, bUpdates);
			}
		}
		if(bRedraw)
		{
			wxRect rInv(0,m_ItemArea.GetY(),m_Parent->GetSize().GetWidth(),m_ItemArea.GetHeight());
			m_Parent->RefreshRect(rInv);
		}
	}
	//wxMessageBox(wxString::Format("VisibleTop = %i",m_VisibleTopIndex));
}

bool wxToolBoxTab::CanScroll(wxScrollDirection scrollDir)
{
	if(!m_Parent) return false;
	bool bCanScroll(true);
	wxToolBoxItem * item1(NULL), * item2(NULL);
	if(m_ToolItems.Count())
	{
		item1 = (*this)[0];
		item2 = (*this)[(unsigned int)m_ToolItems.GetCount()-1];
		if(!item1 || !item2) return false;
		bool b1(false), b2(false);
		int a, b;
		a = item1->GetRectangle().GetTop()-m_Parent->GetItemSpacing();
		/*wxMessageBox(wxString::Format("%s - %i-%i", item1->GetCaption(),
			item1->GetRectangle().GetTop(), m_Parent->GetItemSpacing()));*/
		b = m_ItemArea.GetTop();
		if(a >= b)
		{
			b1 = true;
		}
		/*wxMessageBox(wxString::Format("if(%i < %i)",
			item2->GetRectangle().GetBottom()+m_Parent->GetItemSpacing(),
			m_ItemArea.GetBottom()));*/
		a = item2->GetRectangle().GetBottom()+m_Parent->GetItemSpacing();
		b = m_ItemArea.GetBottom();
		if(a <= b)
		{
			b2 = true;
		}
		if(b1 && b2)
		{
			bCanScroll = false;
		}
		else
		{
			if(scrollDir == Up)
			{
				a = item1->GetRectangle().GetTop()-m_Parent->GetItemSpacing();
				b = 0;
				bCanScroll =  a < b;
				/*wxMessageBox(wxString::Format("MaxY = %i; TabHeight = %i; CanScrollDown = %u",
					a, b, bCanScroll));*/
			}
			else if(scrollDir == Down)
			{
				bCanScroll = item2->GetRectangle().GetBottom()+(int)m_Parent->GetItemSpacing() > m_ItemArea.GetHeight();
				/*wxMessageBox(wxString::Format("MaxY = %i; TabHeight = %i; CanScrollDown = %u",
					item2->GetRectangle().GetBottom()+m_Parent->GetItemSpacing(),
					m_ItemArea.GetBottom(),
					bCanScroll));*/
			}
		}
	}
	else
	{
		bCanScroll = false;
	}
	return bCanScroll;
}

bool wxToolBoxTab::SwapItems(unsigned int index1, unsigned int index2)
{
	bool bSwapped(false);
	wxToolBoxItem * item1 = (*this)[index1];
	wxToolBoxItem * item2 = (*this)[index2];
	if(!item1 || !item2) return false;
	wxRect rcTmp(0,0,0,0);
	if(item1->GetMovable() && item2->GetMovable())
	{
		rcTmp = item2->GetRectangle();
		item2->SetRectangle(item1->GetRectangle());
		item1->SetRectangle(rcTmp);

		item1->SetMouseHover(false);
		item2->SetMouseHover(false);
		wxToolBoxItem tmpitem = *item1;
		*item1 = *item2;
		*item2 = tmpitem;
		if(m_Parent) m_Parent->RefreshRect(m_ItemArea);
		bSwapped = true;
		wxToolBoxEvent evt( wxEVT_TB_ITEMS_SWAPPED, m_Parent->GetId());
		evt.SetEventObject(m_Parent);
		evt.SetTab(this);
		evt.SetToolObjectIndex1(index1);
		evt.SetToolObjectIndex2(index2);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
	}
	return bSwapped;
}

bool wxToolBoxTab::CanMoveItemUp(wxToolBoxItem * item)
{
	int index = IndexOfItem(item);
	if(index > 0 && index < (int)m_ToolItems.Count())
	{
		wxToolBoxItem * prev = (*this)[index-1];
		if(prev)
		{
			if(prev->GetMovable() && item->GetMovable()) return true;
		}
	}
	return false;
}

bool wxToolBoxTab::CanMoveItemDown(wxToolBoxItem * item)
{
	int index = IndexOfItem(item);
	if(index >= 0 && index < (int)m_ToolItems.Count()-1)
	{
		wxToolBoxItem * prev = (*this)[index+1];
		if(prev)
		{
			if(prev->GetMovable() && item->GetMovable()) return true;
		}
	}
	return false;
}

bool wxToolBoxTab::MoveItemUp(wxToolBoxItem * item)
{
	int index = IndexOfItem(item);
	return SwapItems(index, index-1);
}

bool wxToolBoxTab::MoveItemDown(wxToolBoxItem * item)
{
	int index = IndexOfItem(item);
	return SwapItems(index, index+1);
}

int wxToolBoxTab::AddItem(wxToolBoxItem item)
{
	int index(-1);
	if(m_Parent->EndRenameItem(true))
	{
		item.SetParentTab(this);
		item.SetParent(m_Parent);
		m_ToolItems.Add(item);
		index = (int)m_ToolItems.Count()-1;
		wxToolBoxEvent evt( wxEVT_TB_ITEM_ADDED, m_Parent->GetId());
		evt.SetEventObject(m_Parent);
		evt.SetTab(this);
		evt.SetItem(&m_ToolItems[index]);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
		if(m_ToolItems.Count() == 1 && m_Parent->GetShowSelectionAlways())
		{
			m_ToolItems[0].SetSelected(true);
			m_SelItemIndex = 0;
		}
		if(!m_Selected)
		{
			if(!m_Parent->GetSelectedTab())SetSelected(true);
			//m_Selected = true;
			m_NewItemIndex = index;
			//m_Parent->OnTabSelectionChanged(this, m_Parent->GetSelectedTab());
		}
		else
		{
			UpdateNewItem(index);
		}
		if(m_Selected)
		{
			ScrollTo(index);
			m_Parent->Refresh();
		}
	}
	/*wxToolBoxItem * item1;
	for(int i = 0; i < m_ToolItems.Count(); i++)
	{
		item1 = (*this)[i];
		if(item1)
		{
			wxMessageBox(wxString::Format("%s - [%i,%i,%i,%i]",
				item1->GetCaption(), item1->GetRectangle().GetLeft(), item1->GetRectangle().GetTop(),
				item1->GetRectangle().GetRight(), item1->GetRectangle().GetBottom()));
		}
	}*/
	return index;
}

void wxToolBoxTab::DebugItems()
{
	/*wxToolBoxItem * item1;
	for(int i = 0; i < (int)m_ToolItems.Count(); i++)
	{
		item1 = (*this)[i];
		if(item1)
		{
			wxMessageBox(wxString::Format("TAB(%s)%s - [%i,%i,%i,%i]",
				m_Caption,
				item1->GetCaption(), item1->GetRectangle().GetLeft(), item1->GetRectangle().GetTop(),
				item1->GetRectangle().GetRight(), item1->GetRectangle().GetBottom()));
		}
	}*/
}

void wxToolBoxTab::UpdateNewItem(unsigned int index)
{
	wxPoint itemPos(0,0);
	m_OldItemIndex = m_SelItemIndex;
	m_SelItemIndex = index;
	wxToolBoxItem * item;

	if(m_OldItemIndex != -1)
	{
		item = (*this)[m_OldItemIndex];
		if(item) item->SetSelected(false);
		item->Invalidate();
	}
	if(m_ToolItems.Count())
	{
		if(m_SelItemIndex > 0)
		{
			item = (*this)[m_SelItemIndex-1];
			if(item)
			{
				itemPos = wxPoint(item->GetRectangle().GetX(), item->GetRectangle().GetY());
				itemPos.y += m_Parent->GetItemHeight()+m_Parent->GetItemSpacing();
			}
		}
		else
		{
			item = (*this)[(int)m_ToolItems.Count()-1];
			if(item)
			{
				itemPos = wxPoint(GetRectangle().GetTopLeft());
				itemPos.y += m_Parent->GetItemHeight()+m_Parent->GetItemSpacing();
			}
		}
	}
	else
	{
		itemPos.x = m_ItemArea.x;
		itemPos.y = m_Parent->GetItemSpacing();
	}
	item = (*this)[index];
	if(item)
	{
		int maxWidth = 0;
		wxSize itemSize = GetCurrentItemSize(maxWidth);
		item->SetSelected(true);
		item->GetRectangle().SetWidth(itemSize.GetWidth());
		item->GetRectangle().SetHeight(itemSize.GetHeight());
		item->GetRectangle().SetPosition(GetItemLocation(index,
			itemSize.GetWidth(),
			itemSize.GetHeight(), maxWidth));
		wxLogTrace(wxTRACE_Messages, _("UpdateNewItem(%i) Position = %i,%i; Size = %i,%i"),
			index,
			item->GetRectangle().GetLeft(),
			item->GetRectangle().GetTop(),
			item->GetRectangle().GetWidth(),
			item->GetRectangle().GetHeight());
	}
	if(!m_ItemArea.IsEmpty())
	{
		if(EnsureItemVisible(index))
		{
			UpdateItemLoopIndexes();
		}
	}
}

wxSize wxToolBoxTab::GetCurrentItemSize(int & maxWidth)
{
	wxSize res(0,0);
	wxToolBox * parent = GetParent();
	if(parent)
	{
		maxWidth = GetParent()->GetSize().GetWidth() - 2;
		switch(m_Style)
		{
		case wxTB_STYLE_ICONS:
			res.SetWidth(parent->GetItemHeight());
			res.SetHeight(parent->GetItemHeight());
			maxWidth -= 4;
			break;
		case wxTB_STYLE_THUMBNAILS:
		case wxTB_STYLE_LIST:
		default:
			res.SetWidth(maxWidth);
			res.SetHeight(parent->GetItemHeight());
			break;
		}
	}
	return res;
}

wxPoint wxToolBoxTab::GetItemLocation(int index, int width, int height, int maxWidth)
{
	wxPoint res(0,0);
	bool bOk(true);
	if(!m_ToolItems.IsEmpty())
	{
		if(index == 0)
		{
			res.x = (m_Style == wxTB_STYLE_LIST) ? 0 : 4;
			res.y = (m_Style == wxTB_STYLE_LIST) ? 1 : 2;
		}
		else if((size_t)(index-1) < m_ToolItems.Count() && index > 0)
		{
			res = m_ToolItems[(index-1)].GetRectangle().GetTopLeft();
			switch(m_Style)
			{
			case wxTB_STYLE_ICONS:
				res.x += width;
				if(res.x+width > maxWidth)
				{
					res.x = 4;
					res.y += height;
				}
				//res.y = m_ItemArea.GetTop() + (m_ItemArea.GetWidth()/width)*height;
				break;
			case wxTB_STYLE_THUMBNAILS:
			case wxTB_STYLE_LIST:
			default:
				res.y += GetParent()->GetItemHeight();
				break;
			}
		}
		else
		{
			bOk = false;
		}
	}
	return res;
}

void wxToolBoxTab::UpdateItemRects(bool bRedraw)
{
	UpdateItemRects(true, true, bRedraw);
}

void wxToolBoxTab::UpdateItemRects(bool bUpdateXY, bool bUpdateSize, bool bRedraw)
{
	bool bUpdates[2];
	bUpdates[0] = false;
	bUpdates[1] = false;
	wxRect rect(0,0,0,0);
	wxToolBoxItem * item(NULL);
	wxSize itemSize;
	int maxWidth;
	if(bUpdateSize)
	{
		itemSize = GetCurrentItemSize(maxWidth);
	}
	if(!m_Parent) return;
	if(m_ToolItems.Count())
	{
		m_VisibleTopIndex = 0;
		m_VisibleBottomIndex = (unsigned int)m_ToolItems.Count();
		for(unsigned int iLoop = 0; iLoop < m_ToolItems.Count(); iLoop++)
		{
			if(!iLoop)
			{
				item = (*this)[iLoop];
				if(item)
				{
					rect = item->GetRectangle();
					rect.y = m_Parent->GetItemSpacing();
				}
			}
			rect.SetHeight(m_Parent->GetItemHeight());
			rect.SetWidth(m_Parent->GetSize().GetWidth()-2);
			item = (*this)[iLoop];
			if(bUpdateXY)
			{
				wxPoint pt = GetItemLocation(iLoop, itemSize.GetWidth(), itemSize.GetHeight(), maxWidth);
				if(item) item->GetRectangle().SetPosition(pt);
			}
			if(bUpdateSize)
			{
				if(item) item->GetRectangle().SetSize(itemSize);
			}
			rect.y += m_Parent->GetItemHeight() + m_Parent->GetItemSpacing();
			UpdateItemLoopIndexes(iLoop, bUpdates);
		}
		if(bRedraw) m_Parent->RefreshRect(m_ItemArea);
	}
	m_Parent->GetUpScroll()->SetEnabled(CanScroll(Up));
	m_Parent->GetDownScroll()->SetEnabled(CanScroll(Down));
}

void wxToolBoxTab::ScrollTo(unsigned int index)
{
	if(index >= m_ToolItems.Count()) return;
	m_Parent->RefreshTabs();
	//m_Parent->DoLayout(false, true, false);
	UpdateItemLoopIndexes();
	wxToolBoxItem * item = (*this)[index];
	int tab_index = m_Parent->IndexOf(this);
	int tab_count = (int)m_Parent->GetTabs().GetCount();
	int limit_y(0);
	wxToolBoxTab * next_tab(NULL);
	if(!item) return;
	if(index >= m_ToolItems.Count()) return;
	if((int)index >= m_VisibleTopIndex && (int)index <= m_VisibleBottomIndex) return;
	if(item->GetRectangle().GetTop() < m_ItemArea.GetTop()-m_Rectangle.GetTop())
	{
		while((item->GetRectangle().GetTop() < m_ItemArea.GetTop()))
		{
			ScrollItems(Up, false);
		}
	}
	else if(item->GetRectangle().GetBottom() >= m_ItemArea.GetBottom())
	{
		if((tab_index >= 0) && (tab_index < tab_count-1))
		{
			next_tab = m_Parent->GetTabs().Item(tab_index+1)->GetData();
			if(next_tab)
			{
				limit_y = next_tab->GetRectangle().GetTop();
			}
		}
		while(CanScroll(Down))
		{
			if(m_Selected)
			{
				if(next_tab)
				{
					if((int)(m_Rectangle.GetTop()+m_Parent->GetTabHeight()+item->GetRectangle().GetBottom()) < limit_y) break;
				} else if(item->GetRectangle().GetBottom() < m_ItemArea.GetBottom()-m_Rectangle.GetTop()) break;
			}
			else if(item->GetRectangle().GetBottom() < m_ItemArea.GetBottom()-m_Rectangle.GetTop()) break;
			ScrollItems(Down, false);
		}
	}
	m_Parent->Refresh();
}

void wxToolBoxTab::SetStyle(wxToolBoxTabStyle value, bool bApply)
{
	m_Style = value;
	if(bApply)
	{
		m_Parent->RefreshTabs();
		m_Parent->Refresh();
	}
}

unsigned int wxToolBoxTab::GetStyle()
{
	return m_Parent->GetStyle();
	//return m_Style;
}

wxWindow * wxToolBoxTab::GetChildWindow()
{
	return m_ChildWindow;
}

void wxToolBoxTab::SetChildWindow(wxWindow * window)
{
	if(m_ChildWindow)
	{
		m_ChildWindow->Destroy();
	}
	m_ChildWindow = window;
	if(m_ChildWindow)
	{
		m_ChildWindow->Hide();
		if(GetParent()) m_ChildWindow->Reparent(GetParent());
	}
	if(GetParent())
	{
		if(GetSelected())
		{
			ShowChildWindow();
		}
	}
}

wxWindow * wxToolBoxTab::DetachChildWindow()
{
	wxWindow * res = m_ChildWindow;
	m_ChildWindow = NULL;
	return res;
}

void wxToolBoxTab::ShowChildWindow()
{
	if(!m_ChildWindow || !GetParent()) return;
	m_ChildWindow->Move(m_ItemArea.GetLeft(), m_ItemArea.GetTop());
	m_ChildWindow->SetSize(m_ItemArea.GetWidth(), m_ItemArea.GetHeight());
	if(!m_ChildWindow->IsShown())m_ChildWindow->Show();
}

void wxToolBoxTab::HideChildWindow()
{
	if(m_ChildWindow) m_ChildWindow->Hide();
}
