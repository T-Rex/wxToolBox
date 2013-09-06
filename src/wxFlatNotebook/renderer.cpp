#include <wx/wxFlatNotebook/renderer.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h> // for the styles
#include <wx/wxFlatNotebook/fnb_resources.h>
#include <wx/image.h>

wxFNBRenderer::wxFNBRenderer()
: m_tabXBgBmp(16, 16)
, m_xBgBmp(16, 14)
, m_leftBgBmp(16, 14)
, m_rightBgBmp(16, 14)
{
}

wxFNBRenderer::~wxFNBRenderer()
{
}

int wxFNBRenderer::GetLeftButtonPos(wxWindow* pageContainer)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();
	wxRect rect = pc->GetClientRect();
	int clientWidth = rect.width;
	if(style & wxFNB_NO_X_BUTTON)
		return clientWidth - 38;
	else
		return clientWidth - 54;
}

int wxFNBRenderer::GetRightButtonPos(wxWindow* pageContainer)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();
	wxRect rect = pc->GetClientRect();
	int clientWidth = rect.width;
	if(style & wxFNB_NO_X_BUTTON)
		return clientWidth - 22;
	else
		return clientWidth - 38;
}

int wxFNBRenderer::GetDropArrowButtonPos(wxWindow *pageContainer)
{
	return GetRightButtonPos(pageContainer);
}

int wxFNBRenderer::GetXPos(wxWindow* pageContainer)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();
	wxRect rect = pc->GetClientRect();
	int clientWidth = rect.width;
	if(style & wxFNB_NO_X_BUTTON)
		return clientWidth;
	else
		return clientWidth - 22;
}

int wxFNBRenderer::GetButtonsAreaLength(wxWindow* pageContainer)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();

	// ''
	if(style & wxFNB_NO_NAV_BUTTONS && style & wxFNB_NO_X_BUTTON && !(style & wxFNB_DROPDOWN_TABS_LIST))
		return 0;
	
	// 'x'
	if((style & wxFNB_NO_NAV_BUTTONS) && !(style & wxFNB_NO_X_BUTTON) && !(style & wxFNB_DROPDOWN_TABS_LIST))
		return 22;
	
	// '<>'
	if(!(style & wxFNB_NO_NAV_BUTTONS) && (style & wxFNB_NO_X_BUTTON) && !(style & wxFNB_DROPDOWN_TABS_LIST))
		return 53 - 16;
	
	// 'vx'
	if((style & wxFNB_DROPDOWN_TABS_LIST) && !(style & wxFNB_NO_X_BUTTON))
		return 22 + 16;

	// 'v'
	if((style & wxFNB_DROPDOWN_TABS_LIST) && (style & wxFNB_NO_X_BUTTON))
		return 22;

	// '<>x'
	return 53;
}

void wxFNBRenderer::DrawLeftArrow(wxWindow* pageContainer, wxDC& dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();
	if(style & wxFNB_NO_NAV_BUTTONS)
		return;

	// Make sure that there are pages in the container
	if(pc->GetPageInfoVector().empty())
		return;

	// Set the bitmap according to the button status
	wxBitmap arrowBmp;
	switch(pc->m_nLeftButtonStatus)
	{
	case wxFNB_BTN_HOVER:
		arrowBmp = wxBitmap(left_arrow_hilite_xpm);
		break;
	case wxFNB_BTN_PRESSED:
		arrowBmp = wxBitmap(left_arrow_pressed_xpm);
		break;
	case wxFNB_BTN_NONE:
	default:
		arrowBmp = wxBitmap(left_arrow_xpm);
		break;
	}

	if(pc->m_nFrom == 0)
	{
		// Handle disabled arrow
		arrowBmp = wxBitmap(left_arrow_disabled_xpm);
	}

	arrowBmp.SetMask(new wxMask(arrowBmp, MASK_COLOR));

	// Erase old bitmap
	int posx = GetLeftButtonPos( pc );
	dc.DrawBitmap(m_leftBgBmp, posx, 6);

	// Draw the new bitmap
	dc.DrawBitmap(arrowBmp, posx, 6, true);
}

void wxFNBRenderer::DrawRightArrow(wxWindow* pageContainer, wxDC& dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	long style = pc->GetParent()->GetWindowStyleFlag();
	if(style & wxFNB_NO_NAV_BUTTONS)
		return;

	// Make sure that there are pages in the container
	if(pc->GetPageInfoVector().empty())
		return;

	// Set the bitmap according to the button status
	wxBitmap arrowBmp;
	switch(pc->m_nRightButtonStatus)
	{
	case wxFNB_BTN_HOVER:
		arrowBmp = wxBitmap(right_arrow_hilite_xpm);
		break;
	case wxFNB_BTN_PRESSED:
		arrowBmp = wxBitmap(right_arrow_pressed_xpm);
		break;
	case wxFNB_BTN_NONE:
	default:
		arrowBmp = wxBitmap(right_arrow_xpm);
		break;
	}


	// Check if the right most tab is visible, if it is
	// don't rotate right anymore
	if(pc->GetPageInfoVector()[pc->GetPageInfoVector().GetCount()-1].GetPosition() != wxPoint(-1, -1))
	{
		arrowBmp = wxBitmap(right_arrow_disabled_xpm);
	}

	arrowBmp.SetMask(new wxMask(arrowBmp, MASK_COLOR));

	// erase old bitmap
	int posx = GetRightButtonPos( pc ) ;
	dc.DrawBitmap(m_rightBgBmp, posx, 6);

	// Draw the new bitmap
	dc.DrawBitmap(arrowBmp, posx, 6, true);
}

void wxFNBRenderer::DrawDropDownArrow(wxWindow* pageContainer, wxDC& dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	// Check if this style is enabled
	long style = pc->GetParent()->GetWindowStyleFlag();
	if(! (style & wxFNB_DROPDOWN_TABS_LIST) )
		return;

	// Make sure that there are pages in the container
	if(pc->GetPageInfoVector().empty())
		return;

	// Set the bitmap according to the button status
	wxBitmap xbmp;
	
	switch(pc->m_nArrowDownButtonStatus)
	{
	case wxFNB_BTN_HOVER:
		xbmp = wxBitmap(down_arrow_hilite_xpm);
		break;
	case wxFNB_BTN_PRESSED:
		xbmp = wxBitmap(down_arrow_pressed_xpm);
		break;
	case wxFNB_BTN_NONE:
	default:
		xbmp = wxBitmap(down_arrow_xpm);
		break;
	}

	xbmp.SetMask(new wxMask(xbmp, MASK_COLOR));
	// erase old bitmap
	int posx = GetDropArrowButtonPos( pc );
	dc.DrawBitmap(m_xBgBmp, posx, 6);

	// Draw the new bitmap
	dc.DrawBitmap(xbmp, posx, 6, true);
}

void wxFNBRenderer::DrawX(wxWindow* pageContainer, wxDC& dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	// Check if this style is enabled
	long style = pc->GetParent()->GetWindowStyleFlag();
	if(style & wxFNB_NO_X_BUTTON)
		return;

	// Make sure that there are pages in the container
	if(pc->GetPageInfoVector().empty())
		return;

	// Set the bitmap according to the button status
	wxBitmap xbmp;
	switch(pc->m_nXButtonStatus)
	{
	case wxFNB_BTN_HOVER:
		xbmp = wxBitmap(x_button_hilite_xpm);
		break;
	case wxFNB_BTN_PRESSED:
		xbmp = wxBitmap(x_button_pressed_xpm);
		break;
	case wxFNB_BTN_NONE:
	default:
		xbmp = wxBitmap(x_button_xpm);
		break;
	}

	xbmp.SetMask(new wxMask(xbmp, MASK_COLOR));
	// erase old bitmap
	int posx = GetXPos( pc );
	dc.DrawBitmap(m_xBgBmp, posx, 6);

	// Draw the new bitmap
	dc.DrawBitmap(xbmp, posx, 6, true);
}

