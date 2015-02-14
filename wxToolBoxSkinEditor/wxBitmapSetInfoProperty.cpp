#include "stdwx.h"
#include "AdditionalProperties.h"

// -----------------------------------------------------------------------
// wxBitmapSetInfoProperty
// -----------------------------------------------------------------------

bool operator == (const wxBitmapSetInfo & a, const wxBitmapSetInfo & b)
{
    return (a.m_LeftBitmapPath == b.m_LeftBitmapPath && 
			a.m_MiddleBitmapPath == b.m_MiddleBitmapPath && 
			a.m_RightBitmapPath == b.m_RightBitmapPath);
}

//WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(wxBitmapSetInfo,wxBitmapSetInfoProperty,wxBitmapSetInfo())


//class wxBitmapSetInfoPropertyClass : public wxPGPropertyWithChildren
//{
//    WX_PG_DECLARE_PROPERTY_CLASS()
//public:
//
//    wxBitmapSetInfoPropertyClass ( const wxString& label, const wxString& name,
//        const wxBitmapSetInfo& value );
//    virtual ~wxBitmapSetInfoPropertyClass ();
//
//    WX_PG_DECLARE_PARENTAL_TYPE_METHODS()
//    WX_PG_DECLARE_PARENTAL_METHODS()
//
//protected:
//    wxBitmapSetInfo m_value;
//};
//
//
//WX_PG_IMPLEMENT_PROPERTY_CLASS(wxBitmapSetInfoProperty,wxBaseParentProperty,
//                               wxBitmapSetInfo,const wxBitmapSetInfo&,TextCtrl)
//
//
//wxBitmapSetInfoPropertyClass::wxBitmapSetInfoPropertyClass ( const wxString& label, 
//	const wxString& name, const wxBitmapSetInfo& value) 
//	: wxPGPropertyWithChildren(label,name)
//{
//    wxPG_INIT_REQUIRED_TYPE(wxBitmapSetInfo)
//    DoSetValue((void*)&value);
//	AddChild( wxAdvImageFileProperty(wxT("Left"),wxPG_LABEL,value.m_LeftBitmapPath) );
//	AddChild( wxAdvImageFileProperty(wxT("Middle"),wxPG_LABEL,value.m_MiddleBitmapPath) );
//	AddChild( wxAdvImageFileProperty(wxT("Right"),wxPG_LABEL,value.m_RightBitmapPath) );
//}
//
//wxBitmapSetInfoPropertyClass::~wxBitmapSetInfoPropertyClass () { }
//
//void wxBitmapSetInfoPropertyClass::DoSetValue ( wxPGVariant value )
//{
//    wxBitmapSetInfo * pObj = (wxBitmapSetInfo *)wxPGVariantToVoidPtr(value);
//    m_value = *pObj;
//    RefreshChildren();
//}
//
//wxPGVariant wxBitmapSetInfoPropertyClass::DoGetValue () const
//{
//    return wxPGVariant((void*)&m_value);
//}
//
//void wxBitmapSetInfoPropertyClass::RefreshChildren()
//{
//    if ( !GetCount() ) return;
//    Item(0)->DoSetValue( m_value.m_LeftBitmapPath );
//    Item(1)->DoSetValue( m_value.m_MiddleBitmapPath );
//    Item(2)->DoSetValue( m_value.m_RightBitmapPath );
//}
//
//void wxBitmapSetInfoPropertyClass::ChildChanged ( wxPGProperty* p )
//{
//    switch ( p->GetIndexInParent() )
//    {
//        case 0: m_value.m_LeftBitmapPath = p->DoGetValue().GetString(); break;
//        case 1: m_value.m_MiddleBitmapPath = p->DoGetValue().GetString(); break;
//        case 2: m_value.m_RightBitmapPath = p->DoGetValue().GetString(); break;
//    }
//}