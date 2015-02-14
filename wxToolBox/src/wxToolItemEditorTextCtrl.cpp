#include "stdwx.h"
#include "wx/wxToolBox/wxToolBox.h"

BEGIN_EVENT_TABLE(wxToolItemEditorTextCtrl, wxTextCtrl)

END_EVENT_TABLE()

wxToolItemEditorTextCtrl::wxToolItemEditorTextCtrl(wxToolBox * parent, wxToolBoxItem * item, 
												   wxPoint pos,wxSize size, long style)
		:wxTextCtrl(parent, -1, wxEmptyString, pos, size, style),
		m_Item(item)
{
	Connect(-1, wxEVT_COMMAND_TEXT_ENTER, 
                wxCommandEventHandler(wxToolItemEditorTextCtrl::OnEnterKeyPressed));
}

void wxToolItemEditorTextCtrl::OnEnterKeyPressed(wxCommandEvent & event)
{	
	m_Item->SetCaption(GetValue());
	Show(false);	
}