void wxFNBRenderer::PaintStraightGradientBox(wxDC& dc, const wxRect& rect, const wxColour& startColor, const wxColour& endColor, bool  vertical)
{
	int rd, gd, bd, high = 0;
	rd = endColor.Red() - startColor.Red();
	gd = endColor.Green() - startColor.Green();
	bd = endColor.Blue() - startColor.Blue();

	/// Save the current pen and brush
	wxPen savedPen = dc.GetPen();
	wxBrush savedBrush = dc.GetBrush();

	if ( vertical )
		high = rect.GetHeight()-1;
	else
		high = rect.GetWidth()-1;

	if( high < 1 )
		return;

	for (int i = 0; i <= high; ++i)
	{
		int r = startColor.Red() +  ((i*rd*100)/high)/100;
		int g = startColor.Green() + ((i*gd*100)/high)/100;
		int b = startColor.Blue() + ((i*bd*100)/high)/100;

		wxPen p(wxColor(r, g, b));
		dc.SetPen(p);

		if ( vertical )
			dc.DrawLine(rect.x, rect.y+i, rect.x+rect.width, rect.y+i);
		else
			dc.DrawLine(rect.x+i, rect.y, rect.x+i, rect.y+rect.height);
	}

	/// Restore the pen and brush
	dc.SetPen( savedPen );
	dc.SetBrush( savedBrush );
}

void wxFNBRenderer::DrawTabX(wxWindow* pageContainer, wxDC& dc, const wxRect& rect, const int& tabIdx, const int btnStatus)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	if(!pc->HasFlag(wxFNB_X_ON_TAB))
		return;

	/// We draw the 'x' on the active tab only
	if(tabIdx != pc->GetSelection() || tabIdx < 0)
		return;

	// Set the bitmap according to the button status
	wxBitmap xBmp;
	switch(btnStatus)
	{
	case wxFNB_BTN_HOVER:
		xBmp = wxBitmap(x_button_hilite_xpm);
		break;
	case wxFNB_BTN_PRESSED:
		xBmp = wxBitmap(x_button_pressed_xpm);
		break;
	case wxFNB_BTN_NONE:
	default:
		xBmp = wxBitmap(x_button_xpm);
		break;
	}

	/// Set the masking
	xBmp.SetMask(new wxMask(xBmp, MASK_COLOR));

	// erase old button
	dc.DrawBitmap(m_tabXBgBmp, rect.x, rect.y);

	// Draw the new bitmap
	dc.DrawBitmap(xBmp, rect.x, rect.y, true);

	// Update the vectpr
	wxRect rr(rect.x, rect.y, 14, 13);
	pc->GetPageInfoVector()[tabIdx].SetXRect( rr );
}

void wxFNBRenderer::GetBitmap(wxDC &dc, const wxRect &rect, wxBitmap &bmp)
{
	wxMemoryDC mem_dc;
	mem_dc.SelectObject(bmp);
	mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
	mem_dc.SelectObject(wxNullBitmap);
}

wxColor wxFNBRenderer::RandomColor()
{
	int r, g, b;
	r = rand() % 256; // Random value betweem 0-255
	g = rand() % 256; // Random value betweem 0-255
	b = rand() % 256; // Random value betweem 0-255
	return wxColor(r, g, b);
}

wxColor wxFNBRenderer::LightColour(const wxColour& color, int percent)
{
	int rd, gd, bd, high = 0;
	wxColor end_color = wxT("WHITE");
	rd = end_color.Red() - color.Red();
	gd = end_color.Green() - color.Green();
	bd = end_color.Blue() - color.Blue();
	high = 100;

	// We take the percent way of the color from color --> white
	int i = percent;
	int r = color.Red() +  ((i*rd*100)/high)/100;
	int g = color.Green() + ((i*gd*100)/high)/100;
	int b = color.Blue() + ((i*bd*100)/high)/100;
	return wxColor(r, g, b);
}

void wxFNBRenderer::DrawTabsLine(wxWindow* pageContainer, wxDC& dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	wxRect clntRect = pc->GetClientRect();
	wxRect clientRect, clientRect2, clientRect3;
	clientRect3 = wxRect(0, 0, clntRect.width, clntRect.height);

	if(pc->HasFlag(wxFNB_BOTTOM))
	{
		clientRect = wxRect(0, 2, clntRect.width, clntRect.height - 2);
		clientRect2 = wxRect(0, 1, clntRect.width, clntRect.height - 1);
	}
	else
	{
		clientRect = wxRect(0, 0, clntRect.width, clntRect.height - 2);
		clientRect2 = wxRect(0, 0, clntRect.width, clntRect.height - 1);
	}

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen( wxPen(pc->GetSingleLineBorderColor()) );
	dc.DrawRectangle(clientRect2);
	dc.DrawRectangle(clientRect3);

	dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW)));
	dc.DrawRectangle(clientRect);

	if( !pc->HasFlag(wxFNB_TABS_BORDER_SIMPLE) )
	{
		dc.SetPen(wxPen( pc->HasFlag( wxFNB_VC71) ? wxColour(247, 243, 233) : pc->m_tabAreaColor));
		dc.DrawLine(0, 0, 0, clientRect.height+1);
		if(pc->HasFlag(wxFNB_BOTTOM))
		{
			dc.DrawLine(0, clientRect.height+1, clientRect.width, clientRect.height+1);
		}
		else
			dc.DrawLine(0, 0, clientRect.width, 0);
		dc.DrawLine(clientRect.width - 1, 0, clientRect.width - 1, clientRect.height+1);
	}
}

int wxFNBRenderer::CalcTabWidth(wxWindow *pageContainer, int tabIdx, int tabHeight)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	int tabWidth, shapePoints(0), width, pom;
	wxMemoryDC dc;

	wxFont boldFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);

	if( pc->IsDefaultTabs() )
		shapePoints = (int)(tabHeight*tan((double)pc->GetPageInfoVector()[tabIdx].GetTabAngle()/180.0*M_PI));

	// Calculate the text length using the bold font, so when selecting a tab
	// its width will not change
	dc.SetFont(boldFont);
	dc.GetTextExtent(pc->GetPageText(tabIdx), &width, &pom);

	// Set a minimum size to a tab
	if(width < 20)
		width = 20;

	tabWidth = ((wxFlatNotebook *)pc->m_pParent)->GetPadding() * 2 + width;

	/// Style to add a small 'x' button on the top right
	/// of the tab
	if(pc->HasFlag(wxFNB_X_ON_TAB) && tabIdx == pc->GetSelection())
	{
		int spacer = 9;
		if( pc->HasFlag(wxFNB_VC8) )
			spacer = 4;

		tabWidth += ((wxFlatNotebook *)pc->m_pParent)->GetPadding() + spacer;
	}

	if( pc->IsDefaultTabs() )
		// Default style
		tabWidth += 2 * shapePoints;

	bool hasImage = (pc->m_ImageList != NULL && pc->GetPageInfoVector()[tabIdx].GetImageIndex() != -1);

	// For VC71 style, we only add the icon size (16 pixels)
	if(hasImage)
	{
		if( ! pc->IsDefaultTabs() )
			tabWidth += (16 + ((wxFlatNotebook*)pc->m_pParent)->GetPadding());
		else
			// Default style
			tabWidth += (16 + ((wxFlatNotebook*)pc->m_pParent)->GetPadding()) + shapePoints / 2;
	}
	return tabWidth;
}

