#ifndef _TOOLBOX_HOST_FRAME_H
#define _TOOLBOX_HOST_FRAME_H

#include "wxToolBoxSkinEditorHeaders.h"
#include "wx/wxToolBox/wxToolBox.h"


class ToolBoxHostFrame : public wxFrame
{	
public:
	wxToolBox * m_ToolBox;
	ToolBoxHostFrame(wxWindow * parent);
	DECLARE_EVENT_TABLE()
	void OnMove(wxMoveEvent & event);
	void OnClose(wxCloseEvent & event);	

	void OnToolBoxTabAdded(wxToolBoxEvent & event);
	void OnToolBoxTabSelected(wxToolBoxEvent & event);
	void OnToolBoxTabsSwapped(wxToolBoxEvent & event);
	void OnToolBoxTabDeleted(wxToolBoxEvent & event);
	
	void OnToolBoxItemAdded(wxToolBoxEvent & event);
	void OnToolBoxItemSelected(wxToolBoxEvent & event);
	void OnToolBoxItemsSwapped(wxToolBoxEvent & event);
	void OnToolBoxItemDeleted(wxToolBoxEvent & event);

	void OnToolBoxSkinLoaded(wxToolBoxEvent & event);
	void OnToolBoxSkinSaved(wxToolBoxEvent & event);
};

#endif
