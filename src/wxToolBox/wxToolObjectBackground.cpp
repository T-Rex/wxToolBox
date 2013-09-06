#include "wx/wxToolBox/wxToolBox.h"
#include <algorithm>
using namespace std;

wxToolBoxBitmapSet::wxToolBoxBitmapSet(wxString left, wxString middle, wxString right)
: m_LeftPath(left), m_MiddlePath(middle), m_RightPath(right)
{
	if(left != wxEmptyString) 
	{
		m_LeftBitmap.LoadFile(left, wxBITMAP_TYPE_ANY);
	} else m_LeftBitmap = wxNullBitmap;
	if(middle != wxEmptyString) 
	{
		m_MiddleBitmap.LoadFile(middle, wxBITMAP_TYPE_ANY);
	} else m_MiddleBitmap = wxNullBitmap;
	if(right != wxEmptyString)
	{
		m_RightBitmap.LoadFile(right, wxBITMAP_TYPE_ANY);
	} else m_RightBitmap = wxNullBitmap;
}

wxToolBoxBitmapSet::wxToolBoxBitmapSet(const wxToolBoxBitmapSet & bmpset)
: m_LeftPath(bmpset.m_LeftPath),m_MiddlePath(bmpset.m_MiddlePath), 
m_RightPath(bmpset.m_RightPath), m_LeftBitmap(bmpset.m_LeftBitmap), 
m_MiddleBitmap(bmpset.m_MiddleBitmap), m_RightBitmap(bmpset.m_RightBitmap)
{
}

wxToolBoxBitmapSet wxToolBoxBitmapSet::operator = (const wxToolBoxBitmapSet bmpset)
{
	m_LeftPath = bmpset.m_LeftPath;
	m_MiddlePath = bmpset.m_MiddlePath;
	m_RightPath = bmpset.m_RightPath;
	m_LeftBitmap = bmpset.m_LeftBitmap;
	m_MiddleBitmap = bmpset.m_MiddleBitmap;
	m_RightBitmap = bmpset.m_RightBitmap;
	return *this;
}

void wxToolBoxBitmapSet::SetLeftBitmap(wxString bitmap) 
{
	if(bitmap != wxEmptyString)
	{
		m_LeftPath = bitmap;
		m_LeftBitmap.LoadFile(bitmap, wxBITMAP_TYPE_ANY);
	} else m_LeftBitmap = wxNullBitmap;
}

void wxToolBoxBitmapSet::SetMiddleBitmap(wxString bitmap) 
{
	if(bitmap != wxEmptyString)
	{
		m_MiddlePath = bitmap;
		m_MiddleBitmap.LoadFile(bitmap, wxBITMAP_TYPE_ANY);
	} else m_MiddleBitmap = wxNullBitmap;
}

void wxToolBoxBitmapSet::SetRightBitmap(wxString bitmap) 
{
	if(bitmap != wxEmptyString)
	{
		m_RightPath = bitmap;
		m_RightBitmap.LoadFile(bitmap, wxBITMAP_TYPE_ANY);
	} else m_RightBitmap = wxNullBitmap;
}

unsigned int wxToolBoxBitmapSet::GetMaxBitmapHeight() 
{
	return wxMax((int)m_RightBitmap.GetHeight(), 
					(int)wxMax((int)m_MiddleBitmap.GetHeight(), (int)m_LeftBitmap.GetHeight()));
}

wxToolObjectBackground::wxToolObjectBackground(wxToolBoxBitmapSet & normal, 
											   wxToolBoxBitmapSet & hover, 
											   wxToolBoxBitmapSet & selected)
	: m_NormalBackground(normal), m_HoverBackground(hover), m_SelectedBackground(selected)
{
}

unsigned int wxToolObjectBackground::GetMaxBitmapHeight() 
{
	return wxMax((int)m_NormalBackground.GetMaxBitmapHeight(), 
					(int)wxMax((int)m_HoverBackground.GetMaxBitmapHeight(), 
								  (int)m_SelectedBackground.GetMaxBitmapHeight()));
}
