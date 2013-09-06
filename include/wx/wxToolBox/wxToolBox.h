#ifndef _WX_TOOLBOX_H
#define _WX_TOOLBOX_H

#include <wx/wx.h>
#include <wx/dynarray.h>
#include <wx/timer.h>
#include <wx/xml/xml.h>

// ========================================================================
//  wxScrollDirection
// ------------------------------------------------------------------------
///	Scrolling direction
enum wxScrollDirection
{
	/// Scrolling left
	Left  = 3,
	/// Scrolling right
	Right = 4,
	/// Scrolling up
	Up    = 5,
	/// Scrolling down
	Down  = 6,
};

enum wxToolBoxTabStyle
{
	wxTB_STYLE_LIST,
	wxTB_STYLE_THUMBNAILS,
	wxTB_STYLE_ICONS
};

wxToolBoxTabStyle IntToToolBoxTabStyle(int value);

class wxToolBoxTab;
class wxToolBox;

// ========================================================================
//  wxToolObject
// ------------------------------------------------------------------------
///	Base class for all Tool Objects (tabs, items, buttons)
class WXDLLEXPORT wxToolObject
{
protected:
	/// Stores position and size of Tool Object
	wxRect m_Rectangle;
	/// Tooltip text
	wxString m_ToolTip;
public:
	/// Default constructor
	wxToolObject();
	/// Parametrized constructor
	/**
	\param r - rectangle that specifies position and size of Tool Object
	*/
	wxToolObject(wxRect r);
	/// Copy constructor
	wxToolObject(const wxToolObject & toolobject);
	/// Sets position and size of Tool Object
	void SetRectangle(wxRect value) {m_Rectangle = value;}
	/// Returns Tool Object's rectangle
	wxRect & GetRectangle() {return m_Rectangle;}
	/// Sets Tool Object's size
	/**
	\param w - new width
	\param h - new height
	*/
	void SetSize(int w, int h) {m_Rectangle.SetWidth(w); m_Rectangle.SetHeight(h);}
	/// Returns size of Tool Object
	wxSize GetSize() {return wxSize(m_Rectangle.GetWidth(), m_Rectangle.GetHeight());}	
	/// Sets tooltip text
	void SetToolTip(wxString value) {m_ToolTip = value;}
	/// Returns tooltip text
	wxString GetToolTip() {return m_ToolTip;}
};

// ========================================================================
//  wxToolBoxItem
// ------------------------------------------------------------------------
///	Class for storing information about toolbox items
class WXDLLEXPORT wxToolBoxItem : public wxToolObject
{
protected:
	wxWindowID m_ID;
	/// Parent component
	wxToolBox * m_Parent;
	/// Item's caption (label)
	wxString m_Caption;
	/// Item's image index
	int m_ImageIndex;
	/// Indicates if current item is selected
	bool m_Selected;
	/// Indicates if current item is enabled
	bool m_Enabled;
	/// Indicates if user pressed mouse button on current item
	bool m_MouseDown;
	/// Indcates if user moves mouse pointer over current item
	bool m_MouseHover;
	/// Indicates if dragging of current item is allowed (dragging is not implemented)
	bool m_AllowDrag;
	/// Indicates if user drags current item (dragging is not implemented)
	bool m_IsDragging;
	/// Indicates if current item is renamable
	bool m_Renamable;
	/// Indicates if current item is movable
	bool m_Movable;
	/// Indicates if current item is deletable
	bool m_Deletable;
	//wxObject m_Object;
	wxToolBoxTab * m_ParentTab;
	wxRect m_DragSafeRect;
public:
	/// Default constructor
	wxToolBoxItem();
	virtual ~wxToolBoxItem() {}
	/// Parametrized constructor
	/**
	\param caption - caption (label) of item
	\param id - ID of item
	\param imageindex - item's image index
	*/
	wxToolBoxItem(wxString caption, wxWindowID id = wxID_ANY, int imageindex = -1);
	/// Copy constructor
	wxToolBoxItem(const wxToolBoxItem & item);
	/// Equating operator
	wxToolBoxItem operator = (wxToolBoxItem item);
	/// Sets ID of tool item
	/**
	\param value - new ID of tool item
	*/
	void SetID(wxWindowID value) {m_ID = value;}
	/// Returns ID of tool item
	wxWindowID GetID() {return m_ID;}
	/// Sets parent control for item
	/**
	\param parent - pointer to new parent control
	*/
	void SetParent(wxToolBox * parent) {m_Parent = parent;}
	/// Returns pointer to parent control
	wxToolBox * GetParent() {return m_Parent;}
	/// Sets item's caption
	/**
	\param caption - new value of item's caption
	*/
	void SetCaption(wxString caption) {m_Caption = caption;}
	/// Returns item's caption
	wxString GetCaption() {return m_Caption;}
	/// Sets item's image index
	/**
	\param imageindex - new image index
	*/
	void SetImageIndex(int imageindex) {m_ImageIndex = imageindex;}
	/// Returns item's image index
	int GetImageIndex() {return m_ImageIndex;}
	/// Sets selection flag
	/**
	\param selected - new value of selection flag
	*/
	virtual void SetSelected(bool selected);
	/// Returns selection flag
	bool GetSelected() {return m_Selected;}
	/// Sets 'enabled' flag
	/**
	\param enabled - new value of 'enabled' flag
	*/
	virtual void SetEnabled(bool enabled) {m_Enabled = enabled;}
	/// Returns 'enabled' flag
	bool GetEnabled() {return m_Enabled;}
	/// Sets 'mouse down' flag
	/**
	\param mousedown - new value of 'mouse down' flag
	*/
	void SetMouseDown(bool mousedown) {m_MouseDown = mousedown;}
	/// Returns 'mouse down' flag
	bool GetMouseDown() {return m_MouseDown;}
	/// Sets 'mouse hover' flag
	/**
	\param mousehover - new value of 'mouse hover' flag
	*/
	void SetMouseHover(bool mousehover) {m_MouseHover = mousehover;}
	/// Returns 'mouse hover' flag
	bool GetMouseHover() {return m_MouseHover;}
	/// Sets 'allow dragging' flag (dragging is not implemented)
	/**
	\param allowdrag - new value of 'allow dragging' flag
	*/
	void SetAllowDrag(bool allowdrag) {m_AllowDrag = allowdrag;}
	/// Returns 'allow dragging' flag (dragging is not implemented)
	bool GetAllowDrag() {return m_AllowDrag;}
	/// Sets 'is dragging' flag (dragging is not implemented)
	/**
	\param dragging - new value of 'is dragging' flag
	*/
	void SetDragging(bool dragging) {m_IsDragging = dragging;}
	/// Returns 'is dragging' flag (dragging is not implemented)
	bool GetDragging() {return m_IsDragging;}
	/// Sets 'renamable' flag
	/**
	\param renamable - new value of 'renamable' flag
	*/
	void SetRenamable(bool renamable) {m_Renamable = renamable;}
	/// Returns 'renamable' flag
	bool GetRenamable() {return m_Renamable;}
	/// Sets 'movable' flag
	/**
	\param movable - new value of 'movable' flag
	*/
	void SetMovable(bool movable) {m_Movable = movable;}
	/// Returns 'movable' flag
	bool GetMovable() {return m_Movable;}
	/// Sets 'deletable' flag
	/**
	\param deletable - new value of 'deletable' flag
	*/
	void SetDeletable(bool deletable) {m_Deletable = deletable;}
	/// Returns 'deletable' flag
	bool GetDeletable() {return m_Deletable;}

/*	void SetObject(wxObject object) {m_Object = object;}
	wxObject GetObject() {return m_Object;}*/

	/// Sets parent tab
	/**
	\param parenttab - pointer to parent tab
	*/
	void SetParentTab(wxToolBoxTab * parenttab) {m_ParentTab = parenttab;}
	/// Returns pointer to parent tab
	wxToolBoxTab * GetParentTab() {return m_ParentTab;}

