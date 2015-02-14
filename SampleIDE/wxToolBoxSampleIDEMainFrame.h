#ifndef _WX_TOOLBOX_SAMPLE_IDE_MAINFRAME_H
#define _WX_TOOLBOX_SAMPLE_IDE_MAINFRAME_H

#include <wx/wxToolBox/wxToolBox.h>
#include <wx/wxFlatNotebook/wxFlatNotebook.h>

class wxToolBoxSampleIDEMainFrame : public wxFrame
{
	wxAuiManager m_FrameManager;
	wxToolBox * m_ToolBox;

	wxTreeCtrl * m_ClassViewTreeCtrl;
	wxTreeCtrl * m_ResourceViewTreeCtrl;
	wxTreeCtrl * m_SolutionViewTreeCtrl;
	wxFlatNotebook * m_NavigationNotebook;
	wxFlatNotebookImageList m_NavigationImageList;

	wxTextCtrl * m_OutputTextCtrl;
	wxListCtrl * m_TaskListCtrl;
	wxFlatNotebook * m_InfoNotebook;	

	wxTextCtrl * m_SourceEditor;
	wxPanel * m_GUIEditor;
	wxFlatNotebook * m_ContentNotebook;
	void CreateToolBox();
	void CreateNavigationNotebook();
	void CreateInfoNotebook();
	void CreateContentNotebook();

	bool GetPaneVisibility(wxString pane_name);
	void TogglePaneVisibility(wxString pane_name);
public:
	wxToolBoxSampleIDEMainFrame(wxWindow * parent, wxWindowID id = -1, wxString title = wxEmptyString);
	~wxToolBoxSampleIDEMainFrame();
	DECLARE_EVENT_TABLE();
	void OnToggleStatusBar(wxCommandEvent & event);
	void OnToggleToolBox(wxCommandEvent & event);
	void OnToggleNavigation(wxCommandEvent & event);
	void OnToggleInfo(wxCommandEvent & event);
	void OnToggleStandardToolBar(wxCommandEvent & event);
	void OnToggleProjectToolBar(wxCommandEvent & event);
	void OnOpen(wxCommandEvent & event);
	void OnSave(wxCommandEvent & event);
	void OnExit(wxCommandEvent & event);

	void OnToolNotepad(wxCommandEvent & event);

	void OnMenuItemUpdateUI(wxUpdateUIEvent & event);
};

#endif