void wxFNBRenderer::DrawDragHint(wxWindow *pageContainer, int tabIdx)
{
	wxUnusedVar( pageContainer );
	wxUnusedVar( tabIdx );
}

int wxFNBRenderer::CalcTabHeight(wxWindow *pageContainer)
{
	int tabHeight;
	wxMemoryDC dc;
	wxUnusedVar( pageContainer );
//	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

	// For GTK it seems that we must do this steps in order
	// for the tabs will get the proper height on initialization
	// on MSW, preforming these steps yields wierd results
	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
#ifdef __WXGTK__
	dc.SetFont( boldFont );
#endif

	static int height(-1);
	static int width(-1);

	if( height == -1 && width == -1 )
	{
		wxString stam = wxT("Tp");	// Temp data to get the text height;
		dc.GetTextExtent(stam, &width, &height);
	}

	tabHeight = height + wxFNB_HEIGHT_SPACER; // We use 8 pixels as padding
#ifdef __WXGTK__
	// On GTK the tabs are should be larger
	tabHeight += 6;
#endif
	return tabHeight;
}

void wxFNBRenderer::DrawTabs(wxWindow *pageContainer, wxDC &dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
#ifndef __WXMAC__
	// Works well on MSW & GTK, however this lines should be skipped on MAC
	if(pc->GetPageInfoVector().empty() || pc->m_nFrom >= (int)pc->GetPageInfoVector().GetCount())
	{
		pc->Hide();
		return;
	}
#endif

	// Get the text hight
	int tabHeight = CalcTabHeight(pageContainer);
	long style = pc->GetParent()->GetWindowStyleFlag();
	
	// Calculate the number of rows required for drawing the tabs
	wxRect rect = pc->GetClientRect();
	int clientWidth = rect.width;

	// Set the maximum client size
	pc->SetSizeHints(wxSize(GetButtonsAreaLength( pc ), tabHeight));
	wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

	wxBrush backBrush;
	if(style & wxFNB_VC71)
		backBrush = wxBrush(wxColour(247, 243, 233));
	else
		backBrush = wxBrush(pc->m_tabAreaColor);

	wxBrush noselBrush = wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	wxBrush selBrush = wxBrush(pc->m_activeTabColor);

	wxSize size = pc->GetSize();

	// Background
    dc.SetTextBackground((style & wxFNB_VC71 ? wxColour(247, 243, 233) : pc->GetBackgroundColour()));
	dc.SetTextForeground(pc->m_activeTextColor);
	dc.SetBrush(backBrush);

	// If border style is set, set the pen to be border pen
	if(pc->HasFlag(wxFNB_TABS_BORDER_SIMPLE))
		dc.SetPen(borderPen);
	else
	{
		wxColor colr = pc->HasFlag( wxFNB_VC71 ) ? wxColour(247, 243, 233) : pc->GetBackgroundColour();
		dc.SetPen( wxPen(colr) );
	}
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Take 3 bitmaps for the background for the buttons
	{
		wxMemoryDC mem_dc;
		wxRect rect;

		//---------------------------------------
		// X button
		//---------------------------------------
		rect = wxRect(GetXPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_xBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);

		//---------------------------------------
		// Right button
		//---------------------------------------
		rect = wxRect(GetRightButtonPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_rightBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);

		//---------------------------------------
		// Left button
		//---------------------------------------
		rect = wxRect(GetLeftButtonPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_leftBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);
	}

	// We always draw the bottom/upper line of the tabs
	// regradless the style
	dc.SetPen(borderPen);
	DrawTabsLine(pc, dc);

	// Restore the pen
	dc.SetPen(borderPen);

	if(pc->HasFlag( wxFNB_VC71 ))
	{
		int greyLineYVal  = pc->HasFlag( wxFNB_BOTTOM ) ? 0 : size.y - 2;
		int whiteLineYVal = pc->HasFlag( wxFNB_BOTTOM ) ? 3 : size.y - 3;

		wxPen pen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
		dc.SetPen(pen);

		// Draw thik grey line between the windows area and
		// the tab area
		int num = 0;
		for(; num<3; num++)
			dc.DrawLine(0, greyLineYVal + num, size.x, greyLineYVal + num);

		wxPen wbPen = pc->HasFlag(wxFNB_BOTTOM) ? *wxBLACK_PEN : *wxWHITE_PEN;
		dc.SetPen( wbPen );
		dc.DrawLine(1, whiteLineYVal, size.x - 1, whiteLineYVal);

		// Restore the pen
		dc.SetPen(borderPen);
	}

#ifdef __WXMAC__
	// On MAC, Add these lines so the tab background gets painted
	if(pc->GetPageInfoVector().empty() || pc->m_nFrom >= (int)pc->GetPageInfoVector().GetCount())
	{
		pc->Hide();
		return;
	}
#endif

	// Draw labels
	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
	dc.SetFont(boldFont);

	int posx = ((wxFlatNotebook *)pc->m_pParent)->GetPadding();
	int i = 0;

	// Update all the tabs from 0 to 'pc->m_nFrom' to be non visible
	for(i=0; i<pc->m_nFrom; i++)
	{
		pc->GetPageInfoVector()[i].SetPosition(wxPoint(-1, -1));
		pc->GetPageInfoVector()[i].GetRegion().Clear();
	}

	//----------------------------------------------------------
	// Go over and draw the visible tabs
	//----------------------------------------------------------
	for(i=pc->m_nFrom; i<(int)pc->GetPageInfoVector().GetCount(); i++)
	{
		dc.SetPen(borderPen);
		dc.SetBrush((i==pc->GetSelection()) ? selBrush : noselBrush);

		// Now set the font to the correct font
		dc.SetFont((i==pc->GetSelection()) ? boldFont : normalFont);

		// Add the padding to the tab width
		// Tab width:
		// +-----------------------------------------------------------+
		// | PADDING | IMG | IMG_PADDING | TEXT | PADDING | x |PADDING |
		// +-----------------------------------------------------------+
		int tabWidth = CalcTabWidth(pageContainer, i, tabHeight);

		// Check if we can draw more
		if(posx + tabWidth + GetButtonsAreaLength( pc ) >= clientWidth)
			break;

		// By default we clean the tab region
		pc->GetPageInfoVector()[i].GetRegion().Clear();

		// Clean the 'x' buttn on the tab.
		// A 'Clean' rectanlge, is a rectangle with width or height
		// with values lower than or equal to 0
		pc->GetPageInfoVector()[i].GetXRect().SetSize(wxSize(-1, -1));

		// Draw the tab (border, text, image & 'x' on tab)
		DrawTab(pc, dc, posx, i, tabWidth, tabHeight, pc->m_nTabXButtonStatus);

		// Restore the text forground
		dc.SetTextForeground(pc->m_activeTextColor);

		// Update the tab position & size
		int posy = pc->HasFlag(wxFNB_BOTTOM) ? 0 : VERTICAL_BORDER_PADDING;

		pc->GetPageInfoVector()[i].SetPosition(wxPoint(posx, posy));
		pc->GetPageInfoVector()[i].SetSize(wxSize(tabWidth, tabHeight));
		posx += tabWidth;
	}

	// Update all tabs that can not fit into the screen as non-visible
	for(; i<(int)pc->GetPageInfoVector().GetCount(); i++)
	{
		pc->GetPageInfoVector()[i].SetPosition(wxPoint(-1, -1));
		pc->GetPageInfoVector()[i].GetRegion().Clear();
	}

	// Draw the left/right/close buttons
	// Left arrow
	DrawLeftArrow(pc, dc);
	DrawRightArrow(pc, dc);
	DrawX(pc, dc);
	DrawDropDownArrow(pc, dc);
}