	/// Sets dragging rectangle (dragging is not implemented)
	void SetMouseDownLocation(wxPoint pt)
	{
		m_DragSafeRect.SetX(pt.x);
		m_DragSafeRect.SetY(pt.y);		
		m_DragSafeRect.SetSize(wxSize(wxSystemSettings::GetMetric(wxSYS_DRAG_X), 
									  wxSystemSettings::GetMetric(wxSYS_DRAG_Y)));
		m_DragSafeRect.Offset(-wxSystemSettings::GetMetric(wxSYS_DRAG_X)/2, 
							  -wxSystemSettings::GetMetric(wxSYS_DRAG_Y)/2);		
	}
	/// Returns true if given point is inside item's rectangle
	bool HitTest(wxPoint pt);	
	/// Cancells processing of mouse movement
	void CancelHover();
	/// Renames item
	void Rename();
	/// Returns true if item can be dragged from given point (dragging is not implemented)
	bool CanStartDrag(wxPoint pt);
	/// Invalidates item's rectangle
	void Invalidate();
};

WX_DECLARE_OBJARRAY(wxToolBoxItem, wxToolBoxItemArray);

// ========================================================================
//  wxToolBoxTab
// ------------------------------------------------------------------------
///	Contains information about toolbox tab, including array of toolbox items
class WXDLLEXPORT wxToolBoxTab : public wxToolBoxItem
{	
protected:
	/// Used for storing size of visible area of toolbox tab
	wxRect m_ItemArea;		
	/// Array of toolbox items
	wxToolBoxItemArray m_ToolItems;
	/// Index of an element above which cursor of the mouse is placed
	int m_HotItemIndex;
	/// Index of selected item
	int m_SelItemIndex;
	/// Index of previously selected item
	int m_OldItemIndex;
	/// New item will have this index
	int m_NewItemIndex;
	/// Index of first visible item
	int m_VisibleTopIndex;
	/// Index of last visible item
	int m_VisibleBottomIndex;	
	/// Tab style wxTB_TAB_LIST or wxTB_TAB_THUMBNAILS
	wxToolBoxTabStyle m_Style;
	/// Child window
	wxWindow * m_ChildWindow;
	/// Draws all items
	/**
	\param dc - device context to draw to
	\param clipRect - visible area
	*/
	void PaintItems(wxDC & dc, const wxRect & clipRect);
	/// Updates indexes of first and last visible item
	void UpdateItemLoopIndexes();
	/// Updates indexes of first and last visible item
	/**
	\param index - currently selected item
	\param bUpdates - flags
	*/
	void UpdateItemLoopIndexes(int index, bool * bUpdates);
	/// Updates parameters of new item
	/**
	\param index - index of new item
	*/
	void UpdateNewItem(unsigned int index);
	/// Scrolls items with specified offset
	void ScrollItemsO(int offset);
	/// Scrolls items with specified offset
	void ScrollItemsO(int offset, bool bRedraw);

	wxSize GetCurrentItemSize(int & maxWidth);
	wxPoint GetItemLocation(int index, int width, int height, int maxWidth);
public:	
	/// Constructor
	/**
	\param caption - tab caption
	\param id - ID of tab
	\param imageindex - index of associated image in component's image list
	*/
	wxToolBoxTab(wxString caption = wxEmptyString, wxWindowID id = wxID_ANY, int imageindex = -1);
	/// Destructor
	~wxToolBoxTab();
	/// Swaps the positions of two items
	/**
	\param index1 - first item index
	\param index2 - second item index
	*/
	bool SwapItems(unsigned int index1, unsigned int index2);
	/// Returns array of toolbox items
	wxToolBoxItemArray & GetToolItems() {return m_ToolItems;}
	/// Sets an area for displaying items (only items inside this area will be painted)
	/**
	\param itemarea - new area for displaying items
	*/
	void SetItemArea(wxRect itemarea) {m_ItemArea = itemarea;}
	/// Returns an area for displaying items
	wxRect GetItemArea() {return m_ItemArea;}
	/// Makes an item with specified index selected
	/**
	\param selitemindex - index of item to be selected
	*/
	void SetSelItemIndex(int selitemindex) {m_SelItemIndex = selitemindex;}
	/// Returns index of selected item
	int GetSelItemIndex() {return m_SelItemIndex;}
	/// Selects/deselects current tab
	/**
	\param selected - new value of selection flag
	*/
	virtual void SetSelected(bool selected);
	/// Sets current tab enabled
	/**
	\param enabled - new value of Enabled flag
	*/
	virtual void SetEnabled(bool enabled);
	/// Returns the amount of items in current tab
	size_t GetItemCount() {return m_ToolItems.Count();}

	/// Returns toolbox item with specified index. If illegal index specified then returns NULL.
	/**
	\param index - index of toolbox item on current tab
	*/
	wxToolBoxItem * operator [] (int index);	
	/// Returns index of item that contains specified point
	/**
	\param pt - point to be tested
	*/
	int HitTestItem(wxPoint pt);
	/// Makes item with specified index visible
	/**
	\param index - item index
	*/
	bool EnsureItemVisible(int index);
	/// Resets MouseHover flag of current hover item and resets HotItemIndex
	void CancelHotItemHover();

	/// Searches for hover item, sets MouseHover flag of this item and sets HotItemIndex
	void CheckMouseMoveForItems(wxMouseEvent & event);
	/// Returns true if item with specified index is visible. Otherwise returns false
	/**
	\param index - item index
	*/
	bool ItemVisible(unsigned int index);
	/// Returns true if specified item is visible. Otherwise returns false
	/**
	\param item - toolbox item
	*/
	bool ItemVisible(wxToolBoxItem * item);
	/// Returns true if tab contains specified item
	/**
	\param item - toolbox item
	*/
	bool Contains(wxToolBoxItem * item);	
	/// Returns true is scrolling to specified direction is possible
	/**
	\param scrollDir - scrolling direction
	*/
	bool CanScroll(wxScrollDirection scrollDir);
	/// Returns true if specified item can be moved upward
	/**
	\param item - toolbox item
	*/
	bool CanMoveItemUp(wxToolBoxItem * item);
	/// Returns true if specified item can be moved downward
	/**
	\param item - toolbox item
	*/
	bool CanMoveItemDown(wxToolBoxItem * item);
	/// Moves specified item upward
	/**
	\param item - toolbox item
	*/
	bool MoveItemUp(wxToolBoxItem * item);
	/// Moves specified item downward
	/**
	\param item - toolbox item
	*/
	bool MoveItemDown(wxToolBoxItem * item);
	/// Returns index of specified item. If current tab doesn't contain this item then returns -1;
	/**
	\param item - toolbox item
	*/
	int IndexOfItem(wxToolBoxItem * item);	
	/// Deletes item with specified index
	/**
	\param index - item index
	*/
	bool DeleteItem(int index);
	/// Deletes specified item
	/**
	\param item - toolbox item
	*/
	bool DeleteItem(wxToolBoxItem * item);	
	/// Scrolls items to specified direction
	/**
	\param scrollDir - scrolling direction
	*/
	bool ScrollItems(wxScrollDirection scrollDir);
	/// Scrolls items to specified direction
	/**
	\param scrollDir - scrolling direction
	\param bRedraw - if set to true then toolbox component should be repainted after scrolling
	*/
	bool ScrollItems(wxScrollDirection scrollDir, bool bRedraw);

	void ScrollTo(unsigned int index);
	/// Inserts new item to current toolbox tab
	/**
	\param item - item to be inserted
	*/
	int AddItem(wxToolBoxItem item);

	/// Updates rectangles of all items on current tab
	void UpdateItemRects(bool bRedraw);
	/// Updates rectangles of all items on current tab
	/**
	\param bUpdateXY - if set to TRUE then positions of items should be updated
	\param bUpdateSize - if set to TRUE then sizes of items should be updated
	\param bRedraw - if set to TRUE then items should be repainted
	*/
	void UpdateItemRects(bool bUpdateXY, bool bUpdateSize, bool bRedraw);

	/// Sets tab style (not used in current version)
	/**	
	\param value - new value of tab style
	\param bApply - if set to true then component will be repainted after changing the style
	*/
	void SetStyle(wxToolBoxTabStyle value, bool bApply = true);
	/// Returns tab style
	unsigned int GetStyle();

