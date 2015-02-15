#include "stdwx.h"
#include "AdditionalProperties.h"

bool operator == (const wxBitmapSetInfo& a, const wxBitmapSetInfo& b)
{
	return (a.m_LeftBitmapPath == b.m_LeftBitmapPath
		&& a.m_MiddleBitmapPath == b.m_MiddleBitmapPath
		&& a.m_RightBitmapPath == b.m_RightBitmapPath);
}

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(wxBitmapSetInfo)
WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBitmapSetInfoProperty, wxPGProperty, wxBitmapSetInfo, const wxBitmapSetInfo&, TextCtrl)

wxBitmapSetInfoProperty::wxBitmapSetInfoProperty(const wxString& label, 
	const wxString& name, 
	const wxBitmapSetInfo& value)
: wxPGProperty(label, name)
{
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxFileProperty(wxT("Left"), wxPG_LABEL, value.m_LeftBitmapPath));
	AddPrivateChild(new wxFileProperty(wxT("Middle"), wxPG_LABEL, value.m_MiddleBitmapPath));
	AddPrivateChild(new wxFileProperty(wxT("Right"), wxPG_LABEL, value.m_RightBitmapPath));
}

wxBitmapSetInfoProperty::~wxBitmapSetInfoProperty() 
{
}

void wxBitmapSetInfoProperty::RefreshChildren()
{
	if (!GetChildCount()) return;
	const wxBitmapSetInfo& value = wxBitmapSetInfoRefFromVariant(m_value);
	Item(0)->SetValue(value.m_LeftBitmapPath);
	Item(1)->SetValue(value.m_MiddleBitmapPath);
	Item(2)->SetValue(value.m_RightBitmapPath);
}

wxVariant wxBitmapSetInfoProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	wxBitmapSetInfo value;
	value << thisValue;
	switch (childIndex)
	{
	case 0: value.m_LeftBitmapPath = childValue.GetString(); break;
	case 1: value.m_MiddleBitmapPath = childValue.GetString(); break;
	case 2: value.m_RightBitmapPath = childValue.GetString(); break;
	}
	wxVariant newVariant;
	newVariant << value;
	return newVariant;
}
