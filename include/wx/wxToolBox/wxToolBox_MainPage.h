/**

\mainpage wxToolBox Overview
<CENTER>
<IMG SRC="wxToolBox.png">
</CENTER>
	
	wxToolBox is a powerful and extremely customizable skinnable component 
	which allows you to incorporate VS.NET-like toolbox or Outlook-like side bar
	into your applications.\n
	
<TABLE BORDER=0 CELLSPACING=5 WIDTH=100%>
<TR><TD WIDTH=100%></TD><TD><IMG SRC="DevCPP.png"></TD><TD><IMG SRC="VS2005.png"></TD><TD><IMG SRC="VS.png"></TD></TR>
</TABLE>

	<H3>Classes:</H3>
		wxToolObject\n
		wxToolBoxItem\n
		wxToolBoxTab\n
		wxToolBoxScrollButton\n
		wxToolItemEditorTextCtrl\n
		wxToolBoxBitmapSet\n
		wxToolObjectBackground\n
		wxToolBoxEvent\n
		wxToolBox\n

	<HR>
	<H3>Contents:</H3>
	\ref screenshots\n
	\ref basics\n
	\ref operating\n
	\ref backgrounds\n
	\ref skins\n
	\ref styles\n
	\ref events\n
	\ref child_windows\n
	\ref examples\n
	\ref platforms\n
	\ref license\n
	\ref howtoorder\n
	\ref developers\n 
	\ref links\n  
	<HR>

\section screenshots wxToolBox Screenshots

<CENTER>
<TABLE BORDER=0 CELLSPACING=30 CELLPADDING=0>
<TR>
<TD><IMG SRC="wxToolBox_Aero.jpg"></TD>
<TD><IMG SRC="wxToolBox_Quantria.jpg"></TD>
<TD><IMG SRC="wxToolBox_Classic.jpg"></TD>
<TD><IMG SRC="wxToolBox_Standard.jpg"></TD>
</TR></TABLE><BR>
<TABLE BORDER=0 CELLSPACING=30 CELLPADDING=0>
<TR>
<TD><IMG SRC="wxToolBox_Aero_Thumbnails.jpg"></TD>
<TD><IMG SRC="wxToolBox_Anaconda.jpg"></TD>
<TD><IMG SRC="wxToolBox_Glossy.jpg"></TD>
<TD><IMG SRC="wxToolBox_Golden.jpg"></TD>
</TR>
</TABLE>
</CENTER>


\section basics Creating and populating wxToolBox with tabs and items


As seen here, wxToolBox is constructed in the same way as other wxWidgets controls:

\code
// Necessary header file
#include <wx/wxToolBox/wxToolBox.h>

m_ToolBox = new wxToolBox(this,	// parent window
	ID_TOOL_BOX,	// window ID
	wxDefaultPosition,	// position
	wxSize(150,400),	// size
	wxNO_BORDER	// style
	);
\endcode

wxToolBox is usually populated with tabs and items like this:

\code
wxToolBox * m_ToolBox;
...
// We have to create tabs first...
...
wxToolBoxTab * tab1 = new wxToolBoxTab(wxT("Tab 1"));
wxToolBoxTab * tab2 = new wxToolBoxTab(wxT("Tab 2"));
wxToolBoxTab * tab3 = new wxToolBoxTab(wxT("Tab 3"));
wxToolBoxTab * tab4 = new wxToolBoxTab(wxT("Tab 4"));

// Then we have to specify image indexes for our tabs (optional)

tab1->SetImageIndex(rand()%m_ToolBox->GetTabImageList().Count());
tab3->SetImageIndex(rand()%m_ToolBox->GetTabImageList().Count());
tab4->SetImageIndex(rand()%m_ToolBox->GetTabImageList().Count());

// Then we add tabs to our component

m_ToolBox->AddTab(tab1);
m_ToolBox->AddTab(tab2);
m_ToolBox->AddTab(tab3);
m_ToolBox->AddTab(tab4);

// After the tabs have been added, we can add items

wxToolBoxItem item1("Item1", 0);	

for(int i = 0; i < 120; i++)
{
	// So, we can set item's caption...

	item1.SetCaption(wxString::Format(_("Item %i"), i));

	if(m_ToolBox->GetItemImageList().Count())
	{
		// ... and image index

		item1.SetImageIndex(rand()%m_ToolBox->GetItemImageList().Count());
	}

	// Then we can add item to our tab
	if(i < 30)
	{
		tab1->AddItem(item1);
	}
	else if(i >= 30 && i < 60)
	{
		tab2->AddItem(item1);
	}
	else if(i >= 60 && i < 90)
	{
		tab3->AddItem(item1);
	}
	else 
	{			
		tab4->AddItem(item1);
	}
}
\endcode


\section operating Operating with tabs and items


All operations on properties should be done via wxToolBox's methods.

- Retrieving the amount of tabs:\n
You can get amount of avaliable toolbox tabs by accessing list of toolbox tabs(with 
wxToolBox::GetTabs method) or by calling wxToolBox::GetTabCount method.
- You can get the pointer to selected tab using wxToolBox::GetSelectedTab method.
- It is very simple to create new tabs and add populate them with items:

\code
// This example shows hows how to create new tab and how to add this tab to wxToolBox component
wxToolBoxTab * tab = new wxToolBoxTab(tab_name_text_ctrl->GetValue(), tab_icon_spin_ctrl->GetValue());
m_ToolBox->AddTab(tab);
...
// This example shows how to add items to the tab with specified index
wxToolBoxTab * tab = m_ToolBox->GetTab(item_tab_spin_ctrl->GetValue());	
if(!tab) return;
wxToolBoxItem item(item_name_text_ctrl->GetValue(), item_icon_spin_ctrl->GetValue());	
tab->AddItem(item);
\endcode

- You can manually rename toolbox items and tabs using built-in editor 
(textbox that allows to change the caption of tool object and disappears when 
user presses Enter or when user presses mouse button outside this textbox)

\code
wxToolBoxTab * tab;
...
// Here we display editor which allows to rename item with specified index on toolbox tab
m_ToolBox->RenameItem((*tab)[index]);
...
// Here we display editor which allows to rename specified tab
m_ToolBox->RenameItem(tab);
\endcode

- wxToolBox component allows customization of large amount of parameters of its 
tool objects (caption, position, size etc.)

\code
wxToolBox * m_ToolBox;

wxToolBoxItem * item = (*tab)[index];
item->SetCaption(_("Some Item"));
item->SetSelected(true);
item->SetToolTip(_("New ToolTip"));
item->SetDeletable(false);
item->SetMovable(true);

wxToolBoxTab * tab = m_ToolBox->GetTab(m_ToolBox->GetTabCount()-1);
if(tab)
{
	tab->SetSelected(true);
	tab->SwapItems(0, tab->GetItemCount()-1);
	if(!tab->GetItemCount())
	{
		tab->SetEnabled(false);
	}
	tab->SetImageIndex(2);
}

// Here we set new height of toolbox items and height of free space between items
m_ToolBox->SetItemHeight(23);
m_ToolBox->SetItemSpacing(2);
// Here we set new height of toolbox tabs and height of free space between tabs
m_ToolBox->SetTabHeight(26);
m_ToolBox->SetTabSpacing(0);

\endcode

- wxToolBox supports loading/saving of its structure (tabs and items) form/to XML file.

\code
wxToolBox * m_ToolBox;
// Load structure from file
m_ToolBox->LoadStructrue(wxT("my_file_with_structure.xml"));
// Save structure to file
m_ToolBox->SaveStructrue(wxT("my_file_with_structure.xml"));
\endcode

\section backgrounds wxToolBox Backgrounds

Each tool object (toolbox tab, toolbox item or toolbox button) has skinning options.
It means that aech tool object can use background images.
wxToolBox component contains 3 sets of background images:

wxToolBox::m_TabBackground - set of background bitmaps for painting toolbox tabs.
You can use wxToolBox::GetTabBackground method to access it.\n
wxToolBox::m_ItemBackground - set of background bitmaps for painting toolbox items
You can use wxToolBox::GetItemBackground method to access it.\n
wxToolBox::m_ButtonBackground - set of background bitmaps for painting toolbox buttons
You can use wxToolBox::GetButtonBackground method to access it.\n

Each of this 3 sets is an object of wxToolObjectBackground class and each such object
also contains 3 sets of bitmaps:\n

	- wxToolObjectBackground::GetNormalBackground - set of bitmaps that are used for painting tool 
	objects that have normal state.\n	
	- wxToolObjectBackground::GetHoverBackground - set of bitmaps that are used for painting tool 
	objects that have hover state (tool object has hover state when user moves mouse pointer over it).\n	
	- wxToolObjectBackground::GetSelectedBackground - set of bitmaps that are used for painting tool 
	objects that have selected state (tool object has selected state when user presses mouse 
	button on it).\n

Each of previous 3 sets of bitmaps contains 3 bitmaps:\n

- wxToolBoxBitmapSet::GetLeftBitmap - bitmap for painting left part of tool object.
- wxToolBoxBitmapSet::GetMiddleBitmap - bitmap for painting middle part of tool object.
- wxToolBoxBitmapSet::GetRightBitmap - bitmap for painting right part of tool object.

Now let's see how to set background bitmaps manually

\code
wxToolBox * m_ToolBox;
wxToolBoxBitmapSet normal(wxT("./res/TAB_Skin_Left.jpg"),
			wxT("./res/TAB_Skin_Middle.jpg"),
			wxT("./res/TAB_Skin_Right.jpg"));
wxToolBoxBitmapSet hover(wxT("./res/TAB_Skin_Left_Hov.jpg"),
			wxT("./res/TAB_Skin_Middle_Hov.jpg"),
			wxT("./res/TAB_Skin_Right_Hov.jpg"));
wxToolBoxBitmapSet selected(wxT("./res/TAB_Skin_Left_Sel.jpg"),
			wxT("./res/TAB_Skin_Middle_Sel.jpg"),
			wxT("./res/TAB_Skin_Right_Sel.jpg"));
m_ToolBox->SetTabBackground(normal, hover, selected);
m_ToolBox->SetUseTabBackground(true);
m_ToolBox->SetButtonBackground(normal, hover, selected);
m_ToolBox->SetUseButtonBackground(true);
\endcode

If tool object's width is bigger that summary width of left, middle and right part of its skin then
middle bitmap will be tiled

\section skins Working with skins

wxToolBox can use XML-based skin files. Structure of wxToolBox skins is very simple to 
understand and it is very simple to create skins manually using notepad or other text editor.

Here is an example of skin file

\code
<?xml version="1.0" encoding="utf-8"?>
<wxToolBoxSkin>
  <Style StyleValue="0"/>
  <BackgroundColour Red="35" Green="42" Blue="68"/>
  <BorderHighlightColour Red="212" Green="208" Blue="200"/>
  <BorderShadowColour Red="64" Green="64" Blue="64"/>
  <Background Path="Background.png"/>
  <TabIcons Path="ItemIcons.png" ImageWidth="16" ImageHeight="16"/>
  <ItemIcons Path="ItemIcons.png" ImageWidth="16" ImageHeight="16"/>
  <Flags ItemsTransparent="1" SelectedItemTransparent="1" UseBackground="1" 
	UseStdArrows="1" SelectNewTab="1" PreDrawBackgrounds="0" ShowSelectionAlways="0"/>
  <Buttons>
    <ButtonHoverColour Red="212" Green="208" Blue="200"/>
    <ButtonNormalColour Red="170" Green="170" Blue="170"/>
    <ButtonSelectedColour Red="212" Green="208" Blue="200"/>
    <Flags UseButtonBackground="1"/>
    <NormalBackground Left="TAB_Skin_Left.PNG" Middle="TAB_Skin_Middle.PNG" Right="TAB_Skin_Right.PNG"/>
    <HoverBackground Left="TAB_Skin_Left_Hov.PNG" Middle="TAB_Skin_Middle_Hov.PNG" Right="TAB_Skin_Right_Hov.PNG"/>
    <SelectedBackground Left="TAB_Skin_Left_Sel.PNG" Middle="TAB_Skin_Middle_Sel.PNG" Right="TAB_Skin_Right_Sel.PNG"/>
  </Buttons>
  <Tabs>
    <TabHoverColour Red="212" Green="208" Blue="200"/>
    <TabNormalColour Red="170" Green="170" Blue="170"/>
    <TabSelectedColour Red="236" Green="233" Blue="216"/>
    <TabTextColour Red="255" Green="255" Blue="255"/>
    <Flags UseTabBackground="1"/>
    <Sizes TabHeight="26" TabSpacing="1"/>
    <NormalBackground Left="TAB_Skin_Left.PNG" Middle="TAB_Skin_Middle.PNG" Right="TAB_Skin_Right.PNG"/>
    <HoverBackground Left="TAB_Skin_Left_Hov.PNG" Middle="TAB_Skin_Middle_Hov.PNG" Right="TAB_Skin_Right_Hov.PNG"/>
    <SelectedBackground Left="TAB_Skin_Left_Sel.PNG" Middle="TAB_Skin_Middle_Sel.PNG" Right="TAB_Skin_Right_Sel.PNG"/>
    <TabNormalFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="90"/>
    <TabHoverFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="92"/>
    <TabSelectedFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="90"/>
  </Tabs>
  <Items>
    <ItemHoverColour Red="79" Green="95" Blue="121"/>
    <ItemNormalColour Red="35" Green="42" Blue="68"/>
    <ItemSelectedColour Red="146" Green="151" Blue="164"/>
    <ItemTextColour Red="255" Green="255" Blue="255"/>
    <Flags UseItemNormalBackground="0" UseItemHoverBackground="0" UseItemSelectedBackground="0"/>
    <Sizes ItemHeight="26" ItemSpacing="1"/>
    <NormalBackground Left="" Middle="" Right=""/>
    <HoverBackground Left="Item_Skin_Left_Hov.PNG" Middle="Item_Skin_Middle_Hov.PNG" Right="Item_Skin_Right_Hov.PNG"/>
    <SelectedBackground Left="Item_Skin_Left_Sel.PNG" Middle="Item_Skin_Middle_Sel.PNG" Right="Item_Skin_Right_Sel.PNG"/>
    <ItemNormalFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="90"/>
    <ItemHoverFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="90"/>
    <ItemSelectedFont DefaultEncoding="-1" FaceName="MS Shell Dlg 2" Family="74" PointSize="8" Style="90" Underlined="0" Weight="90"/>
  </Items>
</wxToolBoxSkin>
\endcode

You can separately enable or disable skinning of toolbox tabs, toolbox items or toolbox buttons:
\code
wxToolBox * m_ToolBox;
...

m_ToolBox->SetUseTabBackground(true);
m_ToolBox->SetUseButtonBackground(false);
m_ToolBox->SetUseItemBackground(false);
...
// If you want to enable or disable skinning of all tool objects, you can use wxToolBox::UseSkin method
...

m_ToolBox->UseSkin(true);
\endcode

wxToolBox component allows to specify background colours, foreground colours and fonts for all tool objects.\n
All colours and fonts separated to 3 categories: <B>Normal</B>, <B>Hover</B> and <B>Selected</B>.\n

<B>Normal</B> category is used for painting tool objects with normal state\n
<B>Hover</B> category is used for painting tool objects when user moves mouse pointer over them\n
<B>Selected</B> category is used for painting tool objects when user presses mouse button on them\n

Here is an example of customizing colours and fonts:
\code
wxToolBox * m_ToolBox;
...
m_ToolBox->SetBackgroundColour(wxColour(35, 42, 68));

m_ToolBox->SetItemNormalColour(wxColour(35, 42, 68));
m_ToolBox->SetItemHoverColour(wxColour(79, 95, 121));
m_ToolBox->SetItemSelectedColour(wxColour(146, 151, 164));
m_ToolBox->SetTabNormalColour(wxColour(170, 170, 170));
m_ToolBox->SetTabSelectedColour(wxColour(236, 233, 216));
m_ToolBox->SetButtonNormalColour(wxColour(170, 170, 170));

m_ToolBox->SetItemTextColour(*wxWHITE);

wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
m_ToolBox->SetItemNormalFont(font);
m_ToolBox->SetItemHoverFont(font);
font.SetWeight(wxFONTWEIGHT_BOLD);
m_ToolBox->SetItemHoverFont(font);
\endcode

One more important thing is item transparency. wxToolBox allows to specify transparency 
for selected state of item separately:
\code
wxToolBox * m_ToolBox;
...

m_ToolBox->SetItemsTransparent(true);
m_ToolBox->SetSelectedItemTransparent(false);
\endcode

Also wxToolBox component allows to specify colours for painting bevels of tool objects:
\code
// Colour for painting highlighted part of bevels
m_ToolBox->SetBorderHighlightColour(wxColour(192, 192, 192));
// Colour for painting shadowed part of bevels
m_ToolBox->SetBorderShadowColour(wxColour(100, 100, 100));
\endcode

There are several skins available for downloading:
- <a href="skins/Aero.zip">Aero (list/thumbnails)</a>
- <a href="skins/Anaconda.zip">Anaconda</a>
- <a href="skins/Glossy.zip">Glossy</a>
- <a href="skins/Golden.zip">Golden</a>
- <a href="skins/Quantria.zip">Quantria</a>
- <a href="skins/Standard.zip">Standard</a>

\section styles Styles

Currently wxToolBox supports two styles: <B>List</B>, <B>Icons</B> and <B>Thumbnails</B>.\n
- <B>List</B> (wxTB_STYLE_LIST) - wxToolBox looks like an ordinary ToolBox.\n
- <B>Icons</B> (wxTB_STYLE_ICONS) - wxToolBox displays toolbox items as icons without text. This style allows to place several toolbox items in the same row.\n
- <B>Thumbnails</B> (wxTB_STYLE_THUMBNAILS) - wxToolBox has the same behaviour as Sidebar from Outlook (Outlook Bar).\n

<B>Thumbnails</B> style allows you to incorporate a side bar control into your applications. 
The side bar is one of the brightest parts of Outlook's interface. It is elegant, convenient, and intuitive: often 
unnoticeable yet taking the major role in the interface navigation.

<CENTER>
<TABLE BORDER=0 CELLSPACING=10 CELLPADDING=0>
<TR>
<TD><IMG BORDER=0 SRC="wxToolBox_Aero.jpg"></TD>
<TD><IMG BORDER=0 SRC="wxToolBox_Aero_Icons.jpg"></TD>
<TD><IMG BORDER=0 SRC="wxToolBox_Aero_Thumbnails.jpg"></TD>
</TR>
</TABLE>
</CENTER>

It is very simple to switch between ToolBox styles:

\code
wxToolBox * m_ToolBox;
...
m_ToolBox->SetItemHeight(70);
m_ToolBox->SetItemImageHeight(48);
m_ToolBox->SetItemImageWidth(48);
m_ToolBox->SetStyle(wxTB_TAB_THUMBNAILS);
\endcode

\section events Event Handling

wxToolBox allows to handle several types of events. wxEVT_TB_ITEM_SELECTED event which occurs when toolbox item is selected
by user, is used most often. You should use EVT_TB_ITEM_SELECTED(id, fn) macro to handle this event. For complete list of
event types see wxToolBoxEvent class reference.

\code
BEGIN_EVENT_TABLE(ToolBoxHostFrame, wxFrame)
...
EVT_TB_TAB_SELECTED(10001, ToolBoxHostFrame::OnToolBoxTabSelected)
...
EVT_TB_ITEM_ADDED(10001, ToolBoxHostFrame::OnToolBoxItemAdded)
EVT_TB_ITEM_SELECTED(10001, ToolBoxHostFrame::OnToolBoxItemSelected)
...
END_EVENT_TABLE()

...

void ToolBoxHostFrame::OnToolBoxTabSelected(wxToolBoxEvent & event)
{	
	wxToolBoxTab * tab = event.GetTab();	
	if(tab)
	{
		// Do something...
	} 
	else
	{
		// Error occured...
	}	
}

void ToolBoxHostFrame::OnToolBoxItemAdded(wxToolBoxEvent & event)
{	
	wxToolBoxItem * item = event.GetItem();
	if(item)
	{
		// Do something...
	}
	else
	{
		// Error occured...
	}	
}

void ToolBoxHostFrame::OnToolBoxItemSelected(wxToolBoxEvent & event)
{	
	wxToolBoxItem * item = event.GetItem();	
	if(item)
	{
		wxToolBoxTab * tab = item->GetParentTab();
		if(tab)
		{
			// Do something with tab...
		}
		// Do something...
	}
	else
	{
		// Error occured...
	}
}
\endcode

\section child_windows Associating child windows

wxToolBox tabs can contain child windows. If you want to associate a control with wxToolBoxTab then you should use
wxToolBoxTab::SetChildWindow method. E.g.

\code
wxToolBoxTab * tab5 = new wxToolBoxTab(wxT("Tab 5"));
wxTreeCtrl * treeCtrl = new wxTreeCtrl(m_ToolBox, wxID_ANY, 
	wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
wxTreeItemId root = treeCtrl->AddRoot(_("Root"));
treeCtrl->AppendItem(root, _("Item 1"));
treeCtrl->AppendItem(root, _("Item 2"));
treeCtrl->AppendItem(root, _("Item 3"));
treeCtrl->Expand(root);
tab5->SetChildWindow(treeCtrl);
...
m_ToolBox->AddTab(tab5);
\endcode

wxToolBox can contain not only simple controls e.g. wxListBox or wxTreeCtrl. 
You can also place containers such as wxPanel inside wxToolBox tab

\code
wxToolBoxTab * tab4 = new wxToolBoxTab(wxT("Tab 4"));	
tab4->SetImageIndex(rand()%m_ToolBox->GetTabImageList().Count());

wxPanel * tab4panel = new wxPanel(m_ToolBox, wxID_ANY, 
	wxDefaultPosition, wxDefaultSize, wxNO_BORDER);	
wxBoxSizer *tab4panel_sizer = new wxBoxSizer(wxVERTICAL);
tab4panel->SetSizer(tab4panel_sizer);

wxFlexGridSizer * flexgrid_sizer = new wxFlexGridSizer(2, 2, 5, 5);

wxStaticText * fn_label = new wxStaticText(tab4panel, wxID_ANY, _("First Name:"));
wxStaticText * ln_label = new wxStaticText(tab4panel, wxID_ANY, _("Last Name:"));
wxStaticText * em_label = new wxStaticText(tab4panel, wxID_ANY, _("e-mail:"));

wxTextCtrl * fn_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);
wxTextCtrl * ln_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);
wxTextCtrl * em_text = new wxTextCtrl(tab4panel, wxID_ANY, wxEmptyString);

fn_label->SetForegroundColour(*wxWHITE);
ln_label->SetForegroundColour(*wxWHITE);
em_label->SetForegroundColour(*wxWHITE);

flexgrid_sizer->AddGrowableCol(1, 1);
flexgrid_sizer->Add(fn_label, 0, wxALIGN_LEFT);
flexgrid_sizer->Add(fn_text, 0, wxGROW);
flexgrid_sizer->Add(ln_label, 0, wxALIGN_LEFT);
flexgrid_sizer->Add(ln_text, 0, wxGROW);
flexgrid_sizer->Add(em_label, 0, wxALIGN_LEFT);
flexgrid_sizer->Add(em_text, 0, wxGROW);

m_tab4gauge = new wxGauge(tab4panel, wxID_ANY, 100, 
	wxDefaultPosition, wxSize(-1,20), 
	wxHORIZONTAL|wxGA_SMOOTH|wxNO_BORDER);

long submit_id = wxNewId();
wxButton * submit_btn = new wxButton(tab4panel, submit_id, _("Submit"));
Connect(submit_id, wxEVT_COMMAND_BUTTON_CLICKED, 
		wxObjectEventFunction(&wxToolBoxMinimalTestFrame::OnSubmit));

tab4panel_sizer->Add(flexgrid_sizer, 0, wxGROW|wxALL, 5);
tab4panel_sizer->Add(m_tab4gauge, 0, wxGROW|wxALL, 5);
tab4panel_sizer->Add(submit_btn, 0, wxGROW|wxALL, 5);

tab4->SetChildWindow(tab4panel);
\endcode

<CENTER>
<TABLE BORDER=0 CELLSPACING=30 CELLPADDING=0>
<TR>
<TD><IMG SRC="wxToolBox_with_child_control1.jpg"></TD>
<TD><IMG SRC="wxToolBox_with_child_control2.jpg"></TD>
</TR></TABLE>
</CENTER>

\section examples Examples and Documentation

There are some demo applications avaliable for downloading:

<B>ToolBox Skin Editor</B> - utility for creating and editing wxToolBox skins. It demonstrates 
principles of usage of wxToolBox component.

<A HREF="wxToolBoxSkinEditor-bin.zip"><B>ToolBox SkinEditor</B> executable for MS Windows</A>\n
<A HREF="wxToolBoxSkinEditor-src.zip"><B>ToolBox SkinEditor</B> source (MSVC 7.1 project)</A> without component's source code\n
<A HREF="wxDev-CPP-demo.zip">wxDev-CPP Demo Project</A>\n
<A HREF="MinGWStudio-demo.zip">MinGWStudio Demo Project for Linux</A>\n
<A HREF="wxToolBox-docs.zip">wxToolBox documentation</A>\n

<CENTER>
<B>ToolBox SkinEditor</B> ScreenShot


<IMG SRC="SkinEditor.jpg">
</CENTER>

\section platforms Supported platforms

	- wxMSW
	- wxGTK
	- maybe other :)
<CENTER>
<TABLE BORDER=0 CELLSPACING=30>
<TR ALIGN="center">
<TD><B>wxMSW ScreenShot</B></TD>
<TD><B>wxGTK ScreenShot</B></TD>
</TR>
<TR VALIGN="top">
<TD><IMG SRC="wxToolBox_Aero.jpg"></TD>
<TD><IMG SRC="wxToolBox_Aero_GTK.jpg"></TD>
</TR>
</TABLE>
</CENTER>

\section license License

wxToolBox is a proprietary component distributed under <a href="license.txt">commercial license</a>.

\section howtoorder How To Order
You can <A HREF="&#109;&#97;i&#108;&#84;o&#58;t-rex&#64;mail&#46;zp&#46;ua">contact developers</A> 
in case if you want to purchase wxToolBox. The price of wxToolBox (including source code) is <B>$49.95</B>.

\section developers Developers
	
You can contact <a href="&#109;&#97;i&#108;&#84;o&#58;t-rex&#64;mail&#46;zp&#46;ua">Volodymir (T-Rex) Tryapichko</a> in case if you have any questions about wxToolBox.

\section links wxToolBox homepage and other links
	
	Visit <A HREF="http://www.babloid.com/wxcomponents/wxToolBox/index.html">project's homepage</A> to get more info\n
	You can download demo-executable <A HREF="http://www.babloid.com/index.php?page=downloads">here</A>\n

	<!--<CENTER><img src="/cgi-sys/Count.cgi?df=wxToolBox.dat|display=Counter|ft=6|md=6|frgb=255;255;255|dd=B"></CENTER>--!>

*/