	/**
	\brief Returns the child windows associated with toolbox tab
	*/
	wxWindow * GetChildWindow();
	/**
	\brief Associates child window with toolbox tab and deletes window currently 
	associated with this tab
	\param window child window which should be associated with toolbox tab
	*/
	void SetChildWindow(wxWindow * window);
	/**
	\brief Detaches child window from tab
	*/
	wxWindow * DetachChildWindow();
	/**
	\brief Displays child window associated with toolbox tab. 
	You should never call this method manually.
	\return pointer to detached child window
	*/
	void ShowChildWindow();
	/**
	\brief Hides child window associated with toolbox tab. 
	You should never call this method manually.
	*/
	void HideChildWindow();

	void OnMouseDown(wxMouseEvent & event);
	void OnMouseUp(wxMouseEvent & event);	
	void OnMouseMove(wxMouseEvent & event);
	void OnPaint(wxDC & dc, wxRect r);
	bool bDebug;
	void DebugItems();
};

WX_DECLARE_LIST(wxToolBoxTab, wxToolBoxTabList);

// ========================================================================
//  wxToolItemEditorTextCtrl
// ------------------------------------------------------------------------
///	Text area for entering new labels of toolbox tabs and items
class WXDLLEXPORT wxToolItemEditorTextCtrl : public wxTextCtrl
{
	/// Pointer to current toolbox object
	wxToolBoxItem * m_Item;
public:
	/// Constructor
	/**
	\param parent - parent component
	\param item - current toolbox object (wxToolBoxItem or wxToolBoxTab)
	\param pos - position
	\param size - size of text area
	\param style - window style
	*/
	wxToolItemEditorTextCtrl(wxToolBox * parent, wxToolBoxItem * item = NULL, wxPoint pos = wxDefaultPosition,
		wxSize size = wxDefaultSize, long style = wxSIMPLE_BORDER);
	/// Sets current toolbox object
	/**
	\param item - pointer to current toolbox object
	*/
	void SetItem(wxToolBoxItem * item) {m_Item = item;}
	/// Returns pointer to current toolbox object (associated with text control)
	wxToolBoxItem * GetItem() {return m_Item;}
	DECLARE_EVENT_TABLE()
	void OnEnterKeyPressed(wxCommandEvent & event);
};

// ========================================================================
//  wxToolBoxScrollButton
// ------------------------------------------------------------------------
///	Stores information about toolbox button objects (toolbox has 2 scroll buttons - for upward and downward scrolling)
class WXDLLEXPORT wxToolBoxScrollButton : public wxToolObject
{
	/// Indicates if mouse button was pressed on button
	bool m_MouseDown;
	/// Indicates if user moves mouse pointer above button
	bool m_MouseHover;
	/// Indicates if button is enabled
	bool m_Enabled;
	/// Indicates scroll direction
	wxScrollDirection m_Direction;
	/// Pointer to parent component
	wxToolBox * m_Parent;
public:	
	/// Constructor
	/**
	\param parent - pointer to parent component
	\param dir - direction
	\param size - button size
	*/
	wxToolBoxScrollButton(wxToolBox * parent, wxScrollDirection dir, wxSize size);

	/**
	\brief Sets MouseDown flag of button
	\param value - new value of MouseDown flag
	*/
	void SetMouseDown(bool value) {m_MouseDown = value;}
	/// Returns MouseDown flag of button
	bool GetMouseDown() {return m_MouseDown;}
	/**
	\brief Sets MouseHover flag of button
	\param value - new value of MouseHover flag
	*/
	void SetMouseHover(bool value) {m_MouseHover = value;}
	/// Returns MouseHover flag of button
	bool GetMouseHover() {return m_MouseHover;}
	/**
	\brief Enables/disables button
	\param value - new value of MouseHover flag
	*/
	void SetEnabled(bool value) {m_Enabled = value;}
	/// Returns true if toolbox button is enabled, otherwise returns false
	bool GetEnabled() {return m_Enabled;}
	/// Sets X coordinate of toolbox button
	/**
	\param x - new value of X coordinate
	*/
	void SetX(int x) {m_Rectangle.SetX(x);}
	/// Sets Y coordinate of toolbox button
	/**
	\param y - new value of Y coordinate
	*/
	void SetY(int y) {m_Rectangle.SetY(y);}
	/**
	\brief Sets scrolling direction of toolbox button
	\param value - new value of scrolling direction
	*/
	void SetScrollDirection(wxScrollDirection value) {m_Direction = value;}
	/// Returns scrolling direction
	wxScrollDirection GetSrollDirection() {return m_Direction;}	
	/**
	\brief Draws button on specified device context
	\param dc - device context to draw to
	\param clipRect - clipping rectangle
	*/
	void Paint(wxDC & dc, const wxRect & clipRect);	
	/**
	\brief Returns true if button's rectangle contains the point with specified coordinates
	\param x - X coordinate of point
	\param y - Y coordinate of point
	*/
	bool HitTest(wxCoord x, wxCoord y);
	/// Repaints button
	void Invalidate();
};

// ========================================================================
//  wxToolBoxBitmapSet
// ------------------------------------------------------------------------
///	Used for storing set of 3 background bitmaps for toolbox objects
class WXDLLEXPORT wxToolBoxBitmapSet
{
protected:
	/// Path to image file of left background bitmap
	wxString m_LeftPath;
	/// Path to image file of middle background bitmap
	wxString m_MiddlePath;
	/// Path to image file of right background bitmap
	wxString m_RightPath;
	/// Left background bitmap
	wxBitmap m_LeftBitmap;
	/// Middle background bitmap
	wxBitmap m_MiddleBitmap;
	/// Right background bitmap
	wxBitmap m_RightBitmap;
public:
	/// Default constructor
	wxToolBoxBitmapSet() {}
	/// Parametrized constructor
	/**
	\param left - normal background
	\param middle - hover background
	\param right - selected background
	*/
	wxToolBoxBitmapSet(wxString left, wxString middle, wxString right);
	/// Copy constructor
	wxToolBoxBitmapSet(const wxToolBoxBitmapSet & bmpset);
	/// Sets left background
	/**
	\param bitmap - new left background
	*/
	void SetLeftBitmap(wxString bitmap);
	/// Returns left background
	wxBitmap & GetLeftBitmap() {return m_LeftBitmap;}
	/// Returns path from which left bitmap has been loaded
	wxString GetLeftPath() {return m_LeftPath;}
	/// Sets hover background
	/**
	\param bitmap - new hover background
	*/
	void SetMiddleBitmap(wxString bitmap);
	/// Returns hover background
	wxBitmap & GetMiddleBitmap() {return m_MiddleBitmap;}
	/// Returns path from which middle bitmap has been loaded
	wxString GetMiddlePath() {return m_MiddlePath;}
	/// Sets selected background
	/**
	\param bitmap - new selected background
	*/
	void SetRightBitmap(wxString bitmap);
	/// Returns selected background
	wxBitmap & GetRightBitmap() {return m_RightBitmap;}
	/// Returns path from which right bitmap has been loaded
	wxString GetRightPath() {return m_RightPath;}
	/// Equating operator
	wxToolBoxBitmapSet operator = (const wxToolBoxBitmapSet bmpset);
	/// Returns maximal height of background bitmaps
	unsigned int GetMaxBitmapHeight();
};

// ========================================================================
//  wxToolObjectBackground
// ------------------------------------------------------------------------
///	Used for storing 3 sets of background bitmaps for toolbox objects
class WXDLLEXPORT wxToolObjectBackground
{
	/// Normal background
	wxToolBoxBitmapSet m_NormalBackground;
	/// Hover background
	wxToolBoxBitmapSet m_HoverBackground;
	/// Selected background
	wxToolBoxBitmapSet m_SelectedBackground;
public:
	/// Default constructor
	wxToolObjectBackground() {}
	/// Parametrized constructor
	/**
	\param normal - normal background
	\param hover - hover background
	\param selected - selected background
	*/
	wxToolObjectBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected);
	/// Sets normal background
	/**
	\param background - new normal background
	*/
	void SetNormalBackground(wxToolBoxBitmapSet & background) {m_NormalBackground = background;}
	/// Returns normal background
	wxToolBoxBitmapSet & GetNormalBackground() {return m_NormalBackground;}
	/// Sets hover background
	/**
	\param background - new hover background
	*/
	void SetHoverBackground(wxToolBoxBitmapSet & background) {m_HoverBackground = background;}
	/// Returns hover background
	wxToolBoxBitmapSet & GetHoverBackground() {return m_HoverBackground;}
	/// Sets selected background
	/**
	\param background - new selected background
	*/
	void SetSelectedBackground(wxToolBoxBitmapSet & background) {m_SelectedBackground = background;}
	/// Returns selected background
	wxToolBoxBitmapSet & GetSelectedBackground() {return m_SelectedBackground;}
	/// Returns maximal height of background bitmaps
	unsigned int GetMaxBitmapHeight();

};

