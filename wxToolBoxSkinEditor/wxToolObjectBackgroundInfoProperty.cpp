#include "stdwx.h"
#include "AdditionalProperties.h"

bool operator == (const wxToolObjectBackgroundInfo & a, const wxToolObjectBackgroundInfo & b)
{
    return (a.m_NormalBitmapSetInfo == b.m_NormalBitmapSetInfo && 
			a.m_HoverBitmapSetInfo == b.m_HoverBitmapSetInfo && 
			a.m_SelectedBitmapSetInfo == b.m_SelectedBitmapSetInfo);
}

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(wxToolObjectBackgroundInfo)

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxToolObjectBackgroundInfoProperty, wxPGProperty,
	wxToolObjectBackgroundInfo, const wxToolObjectBackgroundInfo&, TextCtrl);

wxToolObjectBackgroundInfoProperty::wxToolObjectBackgroundInfoProperty(const wxString& label,
	const wxString& name,
	const wxToolObjectBackgroundInfo& value)
	: wxPGProperty(label, name)
{
	SetValue(WXVARIANT(value));
	AddPrivateChild(new wxBitmapSetInfoProperty(wxT("Normal"), wxPG_LABEL, value.m_NormalBitmapSetInfo));
	AddPrivateChild(new wxBitmapSetInfoProperty(wxT("Hover"), wxPG_LABEL, value.m_HoverBitmapSetInfo));
	AddPrivateChild(new wxBitmapSetInfoProperty(wxT("Selected"), wxPG_LABEL, value.m_SelectedBitmapSetInfo));
}

wxToolObjectBackgroundInfoProperty::~wxToolObjectBackgroundInfoProperty() 
{
}

void wxToolObjectBackgroundInfoProperty::RefreshChildren()
{
	if (!GetChildCount()) return;
	wxToolObjectBackgroundInfo& value = wxToolObjectBackgroundInfoRefFromVariant(m_value);
	wxVariant childValue;
	childValue << value.m_NormalBitmapSetInfo;
	Item(0)->SetValue(childValue);
	childValue << value.m_HoverBitmapSetInfo;
	Item(1)->SetValue(childValue);
	childValue << value.m_SelectedBitmapSetInfo;
	Item(2)->SetValue(childValue);
}

wxVariant wxToolObjectBackgroundInfoProperty::ChildChanged(wxVariant& thisValue, int childIndex, wxVariant& childValue) const
{
	wxToolObjectBackgroundInfo value;
	value << thisValue;
	switch (childIndex)
	{
	case 0: value.m_NormalBitmapSetInfo << childValue; break;
	case 1: value.m_HoverBitmapSetInfo << childValue; break;
	case 2: value.m_SelectedBitmapSetInfo << childValue; break;
	}
	wxVariant newVariant;
	newVariant << value;
	return newVariant;
}
