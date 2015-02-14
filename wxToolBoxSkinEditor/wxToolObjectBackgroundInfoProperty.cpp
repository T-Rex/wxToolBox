#include "AdditionalProperties.h"

bool operator == (const wxToolObjectBackgroundInfo & a, const wxToolObjectBackgroundInfo & b)
{
    return (a.m_NormalBitmapSetInfo == b.m_NormalBitmapSetInfo && 
			a.m_HoverBitmapSetInfo == b.m_HoverBitmapSetInfo && 
			a.m_SelectedBitmapSetInfo == b.m_SelectedBitmapSetInfo);
}

//WX_PG_IMPLEMENT_VALUE_TYPE_VOIDP(wxToolObjectBackgroundInfo,
//								 wxToolObjectBackgroundInfoProperty,
//								 wxToolObjectBackgroundInfo())
//
//WX_PG_IMPLEMENT_PROPERTY_CLASS(wxToolObjectBackgroundInfoProperty,wxBaseParentProperty,
//                               wxToolObjectBackgroundInfo,const wxToolObjectBackgroundInfo&,TextCtrl)
//
//
//wxToolObjectBackgroundInfoPropertyClass::wxToolObjectBackgroundInfoPropertyClass ( const wxString& label, const wxString& name,
//    const wxToolObjectBackgroundInfo& value) : wxPGPropertyWithChildren(label,name)
//{
//    wxPG_INIT_REQUIRED_TYPE(wxToolObjectBackgroundInfo)
//    DoSetValue((void*)&value);
//    AddChild( wxBitmapSetInfoProperty(wxT("Normal"),wxPG_LABEL,value.m_NormalBitmapSetInfo) );
//    AddChild( wxBitmapSetInfoProperty(wxT("Hover"),wxPG_LABEL,value.m_HoverBitmapSetInfo) );
//    AddChild( wxBitmapSetInfoProperty(wxT("Selected"),wxPG_LABEL,value.m_SelectedBitmapSetInfo) );
//}
//
//wxToolObjectBackgroundInfoPropertyClass::~wxToolObjectBackgroundInfoPropertyClass () { }
//
//void wxToolObjectBackgroundInfoPropertyClass::DoSetValue ( wxPGVariant value )
//{
//    wxToolObjectBackgroundInfo* pObj = (wxToolObjectBackgroundInfo*)wxPGVariantToVoidPtr(value);
//    m_value = *pObj;
//    RefreshChildren();
//}
//
//wxPGVariant wxToolObjectBackgroundInfoPropertyClass::DoGetValue () const
//{
//    return wxPGVariant((void*)&m_value);
//}
//
//void wxToolObjectBackgroundInfoPropertyClass::RefreshChildren()
//{
//    if ( !GetCount() ) return;
//    Item(0)->DoSetValue( (void*) &m_value.m_NormalBitmapSetInfo );
//    Item(1)->DoSetValue( (void*) &m_value.m_HoverBitmapSetInfo );
//    Item(2)->DoSetValue( (void*) &m_value.m_SelectedBitmapSetInfo );
//}
//
//void wxToolObjectBackgroundInfoPropertyClass::ChildChanged ( wxPGProperty* p )
//{
//    switch ( p->GetIndexInParent() )
//    {
//        case 0: m_value.m_NormalBitmapSetInfo = *((wxBitmapSetInfo*) p->DoGetValue().GetVoidPtr()); break;
//        case 1: m_value.m_HoverBitmapSetInfo = *((wxBitmapSetInfo*) p->DoGetValue().GetVoidPtr()); break;
//        case 2: m_value.m_SelectedBitmapSetInfo = *((wxBitmapSetInfo*) p->DoGetValue().GetVoidPtr()); break;
//    }
//}