WX_DECLARE_OBJARRAY(wxBitmap, wxToolBoxIconArray);

// ========================================================================
//  wxToolBox
// ------------------------------------------------------------------------
/** 
	\brief Control which allows you to incorporate VS.NET-like toolbox or Outlook-like side bar
	into your applications. 
*/
class WXDLLEXPORT wxToolBox : public wxControl
{		
	void LoadToolItemParameters(wxToolBoxItem * item, wxXmlNode * node);
	void SaveToolItemParameters(wxToolBoxItem * item, wxXmlNode * node);
	DECLARE_DYNAMIC_CLASS(wxToolBox)
	int m_Style;
protected:	
	wxBitmap m_CanvasBitmap;
	//wxMemoryDC mdc;
	/// Array of tabs
	wxToolBoxTabList m_Tabs;

	/// Selected tab
	wxToolBoxTab * m_SelectedTab;
	/// Previously selected tab
	wxToolBoxTab * m_OldSelectedTab;

	/// Path to image file from which TabImageList bitmaps have been created
	wxString m_TabImageListBitmapPath;
	/// Path to image file from which ItemImageList bitmaps have been created
	wxString m_ItemImageListBitmapPath;
	/// ImageList that contains icons for toolbox tabs
	wxToolBoxIconArray m_TabImageList;
	/// ImageList that contains icons for toolbox items	
	wxToolBoxIconArray m_ItemImageList;

	/// Normal colour of toolbox item
	wxColour m_ItemNormalColour;
	/// This colour is used when you move mouse pointer over toolbox item
	wxColour m_ItemHoverColour;
	/// Colour of selected toolbox item
	wxColour m_ItemSelectedColour;

	/// Normal colour of toolbox tab
	wxColour m_TabNormalColour;
	/// This colour is used when you move mouse pointer over toolbox tab
	wxColour m_TabHoverColour;
	/// Colour of selected tab
	wxColour m_TabSelectedColour;

	/// Normal colour of scroll buttons
	wxColour m_ButtonNormalColour;
	/// Hover colour of scroll buttons
	wxColour m_ButtonHoverColour;
	/// Colour of pressed scroll button
	wxColour m_ButtonSelectedColour;

	/// Width of tab icons
	unsigned int m_TabImageWidth;
	/// Height of tab icons
	unsigned int m_TabImageHeight;

	/// Width of item icons
	unsigned int m_ItemImageWidth;
	/// Height of item icons
	unsigned int m_ItemImageHeight;

	/// Height of tab header
	unsigned int m_TabHeight;
	/// Height of toolbox item
	unsigned int m_ItemHeight;
	/// Icon size
	unsigned int m_IconSize;

	/// Height of free space between toolbox tabs
	unsigned int m_TabSpacing;
	/// Height of free space between toolbox items
	unsigned int m_ItemSpacing;

	/// Width of free space between tab border and tab icon/caption
	unsigned int m_TabCaptionSpacing;
	/// Width of free space between item border and item icon/caption
	unsigned int m_ItemCaptionSpacing;

	unsigned int m_ScrollWait;
	unsigned int m_ScrollDelay;
	
	/// Edit box for editing labels of toolbox items and tabs
	wxToolItemEditorTextCtrl * m_TextBox;
	/// Scroll button for scrolling active tab upward
	wxToolBoxScrollButton *m_UpScroll;
	/// Scroll button for scrolling active tab downward
	wxToolBoxScrollButton * m_DownScroll;
	
	/// Indicates if background image should be painted
	bool m_UseBackground;
	/// Component's background
	wxBitmap m_ToolBoxBackground;
	/// Path to image file from which toolbox background bitmap has been loaded
	wxString m_ToolBoxBackgroundPath;
	/// Indicates if tab background should be painted
	bool m_UseTabBackground;
	/// Set of background bitmaps for toolbox tabs
	wxToolObjectBackground m_TabBackground;
	/// Indicates if item normal background should be painted
	bool m_UseItemNormalBackground;
	/// Indicates if item hover background should be painted
	bool m_UseItemHoverBackground;
	/// Indicates if item selected background should be painted
	bool m_UseItemSelectedBackground;
	/// Set of background bitmaps for toolbox items
	wxToolObjectBackground m_ItemBackground;
	/// Indicates if button background should be painted
	bool m_UseButtonBackground;
	/// Indicates if component should use standard arrows for painting scroll buttons
	bool m_UseStdArrows;
	/// Set of background bitmaps for toolbox buttons
	wxToolObjectBackground m_ButtonBackground;
	
	/// Indicates if toolbox items should be transparent (normal and hover)
	bool m_ItemsTransparent;
	/// Indicates if selected toolbox item should be transparent
	bool m_SelectedItemTransparent;

	wxFont m_TabNormalFont;
	wxFont m_TabHoverFont;
	wxFont m_TabSelectedFont;
	
	wxFont m_ItemNormalFont;
	wxFont m_ItemHoverFont;
	wxFont m_ItemSelectedFont;
	
	wxColour m_ItemTextColour;
	wxColour m_TabTextColour;

	bool m_MouseMoveFreezed;
	bool m_LayoutTimerActive;

	bool m_SelectNewTab;
	bool m_ShowSelectionAlways;

	/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool m_PreDrawBackgrounds;
	
	wxTimer * m_LayoutTimer;

	bool OnRenameFinished(bool bCancelled);
	void UpdateItemFromTextBox();
	/// Applies layout settings (height and spaces)
	void DoLayout(bool bInitial, bool bUpdateItems, bool bRepaint);
	bool DoTimedLayout(wxToolBoxTab * oldSelectedTab);

	wxBitmap m_TabNormalBG;
	wxBitmap m_TabHoverBG;
	wxBitmap m_TabSelectedBG;
	wxBitmap m_TabNormalBGSel;
	wxBitmap m_TabHoverBGSel;
	wxBitmap m_TabSelectedBGSel;
	
	wxBitmap m_ItemNormalBG;
	wxBitmap m_ItemHoverBG;
	wxBitmap m_ItemSelectedBG;	

	/// This pen is used for painting highlighted part of bevels
	wxPen m_BorderHighlightPen;
	/// This pen is used for painting shadowed part of bevels
	wxPen m_BorderShadowPen;

	void PreDrawBackgrounds();
	void PreDrawItemBackground();
	void DrawBG(wxDC & dc, wxBitmap * left_bmp, wxBitmap * middle_bmp, wxBitmap * right_bmp,
		int left, int top);

	/// Creates ImageList with images with specified size from bitmap
	/**
	\param image_list - destination image list
	\param bmp - source bitmap
	\param width - new width of images in ImageList
	\param height - new height of images in ImageList
	\param clear_existing - indicates if existing images should be removed from ImageList before adding new images;
	*/
	void CreateImageListFromBitmap(wxToolBoxIconArray & image_list, wxBitmap & bmp, 
		int width, int height, bool clear_existing = false);
public:
	/// Default constructor
	wxToolBox() {};
	/// Constructor
	/**
	\param parent - parent window
	\param id - window ID
	\param pos - control's position
	\param size - contol's size
	\param style - window style
	*/
	wxToolBox(wxWindow * parent, wxWindowID id = -1, wxPoint pos = wxDefaultPosition, 
		wxSize size = wxDefaultSize, long style = 0);
	/// Destructor
	~wxToolBox();

	/// Returns array of toolbox tabs
	wxToolBoxTabList & GetTabs() {return m_Tabs;}

	/// Returns toolbox tab with specified index. If function fails then returns NULL
	wxToolBoxTab * GetTab(unsigned int index) {return (*this)[index];}