//------------------------------------------
// Renderer manager
//------------------------------------------
wxFNBRendererMgr::wxFNBRendererMgr()
{
	// register renderers
	m_renderers[-1] = wxFNBRendererPtr(new wxFNBRendererDefault());
	m_renderers[wxFNB_VC71] = wxFNBRendererPtr(new wxFNBRendererVC71());
	m_renderers[wxFNB_FANCY_TABS] = wxFNBRendererPtr(new wxFNBRendererFancy());
	m_renderers[wxFNB_VC8] = wxFNBRendererPtr(new wxFNBRendererVC8());
}

wxFNBRendererMgr::~wxFNBRendererMgr()
{
}

wxFNBRendererPtr wxFNBRendererMgr::GetRenderer(long style)
{
	// since we dont have a style for default tabs, we 
	// test for all others - FIXME: add style for default tabs
	if( !(style & wxFNB_VC71) && !(style & wxFNB_VC8) && !(style & wxFNB_FANCY_TABS) )
		return m_renderers[-1];

	if( style & wxFNB_VC71 )
		return m_renderers[wxFNB_VC71];

	if( style & wxFNB_FANCY_TABS )
		return m_renderers[wxFNB_FANCY_TABS];

	if( style & wxFNB_VC8 )
		return m_renderers[wxFNB_VC8];

	// the default is to return the default renderer
	return m_renderers[-1];
}

//------------------------------------------
// Default renderer 
//------------------------------------------

void wxFNBRendererDefault::DrawTab(wxWindow* pageContainer, wxDC &dc, const int &posx, const int &tabIdx, const int &tabWidth, const int &tabHeight, const int btnStatus)
{
		// Default style
	wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

	wxPoint tabPoints[7];
	tabPoints[0].x = posx;
	tabPoints[0].y = pc->HasFlag(wxFNB_BOTTOM) ? 2 : tabHeight - 2;

	tabPoints[1].x = (int)(posx+(tabHeight-2)*tan((double)(pc->GetPageInfoVector())[tabIdx].GetTabAngle()/180.0*M_PI));
	tabPoints[1].y = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - (VERTICAL_BORDER_PADDING+2) : (VERTICAL_BORDER_PADDING+2);

	tabPoints[2].x = tabPoints[1].x+2;
	tabPoints[2].y = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - VERTICAL_BORDER_PADDING : VERTICAL_BORDER_PADDING;

	tabPoints[3].x = (int)(posx+tabWidth-(tabHeight-2)*tan((double)(pc->GetPageInfoVector())[tabIdx].GetTabAngle()/180.0*M_PI))-2;
	tabPoints[3].y = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - VERTICAL_BORDER_PADDING : VERTICAL_BORDER_PADDING;

	tabPoints[4].x = tabPoints[3].x+2;
	tabPoints[4].y = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - (VERTICAL_BORDER_PADDING+2) : (VERTICAL_BORDER_PADDING+2);

	tabPoints[5].x = (int)(tabPoints[4].x+(tabHeight-2)*tan((double)(pc->GetPageInfoVector())[tabIdx].GetTabAngle()/180.0*M_PI));
	tabPoints[5].y = pc->HasFlag(wxFNB_BOTTOM) ? 2 : tabHeight - 2;

	tabPoints[6].x = tabPoints[0].x;
	tabPoints[6].y = tabPoints[0].y;

	if(tabIdx == pc->GetSelection())
	{
		// Draw the tab as rounded rectangle
		dc.DrawPolygon(7, tabPoints);
	}
	else
	{
		if(tabIdx != pc->GetSelection() - 1)
		{
			// Draw a vertical line to the right of the text
			int pt1x, pt1y, pt2x, pt2y;
			pt1x = tabPoints[5].x;
			pt1y = pc->HasFlag(wxFNB_BOTTOM) ? 4 : tabHeight - 6;
			pt2x = tabPoints[5].x;
			pt2y = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - 4 : 4 ;
			dc.DrawLine(pt1x, pt1y, pt2x, pt2y);
		}
	}

	if(tabIdx == pc->GetSelection())
	{
		wxPen savePen = dc.GetPen();
		wxPen whitePen = wxPen(*wxWHITE);
		whitePen.SetWidth(1);
		dc.SetPen(whitePen);

		wxPoint secPt = wxPoint(tabPoints[5].x + 1, tabPoints[5].y);
		dc.DrawLine(tabPoints[0], secPt);

		// Restore the pen
		dc.SetPen(savePen);
	}

	// -----------------------------------
	// Text and image drawing
	// -----------------------------------

	// Text drawing offset from the left border of the
	// rectangle
	int textOffset;

	// The width of the images are 16 pixels
	int padding = static_cast<wxFlatNotebook*>( pc->GetParent() )->GetPadding();
	int shapePoints = (int)(tabHeight * tan((double)pc->GetPageInfoVector()[tabIdx].GetTabAngle()/180.0*M_PI));
	bool hasImage = pc->GetPageInfoVector()[tabIdx].GetImageIndex() != -1;
	int imageYCoord = pc->HasFlag(wxFNB_BOTTOM) ? 6 : 8;

	hasImage ? textOffset = padding * 2 + 16 + shapePoints / 2 : textOffset = padding + shapePoints / 2 ;
	textOffset += 2;

	if(tabIdx != pc->GetSelection())
	{
		// Set the text background to be like the vertical lines
		dc.SetTextForeground( pc->GetNonoActiveTextColor() );
	}

	if(hasImage)
	{
		int imageXOffset = textOffset - 16 - padding;
		dc.DrawBitmap((*pc->GetImageList())[pc->GetPageInfoVector()[tabIdx].GetImageIndex()],
			posx + imageXOffset, imageYCoord, true);
	}

	dc.DrawText(pc->GetPageText(tabIdx), posx + textOffset, imageYCoord);

	// draw 'x' on tab (if enabled)
	if(pc->HasFlag(wxFNB_X_ON_TAB) && tabIdx == pc->GetSelection())
	{
		int textWidth, textHeight;
		dc.GetTextExtent(pc->GetPageText(tabIdx), &textWidth, &textHeight);
		int tabCloseButtonXCoord = posx + textOffset + textWidth + 1;

		// take a bitmap from the position of the 'x' button (the x on tab button)
		// this bitmap will be used later to delete old buttons
		int tabCloseButtonYCoord = imageYCoord;
		wxRect x_rect(tabCloseButtonXCoord, tabCloseButtonYCoord, 16, 16);
		GetBitmap(dc, x_rect, m_tabXBgBmp);

		// Draw the tab
		DrawTabX(pc, dc, x_rect, tabIdx, btnStatus);			
	}
}

//------------------------------------------------------------------
// Visual studio 7.1 
//------------------------------------------------------------------

