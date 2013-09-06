#include "wx/wxToolBox/wxToolBox.h"

wxToolObject::wxToolObject()
: m_Rectangle(0,0,0,0), m_ToolTip(wxEmptyString)
{
}

wxToolObject::wxToolObject(wxRect r)
: m_Rectangle(r), m_ToolTip(wxEmptyString)
{	
}

wxToolObject::wxToolObject(const wxToolObject & toolobject)
: m_Rectangle(toolobject.m_Rectangle), m_ToolTip(toolobject.m_ToolTip)
{
}