	/// Returns the amount of tabs
	unsigned int GetTabCount() {return (unsigned int)m_Tabs.GetCount();}	

	/// Returns TabImageList that contains item icons
	wxToolBoxIconArray & GetTabImageList() {return m_TabImageList;}
	/// Returns ItemImageList that contains item icons
	wxToolBoxIconArray & GetItemImageList() {return m_ItemImageList;}
	/// Returns selected tab
	wxToolBoxTab * GetSelectedTab() {return m_SelectedTab;}

	/// Sets the normal colour of toolbox items
	/**
	\param value - new colour of toolbox items
	*/
	void SetItemNormalColour(wxColour value) {m_ItemNormalColour = value; Refresh();}
	/// Returns normal colour of toolbox items
	const wxColour & GetItemNormalColour() {return m_ItemNormalColour;}
	/// Sets hover colour of toolbox items
	/**
	\param value - new hover colour of toolbox items
	*/
	void SetItemHoverColour(wxColour value) {m_ItemHoverColour = value; Refresh();}
	/// Returns hover colour of toolbox items
	const wxColour & GetItemHoverColour() {return m_ItemHoverColour;}
	/// Sets colour of selected item
	/**
	\param value - new colour of selected toolbox item
	*/
	void SetItemSelectedColour(wxColour value) {m_ItemSelectedColour = value; Refresh();}
	/// Returns colour of selected toolbox item
	const wxColour & GetItemSelectedColour() {return m_ItemSelectedColour;}

	/// Sets the normal colour of toolbox tab
	/**
	\param value - new colour of toolbox tab
	*/
	void SetTabNormalColour(wxColour value) {m_TabNormalColour = value; Refresh();}
	/// Returns normal colour of toolbox items
	const wxColour & GetTabNormalColour() {return m_TabNormalColour;}
	/// Sets hover colour of toolbox tab
	/**
	\param value - new hover colour of toolbox tab
	*/
	void SetTabHoverColour(wxColour value) {m_TabHoverColour = value; Refresh();}
	/// Returns hover colour of toolbox tab
	const wxColour & GetTabHoverColour() {return m_TabHoverColour;}
	/// Sets colour of selected tab
	/**
	\param value - new colour of selected toolbox tab
	*/
	void SetTabSelectedColour(wxColour value) {m_TabSelectedColour = value; Refresh();}
	/// Returns colour of selected toolbox tab
	const wxColour & GetTabSelectedColour() {return m_TabSelectedColour;}

	/// Sets the normal colour of toolbox button
	/**
	\param value - new colour of toolbox tab
	*/
	void SetButtonNormalColour(wxColour value) {m_ButtonNormalColour = value; Refresh();}
	/// Returns normal colour of toolbox button
	const wxColour & GetButtonNormalColour() {return m_ButtonNormalColour;}
	/// Sets hover colour of toolbox button
	/**
	\param value - new hover colour of toolbox tab
	*/
	void SetButtonHoverColour(wxColour value) {m_ButtonHoverColour = value; Refresh();}
	/// Returns hover colour of toolbox button
	const wxColour & GetButtonHoverColour() {return m_ButtonHoverColour;}
	/// Sets colour of pressed toolbox button
	/**
	\param value - new colour of selected toolbox tab
	*/
	void SetButtonSelectedColour(wxColour value) {m_ButtonSelectedColour = value; Refresh();}
	/// Returns colour of pressed toolbox button
	const wxColour & GetButtonSelectedColour() {return m_ButtonSelectedColour;}


	/// Sets horizontal size for tab icons
	/**
	\param value - new horizontal size of tab icons
	*/
	void SetTabImageWidth(unsigned int value) {m_TabImageWidth = value;}	
	/// Returns horizontal size of tab icons
	unsigned int GetTabImageWidth() {return m_TabImageWidth;}

	/// Sets vertical size for tab icons
	/**
	\param value - new vertical size of tab icons
	*/
	void SetTabImageHeight(unsigned int value) {m_TabImageHeight = value;}
	/// Returns vertical size of tab icons
	unsigned int GetTabImageHeight() {return m_TabImageHeight;}


	/// Sets horizontal size for item icons
	/**
	\param value - new horizontal size of item icons
	*/
	void SetItemImageWidth(unsigned int value) {m_ItemImageWidth = value;}	
	/// Returns horizontal size of item icons
	unsigned int GetItemImageWidth() {return m_ItemImageWidth;}

	/// Sets vertical size for item icons
	/**
	\param value - new vertical size of item icons
	*/
	void SetItemImageHeight(unsigned int value) {m_ItemImageHeight = value;}
	/// Returns vertical size of item icons
	unsigned int GetItemImageHeight() {return m_ItemImageHeight;}

	///Sets height of toolbox tabs
	/**
	\param value - new height of toolbox tabs
	*/
	void SetTabHeight(unsigned int value) {m_TabHeight = value; DoLayout(false, false, false); Refresh();}
	/// Returns height of toolbox tabs
	unsigned int GetTabHeight() {return m_TabHeight;}

	///Sets height of toolbox items
	/**
	\param value - new height of toolbox items
	*/
	void SetItemHeight(unsigned int value) {m_ItemHeight = value; DoLayout(false, true, false); Refresh();}
	/// Returns height of toolbox items
	unsigned int GetItemHeight() {return m_ItemHeight;}

	///Sets height of free space between toolbox tabs
	/**
	\param value - new height of free space between toolbox tabs
	*/
	void SetTabSpacing(unsigned int value) {m_TabSpacing = value; DoLayout(false, false, false); Refresh();}
	/// Returns height of free space between toolbox tabs
	unsigned int GetTabSpacing() {return m_TabSpacing;}

	///Sets height of free space between toolbox items
	/**
	\param value - new height of free space between toolbox items
	*/
	void SetItemSpacing(unsigned int value) {m_ItemSpacing = value; DoLayout(false, true, false); Refresh();}
	/// Returns height of free space between toolbox items
	unsigned int GetItemSpacing() {return m_ItemSpacing;}

	/// Sets the width between tab border and icon/caption
	/**
	\param value - new value of width between tab border and icon/caption
	*/
	void SetTabCaptionSpacing(unsigned int value) {m_TabCaptionSpacing = value; Refresh();}
	/// Returns the width between tab border and tab icon/caption
	unsigned int GetTabCaptionSpacing() {return m_TabCaptionSpacing;}

	/// Sets the width between item border and icon/caption
	/**
	\param value - new value of width between item border and icon/caption
	*/
	void SetItemCaptionSpacing(unsigned int value) {m_ItemCaptionSpacing = value; Refresh();}
	/// Returns the width between item border and icon/caption
	unsigned int GetItemCaptionSpacing() {return m_ItemCaptionSpacing;}

	void SetScrollWait(unsigned int value) {m_ScrollWait = value;}
	unsigned int GetScrollWait() {return m_ScrollWait;}

	void SetScrollDelay(unsigned int value) {m_ScrollDelay = value;}
	unsigned int GetScrollDelay() {return m_ScrollDelay;}

	/// Draws 3D border on specified rectangle
	/**
	\param dc - device context to draw to
	\param rect - destination rectangle
	\param bSelected - indicates if function should draw normal 3D border or "pressed" 3D border
	*/
	void DrawBorders(wxDC & dc, const wxRect & rect, bool bSelected);

	/// Returns toolbox object (tab or item) currently associated with caption editor textbox
	wxToolBoxItem * GetEditingItem();

	/// Returns pointer to top scroll button
	wxToolBoxScrollButton * GetUpScroll() {return m_UpScroll;}
	/// Returns pointer to bottom scroll button
	wxToolBoxScrollButton * GetDownScroll() {return m_DownScroll;}

	bool EndRenameItem(bool bSave);
	/// Refreshes contents and updates positions of toolbox tabs and toolbox items
	void RefreshTabs();