void wxFNBRendererVC71::DrawTab(wxWindow* pageContainer, wxDC &dc, const int &posx, const int &tabIdx, const int &tabWidth, const int &tabHeight, const int btnStatus)
{
		// Visual studio 7.1 style
	wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

	dc.SetPen((tabIdx == pc->GetSelection()) ? wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)) : borderPen);
    dc.SetBrush(((tabIdx == pc->GetSelection()) ? wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)) : wxBrush(wxColour(247, 243, 233))));

	if(tabIdx == pc->GetSelection())
	{
		int posy = pc->HasFlag(wxFNB_BOTTOM) ? 0 : VERTICAL_BORDER_PADDING;
		int tabH = pc->HasFlag(wxFNB_BOTTOM) ? tabHeight - 5 : tabHeight - 3;

		dc.DrawRectangle(posx, posy, tabWidth, tabH);

		// Draw a black line on the left side of the
		// rectangle
		wxPen pen = wxPen(*wxBLACK);
		dc.SetPen(pen);

		int blackLineY1 = VERTICAL_BORDER_PADDING;
		int blackLineY2 = tabH;
		dc.DrawLine(posx + tabWidth, blackLineY1, posx + tabWidth, blackLineY2);

		// To give the tab more 3D look we do the following
		// Incase the tab is on top,
		// Draw a thik white line on topof the rectangle
		// Otherwise, draw a thin (1 pixel) black line at the bottom

		pen = wxPen(pc->HasFlag(wxFNB_BOTTOM) ? *wxBLACK : *wxWHITE);
		dc.SetPen(pen);
		int whiteLinePosY = pc->HasFlag(wxFNB_BOTTOM) ? blackLineY2 : VERTICAL_BORDER_PADDING ;
		dc.DrawLine(posx , whiteLinePosY, posx + tabWidth + 1, whiteLinePosY);

		// Draw a white vertical line to the left of the tab
		dc.SetPen( *wxWHITE_PEN );
		if( !pc->HasFlag(wxFNB_BOTTOM) ) blackLineY2 += 1;
		dc.DrawLine(posx, blackLineY1, posx, blackLineY2);
	}
	else
	{
		// We dont draw a rectangle for non selected tabs, but only
		// vertical line on the left

		int blackLineY1 = pc->HasFlag(wxFNB_BOTTOM) ? VERTICAL_BORDER_PADDING + 2 : VERTICAL_BORDER_PADDING + 1;
		int blackLineY2 = pc->GetSize().y - 5 ;
		dc.DrawLine(posx + tabWidth, blackLineY1, posx + tabWidth, blackLineY2);
	}

	// -----------------------------------
	// Text and image drawing
	// -----------------------------------

	// Text drawing offset from the left border of the
	// rectangle
	int textOffset;

	// The width of the images are 16 pixels
	int padding = static_cast<wxFlatNotebook*>( pc->GetParent() )->GetPadding();
	bool hasImage = pc->GetPageInfoVector()[tabIdx].GetImageIndex() != -1;
	int imageYCoord = pc->HasFlag(wxFNB_BOTTOM) ? 5 : 8;
	
	hasImage ? textOffset = padding * 2 + 16 : textOffset = padding;

	if(tabIdx != pc->GetSelection())
	{
		// Set the text background to be like the vertical lines
		dc.SetTextForeground(pc->GetNonoActiveTextColor());
	}

	if(hasImage)
	{
		int imageXOffset = textOffset - 16 - padding;
		dc.DrawBitmap((*pc->GetImageList())[pc->GetPageInfoVector()[tabIdx].GetImageIndex()],
			posx + imageXOffset, imageYCoord, true);
	}

	dc.DrawText(pc->GetPageText(tabIdx), posx + textOffset, imageYCoord);
	
	// draw 'x' on tab (if enabled)
	if(pc->HasFlag(wxFNB_X_ON_TAB) && tabIdx == pc->GetSelection())
	{
		int textWidth, textHeight;
		dc.GetTextExtent(pc->GetPageText(tabIdx), &textWidth, &textHeight);
		int tabCloseButtonXCoord = posx + textOffset + textWidth + 1;

		// take a bitmap from the position of the 'x' button (the x on tab button)
		// this bitmap will be used later to delete old buttons
		int tabCloseButtonYCoord = imageYCoord;
		wxRect x_rect(tabCloseButtonXCoord, tabCloseButtonYCoord, 16, 16);
		GetBitmap(dc, x_rect, m_tabXBgBmp);

		// Draw the tab
		DrawTabX(pc, dc, x_rect, tabIdx, btnStatus);			
	}
}

//------------------------------------------------------------------
// Fancy style
//------------------------------------------------------------------

void wxFNBRendererFancy::DrawTab(wxWindow* pageContainer, wxDC &dc, const int &posx, const int &tabIdx, const int &tabWidth, const int &tabHeight, const int btnStatus)
{
		// Fancy tabs - like with VC71 but with the following differences:
	// - The Selected tab is colored with gradient color
	wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

	wxPen pen = (tabIdx == pc->GetSelection()) ? wxPen( pc->GetBorderColour() ) : wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

	if(tabIdx == pc->GetSelection())
	{
		int posy = pc->HasFlag(wxFNB_BOTTOM) ? 2 : VERTICAL_BORDER_PADDING;
		int th = tabHeight - 5;

		wxRect rect(posx, posy, tabWidth, th);

		wxColour col2 = (pc->HasFlag( wxFNB_BOTTOM ) ) ? pc->GetGradientColourTo() : pc->GetGradientColourFrom();
		wxColour col1 = (pc->HasFlag( wxFNB_BOTTOM ) ) ? pc->GetGradientColourFrom() : pc->GetGradientColourTo();

		PaintStraightGradientBox(dc, rect, col1, col2);
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(pen);
		dc.DrawRectangle(rect);

		// erase the bottom/top line of the rectangle
		dc.SetPen( wxPen( pc->GetGradientColourFrom() ) );
		if(pc->HasFlag(wxFNB_BOTTOM))
            dc.DrawLine(rect.x, 2, rect.x + rect.width, 2);
		else
			dc.DrawLine(rect.x, rect.y + rect.height - 1, rect.x + rect.width, rect.y + rect.height - 1);
	}
	else
	{
		// We dont draw a rectangle for non selected tabs, but only
		// vertical line on the left
		dc.SetPen(borderPen);
		dc.DrawLine(posx + tabWidth, VERTICAL_BORDER_PADDING + 3, posx + tabWidth, tabHeight - 4);
	}

	// -----------------------------------
	// Text and image drawing
	// -----------------------------------

	// Text drawing offset from the left border of the
	// rectangle
	int textOffset;

	// The width of the images are 16 pixels
	int padding = static_cast<wxFlatNotebook*>( pc->GetParent() )->GetPadding();
	bool hasImage = pc->GetPageInfoVector()[tabIdx].GetImageIndex() != -1;
	int imageYCoord = pc->HasFlag(wxFNB_BOTTOM) ? 6 : 8;

	hasImage ? textOffset = padding * 2 + 16 : textOffset = padding ;

	if(tabIdx != pc->GetSelection())
	{
		// Set the text background to be like the vertical lines
		dc.SetTextForeground(pc->GetNonoActiveTextColor());
	}

	if(hasImage)
	{
		int imageXOffset = textOffset - 16 - padding;
		dc.DrawBitmap((*pc->GetImageList())[pc->GetPageInfoVector()[tabIdx].GetImageIndex()],
			posx + imageXOffset, imageYCoord, true);
	}

	dc.DrawText(pc->GetPageText(tabIdx), posx + textOffset, imageYCoord);
	
	// draw 'x' on tab (if enabled)
	if(pc->HasFlag(wxFNB_X_ON_TAB) && tabIdx == pc->GetSelection())
	{
		int textWidth, textHeight;
		dc.GetTextExtent(pc->GetPageText(tabIdx), &textWidth, &textHeight);
		int tabCloseButtonXCoord = posx + textOffset + textWidth + 1;

		// take a bitmap from the position of the 'x' button (the x on tab button)
		// this bitmap will be used later to delete old buttons
		int tabCloseButtonYCoord = imageYCoord;
		wxRect x_rect(tabCloseButtonXCoord, tabCloseButtonYCoord, 16, 16);
		GetBitmap(dc, x_rect, m_tabXBgBmp);

		// Draw the tab
		DrawTabX(pc, dc, x_rect, tabIdx, btnStatus);			
	}
}

