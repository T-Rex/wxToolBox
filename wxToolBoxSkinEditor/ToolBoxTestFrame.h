#ifndef _TOOLBOX_TEST_FRAME_H
#define _TOOLBOX_TEST_FRAME_H

#include "wx/wxToolBox/wxToolBox.h"

class ToolBoxTestFrame : public wxFrame
{	
	DECLARE_DYNAMIC_CLASS(ToolBoxTestFrame)
	wxPanel * m_Panel;	
	wxAuiManager m_FrameManager;
	wxPropertyGrid*  m_PropGrid;
	wxCheckBox * toggle_skin_check_box;	
	wxToolBox * m_ToolBox;
	void CreateToolBox(wxWindow * parent);
	void CreateMenuBar();	
	bool GetPaneVisibility(wxString pane_name);
	void TogglePaneVisibility(wxString pane_name);
public:
	wxTextCtrl * tab_name_text_ctrl;
	wxSpinCtrl * tab_icon_spin_ctrl;
	wxSpinCtrl * tab_index_spin_ctrl;	

	wxSpinCtrl * item_tab_spin_ctrl;
	wxSpinCtrl * item_icon_spin_ctrl;
	wxTextCtrl * item_name_text_ctrl;
	wxSpinCtrl * item_index_spin_ctrl;

	wxSpinCtrl * tab_index_1_spin_ctrl;
	wxSpinCtrl * tab_index_2_spin_ctrl;	

	wxSpinCtrl * item_index_1_spin_ctrl;
	wxSpinCtrl * item_index_2_spin_ctrl;

	wxButton * item_add_button;
	wxButton * item_add_and_edit_button;
	wxButton * item_rename_button;
	wxButton * item_delete_button;

	wxButton * tab_rename_button;
	wxButton * tab_delete_button;

	wxTextCtrl * m_LOGTextCtrl;
	int dx, dy;
	ToolBoxTestFrame();
	ToolBoxTestFrame(wxWindow * parent);
	~ToolBoxTestFrame();
	void CreateProperties();
	void SynchronizeProperties();
	DECLARE_EVENT_TABLE()	
	void OnPropertyChanged(wxPropertyGridEvent & event);
	void OnAddItem(wxCommandEvent & event);
	void OnAddAndEditItem(wxCommandEvent & event);
	void OnDeleteItem(wxCommandEvent & event);
	void OnSwapItems(wxCommandEvent & event);
	void OnRenameItem(wxCommandEvent & event);
	
	void OnAddTab(wxCommandEvent & event);
	void OnAddAndEditTab(wxCommandEvent & event);
	void OnDeleteTab(wxCommandEvent & event);
	void OnSwapTabs(wxCommandEvent & event);
	void OnRenameTab(wxCommandEvent & event);

	void OnToggleSkin(wxCommandEvent & event);
	void OnSaveSkin(wxCommandEvent & event);
	void OnLoadSkin(wxCommandEvent & event);
	void OnExit(wxCommandEvent & event);

	void OnToggleProperties(wxCommandEvent & event);
	void OnToggleToolBox(wxCommandEvent & event);
	void OnToggleLogWindow(wxCommandEvent & event);
	void OnToggleStatusBar(wxCommandEvent & event);	

	void OnTogglePropertiesUpdateUI(wxUpdateUIEvent & event);
	void OnToggleToolBoxUpdateUI(wxUpdateUIEvent & event);
	void OnToggleLogWindowUpdateUI(wxUpdateUIEvent & event);
	void OnToggleStatusBarUpdateUI(wxUpdateUIEvent & event);	

	// wxToolBox Event Handlers
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
