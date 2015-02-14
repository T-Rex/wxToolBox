#ifndef IFLOOR_STDWX_H_
#define IFLOOR_STDWX_H_

#if defined(WIN32) || defined(WINDOWS)
#include <windows.h>
#include <winnt.h>
#define PLUGIN_EXPORTED_API	WXEXPORT
#else

#define PLUGIN_EXPORTED_API	extern "C"
#endif

#if defined(__LINUX__)
    #ifndef INT64_C
        #define INT64_C(c) (c ## LL)
        #define UINT64_C(c) (c ## ULL)
    #endif
#endif
// SYSTEM INCLUDES
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//#ifndef WX_PRECOMP
    #include "wx/wx.h"
//#endif

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/dnd.h>
#include <wx/filefn.h> 
#include <wx/timer.h>
#include <wx/image.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/tipwin.h>
#include <wx/valgen.h>
#include <wx/artprov.h>
#include <wx/gbsizer.h>
#include <wx/tooltip.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>
#include <wx/dynarray.h>
#include <wx/settings.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>

#include <wx/aui/aui.h>
#include <wx/xml/xml.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/propgriddefs.h>
#include <wx/propgrid/propgridiface.h>
#include <wx/propgrid/props.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

// APPLICATION INCLUDES
#endif