//------------------------------------------------------------------
// Visual studio 2005 (VS8)
//------------------------------------------------------------------
void wxFNBRendererVC8::DrawTabs(wxWindow *pageContainer, wxDC &dc)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

#ifndef __WXMAC__ 
	// Works well on MSW & GTK, however this lines should be skipped on MAC
	if(pc->GetPageInfoVector().empty() || pc->m_nFrom >= (int)pc->GetPageInfoVector().GetCount())
	{
		pc->Hide();
		return;
	}
#endif

	// Get the text hight
	int tabHeight = CalcTabHeight( pageContainer );

	// Set the font for measuring the tab height
	wxFont normalFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = normalFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);

	// Calculate the number of rows required for drawing the tabs
	wxRect rect = pc->GetClientRect();

	// Set the maximum client size
	pc->SetSizeHints(wxSize(GetButtonsAreaLength( pc ), tabHeight));
	wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));

	/// Create brushes
	wxBrush backBrush;
	backBrush = wxBrush(pc->m_tabAreaColor);
	wxBrush noselBrush = wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	wxBrush selBrush = wxBrush(pc->m_activeTabColor);
	wxSize size = pc->GetSize();

	// Background
	dc.SetTextBackground(pc->GetBackgroundColour());
	dc.SetTextForeground(pc->m_activeTextColor);
	
	// If border style is set, set the pen to be border pen
	if( pc->HasFlag(wxFNB_TABS_BORDER_SIMPLE) )
		dc.SetPen(borderPen);
	else
		dc.SetPen(*wxTRANSPARENT_PEN);

	int lightFactor = pc->HasFlag(wxFNB_BACKGROUND_GRADIENT) ? 70 : 0;
	/// For VC8 style, we color the tab area in gradient coloring
	PaintStraightGradientBox(dc, pc->GetClientRect(), pc->m_tabAreaColor, LightColour(pc->m_tabAreaColor, lightFactor));

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(0, 0, size.x, size.y);

	// Take 3 bitmaps for the background for the buttons
	{
		wxMemoryDC mem_dc;
		wxRect rect;
		
		//---------------------------------------
		// X button
		//---------------------------------------
		rect = wxRect(GetXPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_xBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);

		//---------------------------------------
		// Right button
		//---------------------------------------
		rect = wxRect(GetRightButtonPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_rightBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);

		//---------------------------------------
		// Left button
		//---------------------------------------
		rect = wxRect(GetLeftButtonPos( pc ), 6, 16, 14);
		mem_dc.SelectObject(m_leftBgBmp);
		mem_dc.Blit(0, 0, rect.width, rect.height, &dc, rect.x, rect.y);
		mem_dc.SelectObject(wxNullBitmap);
	}

	// We always draw the bottom/upper line of the tabs
	// regradless the style
	dc.SetPen(borderPen);
	DrawTabsLine(pc, dc);

	// Restore the pen
	dc.SetPen(borderPen);

	// Draw labels
	dc.SetFont(boldFont);
	int posx;
	int i = 0, cur = 0;

	// Update all the tabs from 0 to 'pc->m_nFrom' to be non visible
	for(i=0; i<pc->m_nFrom; i++)
	{
		pc->GetPageInfoVector()[i].SetPosition(wxPoint(-1, -1));
		pc->GetPageInfoVector()[i].GetRegion().Clear();
	}

	// Draw the visible tabs, in VC8 style, we draw them from right to left
	std::vector<wxRect> vTabsInfo;
	NumberTabsCanFit(pc, vTabsInfo);
	int activeTabPosx(0);
	int activeTabWidth(0);
	int	activeTabHeight(0);

	for(cur=(int)vTabsInfo.size() - 1; cur>=0; cur--) 
	{
		/// 'i' points to the index of the currently drawn tab
		/// in pc->GetPageInfoVector() vector
		i = pc->m_nFrom + cur;
		dc.SetPen(borderPen);
		dc.SetBrush((i==pc->GetSelection()) ? selBrush : noselBrush);

		// Now set the font to the correct font
		dc.SetFont((i==pc->GetSelection()) ? boldFont : normalFont);

		// Add the padding to the tab width
		// Tab width:
		// +-----------------------------------------------------------+
		// | PADDING | IMG | IMG_PADDING | TEXT | PADDING | x |PADDING |
		// +-----------------------------------------------------------+

		int tabWidth = CalcTabWidth( pageContainer, i, tabHeight );
		posx = vTabsInfo[cur].x;

		// By default we clean the tab region
		// incase we use the VC8 style which requires
		// the region, it will be filled by the function
		// drawVc8Tab
		pc->GetPageInfoVector()[i].GetRegion().Clear();
		
		// Clean the 'x' buttn on the tab 
		// 'Clean' rectanlge is a rectangle with width or height
		// with values lower than or equal to 0
		pc->GetPageInfoVector()[i].GetXRect().SetSize(wxSize(-1, -1));

		// Draw the tab
		// Incase we are drawing the active tab
		// we need to redraw so it will appear on top
		// of all other tabs

		// when using the vc8 style, we keep the position of the active tab so we will draw it again later
		if( i == pc->GetSelection() && pc->HasFlag( wxFNB_VC8 ) )
		{
			activeTabPosx = posx;
			activeTabWidth = tabWidth;
			activeTabHeight = tabHeight;
		}
		else
		{
			DrawTab(pc, dc, posx, i, tabWidth, tabHeight, pc->m_nTabXButtonStatus);
		}

		// Restore the text forground
		dc.SetTextForeground(pc->m_activeTextColor);

		// Update the tab position & size
		pc->GetPageInfoVector()[i].SetPosition(wxPoint(posx, VERTICAL_BORDER_PADDING));
		pc->GetPageInfoVector()[i].SetSize(wxSize(tabWidth, tabHeight));
	}

	// Incase we are in VC8 style, redraw the active tab (incase it is visible)
	if(pc->GetSelection() >= pc->m_nFrom && pc->GetSelection()< pc->m_nFrom + (int)vTabsInfo.size() )
	{
		DrawTab(pc, dc, activeTabPosx, pc->GetSelection(), activeTabWidth, activeTabHeight, pc->m_nTabXButtonStatus);
	}

	// Update all tabs that can not fit into the screen as non-visible
	int xx;
	for(xx = pc->m_nFrom + (int)vTabsInfo.size(); xx<(int)pc->GetPageInfoVector().size(); xx++)
	{
		pc->GetPageInfoVector()[xx].SetPosition(wxPoint(-1, -1));
		pc->GetPageInfoVector()[xx].GetRegion().Clear();
	}

	// Draw the left/right/close buttons 
	// Left arrow
	DrawLeftArrow(pc, dc);
	DrawRightArrow(pc, dc);
	DrawX(pc, dc);
	DrawDropDownArrow(pc, dc);
}