	/// Adds new toolbox tab and generates wxEVT_TB_TAB_ADDED event.
	/**
	\param tab - new toolbox tab	
	*/
	int AddTab(wxToolBoxTab * tab);
	/// Deletes toolbox tab with specified index and generates wxEVT_TB_TAB_DELETED event.
	/**
	\param index - index of toolbox tab to be deleted
	*/
	bool DeleteTab(int index);
	/// Deletes specified toolbox tab and generates wxEVT_TB_TAB_DELETED event.
	/**
	\param tab - toolbox tab to be deleted
	*/
	bool DeleteTab(wxToolBoxTab * tab) {return DeleteTab(IndexOf(tab));}

	/// Updates component's ToolTip text
	/**
	\param item - toolbox object which tooltip string should become component's tooltip string
	*/
	void UpdateToolTip(wxToolObject * item);
	void OnTabSelectionChanged(wxToolBoxTab * newtab, wxToolBoxTab * oldtab);
	void OnItemSelectionChanged(wxToolBoxItem * item, wxToolBoxTab * tab) {}
	void OnTabMouseDown(wxToolBoxTab * tab, wxMouseEvent & event);
	void OnTabMouseUp(wxToolBoxTab * tab, wxMouseEvent & event) {}
	void OnItemMouseUp(wxToolBoxItem * item, wxToolBoxTab * tab, wxMouseEvent & event) {}
	void LockMouseMove() {m_MouseMoveFreezed = true;}
	void UnlockMouseMove() {m_MouseMoveFreezed = false;}

	/// Sets path to image file from which TabImageList bitmaps should be created and creates TabImageList
	/**
	\param filename - path to image file from which TabImageList bitmaps should be created
	\param image_width - new width of tab icons
	\param image_height - new height of tab icons
	\param clear_existing - determines if existing TabImageList should be cleared
	*/
	void CreateTabImageList(wxString filename, int image_width, int image_height, bool clear_existing = false);
	/// Returns path to image file from which ImageList bitmaps should be created
	const wxString & GetTabImageListBitmapPath() {return m_TabImageListBitmapPath;}	

	/// Sets path to image file from which ItemImageList bitmaps should be created and creates ItemImageList
	/**
	\param filename - path to image file from which ItemImageList bitmaps should be created
	\param image_width - new width of icons
	\param image_height - new height of icons
	\param clear_existing - determines if existing ItemImageList should be cleared
	*/
	void CreateItemImageList(wxString filename, int image_width, int image_height, bool clear_existing = false);
	/// Returns path to image file from which ImageList bitmaps should be created
	const wxString & GetItemImageListBitmapPath() {return m_ItemImageListBitmapPath;}	

	/// Sets component's background
	/**
	\param background - new background image
	*/
	void SetBackground(wxString background);
	/// Returns component's background bitmap
	const wxBitmap & GetBackground();
	/// Returns path to image file from which background bitmap has been loaded
	wxString GetBackgroundPath() {return m_ToolBoxBackgroundPath;};

	/// Determines if background image should be painted (sets UseBackground flag)
	/**
	\param value - new value of UseBackground flag
	*/
	void SetUseBackground(bool value) {m_UseBackground = value; Refresh();}
	/// Returns UseBackground flag
	bool GetUseBackground() {return m_UseBackground;}

