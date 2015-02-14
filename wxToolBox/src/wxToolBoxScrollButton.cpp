#include "wx/wxToolBox/wxToolBox.h"

char *ArrowUp_xpm[] = {
"    10    3        2            1",
". c None",
"` c #000000",
"....``....",
"...````...",
"..``````.."
};

char *ArrowDown_xpm[] = {
"    10     4        2            1",
". c None",
"` c #000000",
"..........",
"..``````..",
"...````...",
"....``....",
};

wxBitmap GetStandardToolBoxArrow(wxScrollDirection dir)
{	
	wxBitmap std_arrow_bitmap_up(ArrowUp_xpm);
	wxBitmap std_arrow_bitmap_down(ArrowDown_xpm);
	switch(dir)
	{
	case Left:
	case Right:
		break;
	case Up:
		return std_arrow_bitmap_up;
		break;
	case Down:
		return std_arrow_bitmap_down;
		break;
	}
	return wxNullBitmap;
}

wxToolBoxScrollButton::wxToolBoxScrollButton(wxToolBox * parent, 
		wxScrollDirection dir, wxSize size)
	: m_MouseDown(false), m_MouseHover(false), m_Enabled(true),
	m_Direction(dir), m_Parent(parent)
{	
	m_Rectangle = wxRect(0,0,size.GetWidth(), size.GetHeight());
	m_ToolTip = wxT("Scroll ");
	if(m_Direction == 5)
	{
		m_ToolTip += wxT("Up");
	} else m_ToolTip += wxT("Down");
}

void wxToolBoxScrollButton::Paint(wxDC & dc, const wxRect & clipRect)
{		
	int lw, rw, i;	
	wxBitmap * left_bmp(NULL), * middle_bmp(NULL), * right_bmp(NULL);
	if(m_Parent)
	{
		if(m_MouseDown)
		{
			if(m_Parent->GetUseButtonBackground())
			{
				left_bmp = &m_Parent->GetButtonBackground().GetSelectedBackground().GetLeftBitmap();
				middle_bmp = &m_Parent->GetButtonBackground().GetSelectedBackground().GetMiddleBitmap();
				right_bmp = &m_Parent->GetButtonBackground().GetSelectedBackground().GetRightBitmap();
			}
			else
			{
				dc.SetBrush(wxBrush(m_Parent->GetButtonSelectedColour(), wxSOLID));
			}
		}
		else if (m_MouseHover)
		{
			if(m_Parent->GetUseButtonBackground())
			{
				left_bmp = &m_Parent->GetButtonBackground().GetHoverBackground().GetLeftBitmap();
				middle_bmp = &m_Parent->GetButtonBackground().GetHoverBackground().GetMiddleBitmap();
				right_bmp = &m_Parent->GetButtonBackground().GetHoverBackground().GetRightBitmap();
			}
			else
			{			
				dc.SetBrush(wxBrush(m_Parent->GetButtonHoverColour(), wxSOLID));
			}
		}
		else 
		{
			if(m_Parent->GetUseButtonBackground())
			{
				left_bmp = &m_Parent->GetButtonBackground().GetNormalBackground().GetLeftBitmap();
				middle_bmp = &m_Parent->GetButtonBackground().GetNormalBackground().GetMiddleBitmap();
				right_bmp = &m_Parent->GetButtonBackground().GetNormalBackground().GetRightBitmap();
			}
			else
			{				
				dc.SetBrush(wxBrush(m_Parent->GetButtonNormalColour(), wxSOLID));
			}
		}
	}
	if(m_Rectangle.Intersects(clipRect))
	{
		wxRect rect = m_Rectangle;
		if(m_MouseDown) rect.Offset(0,1);
		if(m_Parent->GetUseButtonBackground())
		{
			lw = left_bmp->GetWidth();
			rw = right_bmp->GetWidth();
			if(left_bmp->Ok()) dc.DrawBitmap(*left_bmp, m_Rectangle.GetLeft(), m_Rectangle.GetTop(), true);
			if(middle_bmp->Ok())
			{
				for(i = m_Rectangle.GetLeft()+lw; i <= m_Rectangle.GetRight()-rw; i+= middle_bmp->GetWidth())
				{
					dc.DrawBitmap(*middle_bmp, i, m_Rectangle.GetTop(), true);
				}
			}
			if(right_bmp->Ok()) dc.DrawBitmap(*right_bmp, m_Rectangle.GetRight()-rw+1, m_Rectangle.GetTop(), true);
			if(m_Parent->GetUseStdArrows())
			{
				wxBitmap arrow = GetStandardToolBoxArrow(m_Direction);
				dc.DrawBitmap(arrow, rect.GetLeft()+(rect.GetWidth()-arrow.GetWidth())/2, 
					rect.GetTop()+(rect.GetHeight()-arrow.GetHeight())/2, true);
			}
			else
			{
			}
		}
		else
		{			
			dc.DrawRectangle(m_Rectangle);
			m_Parent->DrawBorders(dc, m_Rectangle, m_MouseDown);						
			wxBitmap arrow = GetStandardToolBoxArrow(m_Direction);
			dc.DrawBitmap(arrow, rect.GetLeft()+(rect.GetWidth()-arrow.GetWidth())/2, 
				rect.GetTop()+(rect.GetHeight()-arrow.GetHeight())/2, true);
		}
	}	
}

bool wxToolBoxScrollButton::HitTest(wxCoord x, wxCoord y)
{
	return m_Rectangle.Contains(x,y);
}

void wxToolBoxScrollButton::Invalidate()
{
	if(m_Parent) m_Parent->RefreshRect(m_Rectangle);
}