void wxFNBRendererVC8::DrawTab(wxWindow* pageContainer, wxDC &dc, const int &posx, const int &tabIdx, const int &tabWidth, const int &tabHeight, const int btnStatus)
{
	// Fancy tabs - like with VC71 but with the following differences:
	// - The Selected tab is colored with gradient color
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	wxPen borderPen = wxPen( pc->GetBorderColour() );
	wxPoint tabPoints[8];

	// If we draw the first tab or the active tab, 
	// we draw a full tab, else we draw a truncated tab
	//
	//             X(2)                  X(3)
	//        X(1)                            X(4)
	//                                          
	//                                           X(5)
	//                                           
	// X(0),(7)                                  X(6)
	//
	//

	tabPoints[0].x = pc->HasFlag( wxFNB_BOTTOM ) ? posx : posx + m_factor;
	tabPoints[0].y = pc->HasFlag( wxFNB_BOTTOM ) ? 2 : tabHeight - 3;

	tabPoints[1].x = tabPoints[0].x + tabHeight - VERTICAL_BORDER_PADDING - 3 - m_factor;
	tabPoints[1].y = pc->HasFlag( wxFNB_BOTTOM ) ? tabHeight  - (VERTICAL_BORDER_PADDING+2) : (VERTICAL_BORDER_PADDING+2);

	tabPoints[2].x = tabPoints[1].x + 4;
	tabPoints[2].y = pc->HasFlag( wxFNB_BOTTOM ) ? tabHeight  - VERTICAL_BORDER_PADDING : VERTICAL_BORDER_PADDING;

	tabPoints[3].x = tabPoints[2].x + tabWidth - 2;
	tabPoints[3].y = pc->HasFlag( wxFNB_BOTTOM ) ? tabHeight  - VERTICAL_BORDER_PADDING : VERTICAL_BORDER_PADDING;

	tabPoints[4].x = tabPoints[3].x + 1;
	tabPoints[4].y = pc->HasFlag( wxFNB_BOTTOM ) ? tabPoints[3].y - 1 : tabPoints[3].y + 1;

	tabPoints[5].x = tabPoints[4].x + 1;
	tabPoints[5].y = pc->HasFlag( wxFNB_BOTTOM ) ? (tabPoints[4].y - 1 ): tabPoints[4].y + 1;

	tabPoints[6].x = tabPoints[2].x + tabWidth;
	tabPoints[6].y = tabPoints[0].y;

	tabPoints[7].x = tabPoints[0].x;
	tabPoints[7].y = tabPoints[0].y;

	pc->GetPageInfoVector()[tabIdx].SetRegion(8, tabPoints);

	// Draw the polygon
	wxBrush br = dc.GetBrush();
	dc.SetBrush(wxBrush(tabIdx == pc->GetSelection() ? pc->GetActiveTabColour() : pc->GetGradientColourTo() ));
	dc.SetPen(wxPen(tabIdx == pc->GetSelection() ? wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW) : pc->GetBorderColour()));
	dc.DrawPolygon(8, tabPoints);

	// Restore the brush
	dc.SetBrush(br);

	wxRect rect = pc->GetClientRect();

	if(tabIdx != pc->GetSelection() && !pc->HasFlag( wxFNB_BOTTOM ))
	{
		// Top default tabs
		dc.SetPen(wxPen( pc->GetBorderColour() ));
		int lineY = rect.height;
		wxPen curPen = dc.GetPen();
		curPen.SetWidth(1);
		dc.SetPen(curPen);
		dc.DrawLine(posx, lineY, posx+rect.width, lineY);
	}

	// Incase we are drawing the selected tab, we draw the border of it as well
	// but without the bottom (upper line incase of wxBOTTOM)
	if(tabIdx == pc->GetSelection())
	{
		wxPen borderPen = wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW));
		wxBrush brush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(borderPen);
		dc.SetBrush(brush);
		dc.DrawPolygon(8, tabPoints);

		// Delete the bottom line (or the upper one, incase we use wxBOTTOM) 
		dc.SetPen(wxPen(wxT("WHITE")));
		dc.DrawLine(tabPoints[0], tabPoints[6]);
	}


	FillVC8GradientColor(pc, dc, tabPoints, tabIdx == pc->GetSelection(), tabIdx);

	// Draw a thin line to the right of the non-selected tab
	if(tabIdx != pc->GetSelection())
	{
		dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)));
		dc.DrawLine(tabPoints[4].x-1, tabPoints[4].y, tabPoints[5].x-1, tabPoints[5].y);
		dc.DrawLine(tabPoints[5].x-1, tabPoints[5].y, tabPoints[6].x-1, tabPoints[6].y);
	}

	// Text drawing offset from the left border of the 
	// rectangle
	int textOffset;

	// The width of the images are 16 pixels
	int vc8ShapeLen = tabHeight - VERTICAL_BORDER_PADDING - 2;
	if( pc->TabHasImage( tabIdx ) )
		textOffset = ((wxFlatNotebook *)pc->m_pParent)->GetPadding() * 2 + 16 + vc8ShapeLen; 
	else
		textOffset = ((wxFlatNotebook *)pc->m_pParent)->GetPadding() + vc8ShapeLen;

	// Draw the image for the tab if any
	int imageYCoord = pc->HasFlag(wxFNB_BOTTOM) ? 6 : 8;

	if( pc->TabHasImage( tabIdx ) )
	{
		int imageXOffset = textOffset - 16 - ((wxFlatNotebook *)pc->m_pParent)->GetPadding();
		dc.DrawBitmap((*pc->GetImageList())[pc->GetPageInfoVector()[tabIdx].GetImageIndex()], 
			posx + imageXOffset, imageYCoord, true);
	}

	wxFont boldFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
		
	// if selected tab, draw text in bold
	if( tabIdx == pc->GetSelection() )
	{
		boldFont.SetWeight(wxFONTWEIGHT_BOLD);
	}

	dc.SetFont( boldFont );
	dc.DrawText(pc->GetPageText(tabIdx), posx + textOffset, imageYCoord);
	
	// draw 'x' on tab (if enabled)
	if(pc->HasFlag(wxFNB_X_ON_TAB) && tabIdx == pc->GetSelection())
	{
		int textWidth, textHeight;
		dc.GetTextExtent(pc->GetPageText(tabIdx), &textWidth, &textHeight);
		int tabCloseButtonXCoord = posx + textOffset + textWidth + 1;

		// take a bitmap from the position of the 'x' button (the x on tab button)
		// this bitmap will be used later to delete old buttons
		int tabCloseButtonYCoord = imageYCoord;
		wxRect x_rect(tabCloseButtonXCoord, tabCloseButtonYCoord, 16, 16);
		GetBitmap(dc, x_rect, m_tabXBgBmp);
		wxBitmap bmp( 16, 16 );

		// Draw the tab
		DrawTabX(pc, dc, x_rect, tabIdx, btnStatus);			
	}
}