	/// Sets item background images
	/**
	\param normal - normal item background
	\param hover - hover item background
	\param selected - selected item background
	*/
	void SetItemBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected);
	/// Returns item background
	wxToolObjectBackground & GetItemBackground() {return m_ItemBackground;}
	/// Sets normal item background bitmap
	/**
	\param background - new normal item background bitmap
	*/
	void SetNormalItemBackground(wxToolBoxBitmapSet & background);
	/// Sets hover item background bitmap
	/**
	\param background - new hover item background bitmap
	*/
	void SetHoverItemBackground(wxToolBoxBitmapSet & background);
	/// Sets selected item background bitmap
	/**
	\param background - new selected item background bitmap
	*/
	void SetSelectedlItemBackground(wxToolBoxBitmapSet & background);
	/// Determines if item normal background should be painted (sets UseItemNormalBackground flag)
	/**
	\param value - new value of UseItemNormalBackground flag
	*/
	void SetUseItemNormalBackground(bool value) {m_UseItemNormalBackground = value; Refresh();}
	/// Returns UseItemNormalBackground flag
	bool GetUseItemNormalBackground() {return m_UseItemNormalBackground;}
	/// Determines if item hover background should be painted (sets UseItemHoverBackground flag)
	/**
	\param value - new value of UseItemHoverBackground flag
	*/
	void SetUseItemHoverBackground(bool value) {m_UseItemHoverBackground = value; Refresh();}
	/// Returns UseItemHoverBackground flag
	bool GetUseItemHoverBackground() {return m_UseItemHoverBackground;}
	/// Determines if item selected background should be painted (sets UseItemSelectedBackground flag)
	/**
	\param value - new value of UseItemSelectedBackground flag
	*/
	void SetUseItemSelectedBackground(bool value) {m_UseItemSelectedBackground = value; Refresh();}
	/// Returns UseItemSelectedBackground flag
	bool GetUseItemSelectedBackground() {return m_UseItemSelectedBackground;}
	/// Sets UseItemNormalBackground, UseItemHoverBackground and UseItemSelectedBackground flag
	/**
	\param value - new value of item background flags
	*/
	void SetUseItemBackground(bool value);

	/// Sets tab background images
	/**
	\param normal - normal tab background
	\param hover - hover tab background
	\param selected - selected tab background
	*/
	void SetTabBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected);
	/// Returns tab background
	wxToolObjectBackground & GetTabBackground() {return m_TabBackground;}
	/// Sets normal tab background bitmap
	/**
	\param background - new normal tab background bitmap
	*/
	void SetNormalTabBackground(wxToolBoxBitmapSet & background);
	/// Sets hover tab background bitmap
	/**
	\param background - new hover tab background bitmap
	*/
	void SetHoverTabBackground(wxToolBoxBitmapSet & background);
	/// Sets selected tab background bitmap
	/**
	\param background - new selected tab background bitmap
	*/
	void SetSelectedlTabBackground(wxToolBoxBitmapSet & background);
	/// Determines if tab background should be painted (sets UseTabBackground flag)
	/**
	\param value - new value of UseTabBackground flag
	*/
	void SetUseTabBackground(bool value) {m_UseTabBackground = value; Refresh();}
	/// Returns UseTabBackground flag
	bool GetUseTabBackground() {return m_UseTabBackground;}

	/// Sets button background images
	/**
	\param normal - normal button background
	\param hover - hover button background
	\param selected - selected tab background
	*/
	void SetButtonBackground(wxToolBoxBitmapSet & normal, wxToolBoxBitmapSet & hover, wxToolBoxBitmapSet & selected);
	/// Returns button background
	wxToolObjectBackground & GetButtonBackground() {return m_ButtonBackground;}
	/// Sets normal button background bitmap
	/**
	\param background - new normal button background bitmap
	*/
	void SetNormalButtonBackground(wxToolBoxBitmapSet & background);
	/// Sets hover button background bitmap
	/**
	\param background - new hover button background bitmap
	*/
	void SetHoverButtonBackground(wxToolBoxBitmapSet & background);
	/// Sets selected button background bitmap
	/**
	\param background - new selected button background bitmap
	*/
	void SetSelectedlButtonBackground(wxToolBoxBitmapSet & background);
	/// Determines if button background should be painted (sets UseTabBackground flag)
	/**
	\param value - new value of UseButtonBackground flag
	*/
	void SetUseButtonBackground(bool value) {m_UseButtonBackground = value; Refresh();}
	/// Returns UseTabBackground flag
	bool GetUseButtonBackground() {return m_UseButtonBackground;}
	/// Determines if component should use standard arrows for painting scroll buttons
	/**
	\param value - new value of UseStdArrows flag
	*/
	void SetUseStdArrows(bool value) {m_UseStdArrows = value; Refresh();}
	/// Returns UseTabBackground flag
	bool GetUseStdArrows() {return m_UseStdArrows;}

	/// Sets the flag that indicates if toolbox items (normal and hover) should be transparent
	/**
	\param value - new value of transparency
	*/
	void SetItemsTransparent(bool value) {m_ItemsTransparent = value; Refresh();}
	/// Returns transparecy of toolbox items
	bool GetItemsTransparent() {return m_ItemsTransparent;}
	/// Sets the flag that indicates if selected toolbox item should be transparent
	/**
	\param value - new value of transparency of selected item
	*/
	void SetSelectedItemTransparent(bool value) {m_SelectedItemTransparent = value; Refresh();}
	/// Returns transparecy of selected toolbox item
	bool GetSelectedItemTransparent() {return m_SelectedItemTransparent;}

	/// Sets the colour of toolbox item caption
	/**
	\param value - new colour of toolbox item caption
	*/
	void SetItemTextColour(wxColour value) {m_ItemTextColour = value; Refresh();}
	/// Returns the colour of toolbox item caption
	const wxColour & GetItemTextColour() {return m_ItemTextColour;}

	/// Sets the colour of toolbox tab caption
	/**
	\param value - new colour of toolbox tab caption
	*/
	void SetTabTextColour(wxColour value) {m_TabTextColour = value; Refresh();}
	/// Returns the colour of toolbox tab caption
	const wxColour & GetTabTextColour() {return m_TabTextColour;}

	/// Sets normal font of toolbox item
	/**
	\param font - new normal font of toolbox item
	*/
	void SetItemNormalFont(wxFont & font) {m_ItemNormalFont = font; Refresh();};
	/// Returns normal font of toolbox item
	const wxFont & GetItemNormalFont() {return m_ItemNormalFont;}
	/// Sets hover font of toolbox item
	/**
	\param font - new normal font of toolbox item
	*/
	void SetItemHoverFont(wxFont & font) {m_ItemHoverFont = font; Refresh();};
	/// Returns hover font of toolbox item
	const wxFont & GetItemHoverFont() {return m_ItemHoverFont;}
	/// Sets font of selected toolbox item
	/**
	\param font - new font of selected toolbox item
	*/
	void SetItemSelectedFont(wxFont & font) {m_ItemSelectedFont = font; Refresh();};
	/// Returns font of selected toolbox item
	const wxFont & GetItemSelectedFont() {return m_ItemSelectedFont;}

	/// Sets normal font of toolbox tab
	/**
	\param font - new normal font of toolbox tab
	*/
	void SetTabNormalFont(wxFont & font) {m_TabNormalFont = font; Refresh();};
	/// Returns normal font of toolbox tab
	const wxFont & GetTabNormalFont() {return m_TabNormalFont;}
	/// Sets hover font of toolbox tab
	/**
	\param font - new hover font of toolbox tab	*/
	void SetTabHoverFont(wxFont & font) {m_TabHoverFont = font; Refresh();};
	/// Returns hover font of toolbox tab
	const wxFont & GetTabHoverFont() {return m_TabHoverFont;}
	/// Sets font of selected toolbox tab
	/**
	\param font - new font of selected toolbox tab
	*/
	void SetTabSelectedFont(wxFont & font) {m_TabSelectedFont = font; Refresh();};
	/// Returns font of selected toolbox tab
	const wxFont & GetTabSelectedFont() {return m_TabSelectedFont;}

	/// Sets PreDrawBackground flag (EXPERIMENTAL)
	/**	
	If PreDrawBackground set to true then wxToolBox creates tab backgrounds in memory 
	using skin bitmaps and uses this pre-created bitmap for painting toolbox tabs. 
	When using PreDrawBackground flag, component paints shaped skins a little bit faster.
	If PreDrawBackground set to false then components draws skin bitmaps directly.
	\param value - new value of PreDrawBackground flag
	*/
	void SetPreDrawBackgrounds(bool value);
	/// Returns PreDrawBackground flag (EXPERIMENTAL)
	bool GetPreDrawBackgrounds() {return m_PreDrawBackgrounds;}

	/// Sets SelectNewTab flag which specifies whether new tabs should be activated 
	/**
	\param value - new value of SelectNewTab flag
	*/
	void SetSelectNewTab(bool value) {m_SelectNewTab = value;}
	/// Returns SelectNewTab flag which specifies whether new tabs should be activated 
	bool GetSelectNewTab() {return m_SelectNewTab;}

	/// Sets ShowSelectionAlways flag which specifies if toolbox tabs should always have selected item
	/**
	\param value - new value of ShowSelectionAlways flag
	*/
	void SetShowSelectionAlways(bool value) {m_ShowSelectionAlways = value;}
	/// Returns ShowSelectionAlways flag which specifies if toolbox tabs should always have selected item
	bool GetShowSelectionAlways() {return m_ShowSelectionAlways;}

	/// Sets the colour which is used for painting highlighted part of bevels
	/**
	\param colour - colour that will be used for painting highlighted part of bevels
	*/
	void SetBorderHighlightColour(wxColour & colour) {m_BorderHighlightPen = wxPen(colour, 1, wxSOLID);}
	/// Returns the colour which is used for painting highlighted part of bevels
	wxColour GetBorderHighlightColour() {return m_BorderHighlightPen.GetColour();}
	/// Returns the pen which is used for painting highlighted part of bevels
	const wxPen & GetBorderHighlightPen() {return m_BorderHighlightPen;}
	/// Sets the colour which is used for painting shadowed part of bevels
	/**
	\param colour - colour that will be used for painting shadowed part of bevels
	*/
	void SetBorderShadowColour(wxColour & colour) {m_BorderShadowPen = wxPen(colour, 1, wxSOLID);}
	/// Returns the colour which is used for painting shadowed part of bevels
	wxColour GetBorderShadowColour() {return m_BorderShadowPen.GetColour();}
	/// Returns the pen which is used for painting shadowed part of bevels
	const wxPen & GetBorderShadowPen() {return m_BorderShadowPen;}

	/// Returns precalculated tab normal background bitmap
	wxBitmap & GetTabNormalBGPre() {return m_TabNormalBG;}
	/// Returns precalculated tab hover background bitmap
	wxBitmap & GetTabHoverBGPre() {return m_TabHoverBG;}
	/// Returns precalculated tab selected background bitmap
	wxBitmap & GetTabSelectedBGPre() {return m_TabSelectedBG;}
	/// Returns precalculated active tab normal background bitmap
	wxBitmap & GetTabNormalBGSelPre() {return m_TabNormalBGSel;}
	/// Returns precalculated active tab hover background bitmap
	wxBitmap & GetTabHoverBGSelPre() {return m_TabHoverBGSel;}
	/// Returns precalculated active tab selected background bitmap
	wxBitmap & GetTabSelectedBGSelPre() {return m_TabSelectedBGSel;}
	/// Returns precalculated item normal background bitmap
	wxBitmap & GetItemNormalBGPre() {return m_ItemNormalBG;}
	/// Returns precalculated item hover background bitmap
	wxBitmap & GetItemHoverBGPre() {return m_ItemHoverBG;}
	/// Returns precalculated item selected background bitmap
	wxBitmap & GetItemSelectedBGPre() {return m_ItemSelectedBG;}

	/// Swaps two toolbox tabs
	/**
	\param index1 - first tab index
	\param index2 - second tab index
	*/
	bool SwapTabs(int index1, int index2);
	/// Swaps two toolbox tabs
	/**
	\param tab1 - first tab
	\param tab2 - second tab
	*/
	bool SwapTabs(wxToolBoxTab * tab1, wxToolBoxTab * tab2);
	/// Returns index of specified toolbox tab. If tab was not found then returns -1
	/**
	\param tab - pointer to wxToolBox tab which index should be returned
	*/
	int IndexOf(wxToolBoxTab * tab);

	/// Determines new value of all Use*Background flags
	/**
	\param use_skin - new value of Use*Background flags
	*/
	void UseSkin(bool use_skin);

	/// Loads skin
	/**
	\param path_to_skin - path to skin-file
	*/
	bool LoadSkin(wxString path_to_skin);
	/// Saves skin
	/**
	\param path_to_skin - path to skin-file
	*/
	bool SaveSkin(wxString path_to_skin);

	/// Returns toolbox tab with specified index. If illegal index specified then returns NULL.
	/**
	\param index - index of toolbox tab
	*/
	wxToolBoxTab * operator [] (int index);

	/// Renames specified item (shows textbox for editing item's caption)
	/**
	\param item - item to be renamed
	*/
	void RenameItem(wxToolBoxItem * item);

	/// Switches the style of displaying ToolBox items
	/**
	\param value - new style of displaying items <B>wxTB_STYLE_LIST</B> or <B>wxTB_STYLE_THUMBNAILS</B>
	*/
	void SetStyle(unsigned int value);
	/// Returns the style of displaying ToolBox items
	unsigned int GetStyle() {return m_Style;}

	/// Loads toolbox tabs and items from file
	/**
	\param path_to_structure - path to toolbox structure file
	*/
	bool LoadStructure(wxString path_to_structure);
	/// Saves toolbox tabs and items to file
	/**
	\param path_to_structure - path to toolbox structure file
	*/
	bool SaveStructure(wxString path_to_structure);

	DECLARE_EVENT_TABLE()

	void OnPaint(wxPaintEvent & event);
	void OnEraseBackground(wxEraseEvent & event);
	void OnSize(wxSizeEvent & event);
	void OnTimer(wxTimerEvent & event);
	void OnContextMenu(wxContextMenuEvent & event);
	void OnMouseDown(wxMouseEvent & event);
	void OnMouseUp(wxMouseEvent & event);
	void OnMouseWheel(wxMouseEvent & event);
	void OnMouseMove(wxMouseEvent & event);
	void OnMouseLeave(wxMouseEvent & event);
	void OnTimer_LayoutElapsed(wxTimerEvent & event);
