#include "stdwx.h"
#include "wx/wxToolBox/wxToolBox.h"

IMPLEMENT_DYNAMIC_CLASS(wxToolBoxEvent, wxCommandEvent)

DEFINE_EVENT_TYPE( wxEVT_TB_TAB_SELECTED )
DEFINE_EVENT_TYPE( wxEVT_TB_ITEM_SELECTED )
DEFINE_EVENT_TYPE( wxEVT_TB_TAB_MOVED )
DEFINE_EVENT_TYPE( wxEVT_TB_TABS_SWAPPED )
DEFINE_EVENT_TYPE( wxEVT_TB_ITEM_MOVED )
DEFINE_EVENT_TYPE( wxEVT_TB_ITEMS_SWAPPED )
DEFINE_EVENT_TYPE( wxEVT_TB_TAB_ADDED )
DEFINE_EVENT_TYPE( wxEVT_TB_ITEM_ADDED )
DEFINE_EVENT_TYPE( wxEVT_TB_TAB_DELETED )
DEFINE_EVENT_TYPE( wxEVT_TB_ITEM_DELETED )
DEFINE_EVENT_TYPE( wxEVT_TB_SKIN_LOADED )
DEFINE_EVENT_TYPE( wxEVT_TB_SKIN_SAVED )
DEFINE_EVENT_TYPE( wxEVT_TB_STRUCTURE_LOADED )
DEFINE_EVENT_TYPE( wxEVT_TB_STRUCTURE_SAVED )

wxToolBoxEvent::wxToolBoxEvent(wxEventType commandType, int id)
: wxCommandEvent(commandType,id), m_Tab(NULL), m_Item(NULL),
m_ToolObjectIndex1(-1), m_ToolObjectIndex2(-1)
{
}

wxToolBoxEvent::wxToolBoxEvent(const wxToolBoxEvent & event)
: wxCommandEvent(event), m_Tab(event.m_Tab), m_Item(event.m_Item),
m_ToolObjectIndex1(event.m_ToolObjectIndex1), m_ToolObjectIndex2(event.m_ToolObjectIndex2)
{
}
