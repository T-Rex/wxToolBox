#ifndef _ADDITIONAL_PROPERTIES_H
#define _ADDITIONAL_PROPERTIES_H

#include <wx/wx.h>

// Main propertygrid header.
#include <wx/propgrid/propgrid.h>
// Needed for implementing custom properties.
#include <wx/propgrid/property.h>
#include <wx/propgrid/propgriddefs.h>
#include <wx/propgrid/propgridiface.h>
#include <wx/propgrid/props.h>
// Extra property classes.
#include <wx/propgrid/advprops.h>
// This defines wxPropertyGridManager.
#include <wx/propgrid/manager.h>

// ------------------------------------------------------------------------
//WX_PG_DECLARE_PROPERTY(wxAdvImageFileProperty,const wxString&,wxEmptyString)

// ------------------------------------------------------------------------
class wxBitmapSetInfo
{
public:
    wxBitmapSetInfo()
    {
        m_LeftBitmapPath = m_RightBitmapPath = m_MiddleBitmapPath = wxEmptyString;
    }
    wxBitmapSetInfo(wxString leftpath, wxString middlepath, wxString rightpath)
    {
        m_LeftBitmapPath = leftpath; 
		m_MiddleBitmapPath = middlepath; 
		m_RightBitmapPath = rightpath;
    }

	wxBitmapSetInfo & operator = (const wxBitmapSetInfo & bsi)
	{
		m_LeftBitmapPath = bsi.m_LeftBitmapPath;
		m_MiddleBitmapPath = bsi.m_MiddleBitmapPath;
		m_RightBitmapPath = bsi.m_RightBitmapPath;
		return *this;
	}

    wxString m_LeftBitmapPath;
	wxString m_MiddleBitmapPath;
	wxString m_RightBitmapPath;
};

bool operator == (const wxBitmapSetInfo & a, const wxBitmapSetInfo & b);

//WX_PG_DECLARE_VALUE_TYPE_VOIDP(wxBitmapSetInfo)
//WX_PG_DECLARE_PROPERTY(wxBitmapSetInfoProperty,const wxBitmapSetInfo &,wxBitmapSetInfo())

// ------------------------------------------------------------------------

class wxToolObjectBackgroundInfo : public wxObject
{
public:
    wxToolObjectBackgroundInfo()
    {        
    }

    wxToolObjectBackgroundInfo(const wxBitmapSetInfo & normal, 
							   const wxBitmapSetInfo & hover,
							   const wxBitmapSetInfo & selected)
    {
        m_NormalBitmapSetInfo = normal;
		m_HoverBitmapSetInfo = hover;
		m_SelectedBitmapSetInfo = selected;
    }

	void GetFileNames(wxString & normal_left, wxString & normal_middle, wxString & normal_right,
		wxString & hover_left, wxString & hover_middle, wxString & hover_right,
		wxString & selected_left, wxString & selected_middle, wxString & selected_right)
	{
		normal_left = m_NormalBitmapSetInfo.m_LeftBitmapPath;
		normal_middle = m_NormalBitmapSetInfo.m_MiddleBitmapPath;
		normal_right = m_NormalBitmapSetInfo.m_RightBitmapPath;

		hover_left = m_HoverBitmapSetInfo.m_LeftBitmapPath;
		hover_middle = m_HoverBitmapSetInfo.m_MiddleBitmapPath;
		hover_right = m_HoverBitmapSetInfo.m_RightBitmapPath;

		selected_left = m_SelectedBitmapSetInfo.m_LeftBitmapPath;
		selected_middle = m_SelectedBitmapSetInfo.m_MiddleBitmapPath;
		selected_right = m_SelectedBitmapSetInfo.m_RightBitmapPath;
	}

	void SetFileNames(wxString normal_left, wxString normal_middle, wxString normal_right,
		wxString hover_left, wxString hover_middle, wxString hover_right,
		wxString selected_left, wxString selected_middle, wxString selected_right)
	{
		 m_NormalBitmapSetInfo.m_LeftBitmapPath = normal_left;
		 m_NormalBitmapSetInfo.m_MiddleBitmapPath = normal_middle;
		 m_NormalBitmapSetInfo.m_RightBitmapPath = normal_right;

		 m_HoverBitmapSetInfo.m_LeftBitmapPath = hover_left;
		 m_HoverBitmapSetInfo.m_MiddleBitmapPath = hover_middle;
		 m_HoverBitmapSetInfo.m_RightBitmapPath = hover_right;

		 m_SelectedBitmapSetInfo.m_LeftBitmapPath = selected_left;
		 m_SelectedBitmapSetInfo.m_MiddleBitmapPath = selected_middle;
		 m_SelectedBitmapSetInfo.m_RightBitmapPath = selected_right;
	}

    wxBitmapSetInfo m_NormalBitmapSetInfo;
	wxBitmapSetInfo m_HoverBitmapSetInfo;
	wxBitmapSetInfo m_SelectedBitmapSetInfo;
};

bool operator == (const wxToolObjectBackgroundInfo & a, const wxToolObjectBackgroundInfo & b);

//WX_PG_DECLARE_VALUE_TYPE_VOIDP(wxToolObjectBackgroundInfo)
//WX_PG_DECLARE_PROPERTY(wxToolObjectBackgroundInfoProperty,
//					   const wxToolObjectBackgroundInfo &,
//					   wxToolObjectBackgroundInfo())
//
//class wxToolObjectBackgroundInfoPropertyClass : public wxPGPropertyWithChildren
//{
//    WX_PG_DECLARE_PROPERTY_CLASS()
//public:
//
//    wxToolObjectBackgroundInfoPropertyClass ( const wxString& label, const wxString& name,
//        const wxToolObjectBackgroundInfo& value );
//    virtual ~wxToolObjectBackgroundInfoPropertyClass ();
//
//    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
//    WX_PG_DECLARE_PARENTAL_METHODS()
//
//protected:
//    wxToolObjectBackgroundInfo m_value;
//};

#endif