void wxFNBRendererVC8::FillVC8GradientColor(wxWindow* pageContainer, wxDC &dc, const wxPoint tabPoints[], const bool bSelectedTab, const int tabIdx)
{
	// calculate gradient coefficients
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );

	if( m_first )
	{
		m_first = false;
		pc->m_colorTo   = LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), 0); 
		pc->m_colorFrom = LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), 60);
	}
	wxColour col2 = pc->HasFlag( wxFNB_BOTTOM ) ? pc->GetGradientColourTo() : pc->GetGradientColourFrom();
	wxColour col1 = pc->HasFlag( wxFNB_BOTTOM ) ? pc->GetGradientColourFrom() : pc->GetGradientColourTo();

	// If colorful tabs style is set, override the tab color
	if(pc->HasFlag(wxFNB_COLORFUL_TABS))
	{
		if( !pc->GetPageInfoVector()[tabIdx].GetColor().Ok() )
		{
			// First time, generate color, and keep it in the vector
			wxColor tabColor(RandomColor());
			pc->GetPageInfoVector()[tabIdx].SetColor(tabColor);
		}

		if( pc->HasFlag(wxFNB_BOTTOM) )
		{
			col2 = LightColour( pc->GetPageInfoVector()[tabIdx].GetColor() , 50 );
			col1 = LightColour( pc->GetPageInfoVector()[tabIdx].GetColor() , 80 );
		}
		else
		{
			col1 = LightColour( pc->GetPageInfoVector()[tabIdx].GetColor() , 50 );
			col2 = LightColour( pc->GetPageInfoVector()[tabIdx].GetColor() , 80 );
		}
	}

	int size = abs(tabPoints[2].y - tabPoints[0].y) - 1;

	double rstep = double((col2.Red() -   col1.Red())) / double(size), rf = 0,
		gstep = double((col2.Green() - col1.Green())) / double(size), gf = 0,
		bstep = double((col2.Blue() -  col1.Blue())) / double(size), bf = 0;

	wxColour currCol;
	int y = tabPoints[0].y; 

	// If we are drawing the selected tab, we need also to draw a line 
	// from 0->tabPoints[0].x and tabPoints[6].x -> end, we achieve this
	// by drawing the rectangle with transparent brush
	// the line under the selected tab will be deleted by the drwaing loop
	if( bSelectedTab )
		DrawTabsLine(pc, dc);

	while( true )
	{
		if(pc->HasFlag( wxFNB_BOTTOM ))
		{
			if(y > tabPoints[0].y + size)
				break;
		}
		else
		{
			if(y < tabPoints[0].y - size)
				break;
		}

		currCol.Set(
			(unsigned char)(col1.Red() + rf),
			(unsigned char)(col1.Green() + gf),
			(unsigned char)(col1.Blue() + bf)
			);

		dc.SetPen(bSelectedTab ? wxPen(pc->m_activeTabColor) : wxPen(currCol));
		int startX = GetStartX(tabPoints, y, pc->GetParent()->GetWindowStyleFlag()) ;
		int endX   = GetEndX(tabPoints, y, pc->GetParent()->GetWindowStyleFlag());
		dc.DrawLine(startX, y, endX, y);

		// Draw the border using the 'edge' point
		dc.SetPen(wxPen(bSelectedTab ? wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW) : pc->m_colorBorder));
		
		dc.DrawPoint(startX, y);
		dc.DrawPoint(endX, y);
		
		// Progress the color 
		rf += rstep; gf += gstep; bf += bstep;

		pc->HasFlag( wxFNB_BOTTOM ) ? y++ : y--;
	}
}

int wxFNBRendererVC8::GetStartX(const wxPoint tabPoints[], const int &y, long style)
{
	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

	// We check the 3 points to the left
	bool bBottomStyle = style & wxFNB_BOTTOM ? true : false;
	bool match = false;

	if(bBottomStyle)
	{
		for(int i=0; i<3; i++)
		{	
			if(y >= tabPoints[i].y && y < tabPoints[i+1].y)
			{
				x1 = tabPoints[i].x;
				x2 = tabPoints[i+1].x;
				y1 = tabPoints[i].y;
				y2 = tabPoints[i+1].y;
				match = true;
				break;
			}
		}
	}
	else
	{
		for(int i=0; i<3; i++)
		{	
			if(y <= tabPoints[i].y && y > tabPoints[i+1].y)
			{
				x1 = tabPoints[i].x;
				x2 = tabPoints[i+1].x;
				y1 = tabPoints[i].y;
				y2 = tabPoints[i+1].y;
				match = true;
				break;
			}
		}
	}

	if(!match)
		return tabPoints[2].x;

	// According to the equation y = ax + b => x = (y-b)/a
	// We know the first 2 points

	double a;
	if(x2 == x1) 
		return static_cast<int>( x2 );
	else
		a = (y2 - y1) / (x2 - x1);

	double b = y1 - ((y2 - y1) / (x2 - x1)) * x1;

	if(a == 0) 
		return static_cast<int>( x1 );

	double x = (y - b) / a;
	return static_cast<int>( x );
}

int wxFNBRendererVC8::GetEndX(const wxPoint tabPoints[], const int &y, long style)
{
	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

	// We check the 3 points to the left
	bool bBottomStyle = style & wxFNB_BOTTOM ? true : false;
	bool match = false;

	if(bBottomStyle)
	{
		for(int i=7; i>3; i--)
		{	
			if(y >= tabPoints[i].y && y < tabPoints[i-1].y)
			{
				x1 = tabPoints[i].x;
				x2 = tabPoints[i-1].x;
				y1 = tabPoints[i].y;
				y2 = tabPoints[i-1].y;
				match = true;
				break;
			}
		}
	}
	else
	{
		for(int i=7; i>3; i--)
		{	
			if(y <= tabPoints[i].y && y > tabPoints[i-1].y)
			{
				x1 = tabPoints[i].x;
				x2 = tabPoints[i-1].x;
				y1 = tabPoints[i].y;
				y2 = tabPoints[i-1].y;
				match = true;
				break;
			}
		}
	}

	if(!match)
		return tabPoints[3].x;

	// According to the equation y = ax + b => x = (y-b)/a
	// We know the first 2 points
	double a = (y2 - y1) / (x2 - x1);
	double b = y1 - ((y2 - y1) / (x2 - x1)) * x1;

	if(a == 0) 
		return (int)x1;

	// Vertical line
	if(x1 == x2)
		return (int)x1;

	double x = (y - b) / a;
	return (int)x;
}

void wxFNBRendererVC8::NumberTabsCanFit(wxWindow *pageContainer, std::vector<wxRect> &vTabInfo, int from)
{
	wxPageContainer *pc = static_cast<wxPageContainer*>( pageContainer );
	int tabHeight, clientWidth;
	
	wxRect rect = pc->GetClientRect();
	clientWidth = rect.width;

	/// Empty results
	vTabInfo.clear();

	tabHeight = CalcTabHeight( pageContainer );

	// The drawing starts from posx
	int posx = ((wxFlatNotebook *)pc->m_pParent)->GetPadding();
	
	if( from < 0 )
		from = pc->m_nFrom;

	for(int i = from; i<(int)pc->GetPageInfoVector().GetCount(); i++)
	{
		int vc8glitch = tabHeight + wxFNB_HEIGHT_SPACER;
		int tabWidth = CalcTabWidth( pageContainer, i, tabHeight );
		if(posx + tabWidth + vc8glitch + GetButtonsAreaLength( pc ) >= clientWidth)
			break;

		/// Add a result to the returned vector
		wxRect tabRect(posx, VERTICAL_BORDER_PADDING, tabWidth , tabHeight);
		vTabInfo.push_back(tabRect);

		/// Advance posx
		posx += tabWidth + wxFNB_HEIGHT_SPACER;
	}
}