#ifdef __WXGTK__
	void OnWindowCreate(wxWindowCreateEvent & event);
#endif
public:	
};

bool CoordInRange(int coord, int mincoord, int maxcoord);

/// Returns a bitmap with simple arrow for toolbox scroll buttons
/**
\param dir - scroll direction
*/
wxBitmap GetStandardToolBoxArrow(wxScrollDirection dir);

// ========================================================================
//  wxToolBoxEvent
// ------------------------------------------------------------------------
/**	

<TABLE BORDER=0 CELLPADDING=5 CELLSPACING=2>
<TR><TH COLSPAN=2 ALIGN="left">
Event table macros
</TH></TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_TAB_ADDED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_TAB_ADDED command, which is generated by a wxToolBox control when adding new tab.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_TAB_SELECTED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_TAB_SELECTED command, which is generated by a wxToolBox control when selecting tab.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_TAB_DELETED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_TAB_DELETED command, which is generated by a wxToolBox control when deleting tab.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_TABS_SWAPPED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_TABS_SWAPPED command, which is generated by a wxToolBox control when swapping tabs.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_ITEM_ADDED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_ITEM_ADDED command, which is generated by a wxToolBox control when adding new item.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_ITEM_SELECTED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_ITEM_SELECTED command, which is generated by a wxToolBox control when selecting item.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_ITEM_DELETED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_ITEM_DELETED command, which is generated by a wxToolBox control when deleting item.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_ITEMS_SWAPPED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_ITEMS_SWAPPED command, which is generated by a wxToolBox control when swapping items.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_SKIN_LOADED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_SKIN_LOADED command, which is generated by a wxToolBox control when loading skin.</TD>
</TR>
<TR>
<TD BGCOLOR="#DDDDDD"><B>EVT_TB_SKIN_SAVED(id, fn)</B></TD>
<TD BGCOLOR="#DDDDDD">Process a wxEVT_TB_SKIN_SAVED command, which is generated by a wxToolBox control when saving skin.</TD>
</TR>
</TABLE>

\brief A toolbox event holds information about events associated with wxToolBox objects.
*/
class WXDLLEXPORT wxToolBoxEvent : public wxCommandEvent
{
	wxToolBoxTab * m_Tab;
	wxToolBoxItem * m_Item;
	int m_ToolObjectIndex1;
	int m_ToolObjectIndex2;
	DECLARE_DYNAMIC_CLASS(wxToolBoxEvent)
public:
	/// Default constructor
	/**
	\param commandType - event command type
	\param id - component's identifier
	*/
	wxToolBoxEvent(wxEventType commandType=0, int id=0);
	wxToolBoxEvent(const wxToolBoxEvent & evt);
	/// Creates a clone of event object :)
	wxEvent * Clone(void) const { return new wxToolBoxEvent(*this); }
	/// Associates toolbox tab with event
	/**
	\param tab - existing toolbox tab
	*/
	void SetTab(wxToolBoxTab * tab) {m_Tab = tab;}
	/// Returns toolbox tab associated with event
	wxToolBoxTab * GetTab() {return m_Tab;}
	/// Associates toolbox item with event
	/**
	\param item - existing toolbox item
	*/
	void SetItem(wxToolBoxItem * item) {m_Item = item;}
	/// Returns toolbox item associated with event
	wxToolBoxItem * GetItem() {return m_Item;}
	/** 
		\brief Sets index of tool object associated with event (used for handling 
		wxEVT_TB_TAB_DELETED and wxEVT_TB_ITEM_DELETED)
		\param value - new tool object index
	*/
	void SetToolObjectIndex(int value) {m_ToolObjectIndex1 = value;}
	/// Returns index of tool object associated with event (used for handling 
	int GetToolObjectIndex() {return m_ToolObjectIndex1;}
	/** 
		\brief Sets index of first tool object in wxEVT_TB_TABS_SWAPPED and
		wxEVT_TB_ITEMS_SWAPPED event handlers
		\param value - new tool object index
	*/
	void SetToolObjectIndex1(int value) {m_ToolObjectIndex1 = value;}
	/// Returns index of first tool object in wxEVT_TB_TABS_SWAPPED and wxEVT_TB_ITEMS_SWAPPED event handlers
	int GetToolObjectIndex1() {return m_ToolObjectIndex1;}
	/** 
		\brief Sets index of second tool object in wxEVT_TB_TABS_SWAPPED and
		wxEVT_TB_ITEMS_SWAPPED event handlers
		\param value - new tool object index
	*/
	void SetToolObjectIndex2(int value) {m_ToolObjectIndex2 = value;}
	/// Returns index of second tool object in wxEVT_TB_TABS_SWAPPED and wxEVT_TB_ITEMS_SWAPPED event handlers
	int GetToolObjectIndex2() {return m_ToolObjectIndex2;}
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_TAB_ADDED, 13666)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_TAB_SELECTED, 13667)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_TAB_DELETED, 13678)	
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_TABS_SWAPPED, 13669)
	/// Not implemented
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_TAB_MOVED, 13670)

	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_ITEM_ADDED, 13671)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_ITEM_SELECTED, 13672)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_ITEM_DELETED, 13673)		
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_ITEMS_SWAPPED, 13674)
	/// Not implemented
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_ITEM_MOVED, 13675)

	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_SKIN_LOADED, 13676)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_SKIN_SAVED, 13677)

	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_STRUCTURE_LOADED, 13678)
	DECLARE_EXPORTED_EVENT_TYPE(WXDLLEXPORT, wxEVT_TB_STRUCTURE_SAVED, 13679)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxToolBoxEventFunction)(wxToolBoxEvent & event);

#define EVT_TB_TAB_ADDED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_TAB_ADDED,		id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_TAB_SELECTED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_TAB_SELECTED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_TAB_DELETED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_TAB_DELETED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_TABS_SWAPPED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_TABS_SWAPPED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
/// Not implemented
#define EVT_TB_TAB_MOVED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_TAB_MOVED,		id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_TB_ITEM_ADDED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_ITEM_ADDED,		id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_ITEM_SELECTED(id, fn)	DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_ITEM_SELECTED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_ITEM_DELETED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_ITEM_DELETED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_ITEMS_SWAPPED(id, fn)	DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_ITEMS_SWAPPED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
/// Not implemented
#define EVT_TB_ITEM_MOVED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_ITEM_MOVED,		id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_TB_SKIN_LOADED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_SKIN_LOADED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_SKIN_SAVED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_SKIN_SAVED,		id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),

#define EVT_TB_STRUCTURE_LOADED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_STRUCTURE_LOADED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),
#define EVT_TB_STRUCTURE_SAVED(id, fn)		DECLARE_EVENT_TABLE_ENTRY( wxEVT_TB_STRUCTURE_SAVED,	id, -1, (wxObjectEventFunction) (wxEventFunction)  wxStaticCastEvent( wxToolBoxEventFunction, & fn ), (wxObject *) NULL ),

#endif
